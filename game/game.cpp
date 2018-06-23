#include "game.h"

#define TESTING
#ifdef TESTING
#include <cstdio>
#endif

#include <cwchar>
#include <thread>

#include "events/eventmanager.h"

Game::Game() :
    hStdOut(GetStdHandle(STD_OUTPUT_HANDLE)),
    scoreDelta(0),
    altDown(false), end(false), removeMode(false)
{
    letterBag.loadPolishScrabble();
}

void Game::run()
{
    // setup
    generateLetterValues();
    setupBoard();
    setupWidgets();
    bindEvents();
    // Start event manager
    EventManager::start();

    // You don't want your players to get seizures
    disableSeizureMode();

    // Set current player
    players.front().activate();
    currentPlayer = players.begin();
    while(!end) {
        resetTickVars();
        // Input
        EventManager::pollEvents();

        if (commited)
            nextPlayer();

        // Prepare widgets
        updateScoreboard();
        updateLettersWidget();
        paintTiles();
        paintScoreDelta();

        // Display
        repaint();

        // Wait
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Game::repaint()
{
    boardWidget.display(hStdOut);
    legendWidget.display(hStdOut);
    scoresWidget.display(hStdOut);
    lettersWidget.display(hStdOut);
}

void Game::setLanguage(Game::Language l)
{
    lang = l;
}

void Game::addPlayer(const wchar_t *name)
{
    if (wcslen(name) > MAX_PLAYER_NAME_LEN)
        players.emplace_back(Player(L"LONGNAME", false));
    else
        players.emplace_back(Player(name, false));
    players.back().takeLetters(&letterBag, MAX_LETTERS_ON_HAND);
}

void Game::keyPressEvent(KeyEvent *e)
{
    if (commited) // Ignore all inputs after commiting tiles on board
        return;
    const short &key = e->key();
    if (e->key() == VK_RMENU)
        altDown = true;
    else if (key == VK_LEFT)
        cursor.left();
    else if (key == VK_RIGHT)
        cursor.right();
    else if (key == VK_UP)
        cursor.up();
    else if (key == VK_DOWN)
        cursor.down();
    else if (key == VK_RETURN)
        commitTiles();
    else if (key == VK_BACK) {
        if (altDown)
            undoAll();
        else
            undoTile();
    } else if (key == VK_TAB) {
        disableSeizureMode();
    } else if (key == VK_DELETE) { // toggle remove mode
        undoAll();
        removeMode ^= 1;
    }
    // Virtual key codes match ascii values of letters
    else if (e->key() >= 'A' && e->key() <= 'Z') {
        char altFrom[10]    =  "ACELNOSXZ";
        wchar_t altTo[10]   = L"ĄĆĘŁŃÓŚŹŻ";
        if (altDown) {
            for (int i = 0; i < 10; i++) {
                if (altFrom[i] == key) {
                    placeLetter(altTo[i]);
                    break;
                }
            }
        } else {
            placeLetter(wchar_t(key));
        }
    } else if (key == VK_SPACE) {
        placeLetter(SCRABBLE_BLANK);
    }
    cursor.fitInBox(0,0,SCRABBLE_BOARD_SIZE-1, SCRABBLE_BOARD_SIZE-1);
}

void Game::keyReleaseEvent(KeyEvent *e)
{
    if (e->key() == VK_RMENU)
        altDown = false;
}

void Game::disableSeizureMode()
{
    // Disable the annoying console cursor
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hStdOut, &cci);
    cci.bVisible = false;
    SetConsoleCursorInfo(hStdOut, &cci);
    //boardWidget.toggleCursor();
}

void Game::bindEvents()
{
    EventManager::connect(KeyPress, [this](Event* e) {
        keyPressEvent(dynamic_cast<KeyEvent*>(e));
    });
    EventManager::connect(KeyRelease, [this](Event* e) {
        keyReleaseEvent(dynamic_cast<KeyEvent*>(e));
    });
    EventManager::newTimer(std::chrono::milliseconds(1000), [this](TimerEvent*) {
        disableSeizureMode();
    });
}

void Game::setupWidgets()
{
    boardWidget.resize(19,19);
    boardWidget.setBorder(Letter('#', ConsoleColor(GREEN, DARK_GREEN)), 2);
    boardWidget.setCursor(&cursor);

    legendWidget.resize(10, 19);
    legendWidget.move(22,0);
    legendWidget.setBorder(Letter('#', ConsoleColor(GREEN, DARK_GREEN)), 1);
    legendWidget.setBackgroundColor(DARK_TEAL);
    legendWidget.setString(0, 0,L"LEGEND:", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setString(1, 0,L"========", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setLetter(2, 0,Letter(' ', ConsoleColor(BLACK, DARK_RED)));
    legendWidget.setString(2, 1,L"-triple\n word", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setString(4, 0,L"--------", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setLetter(5, 0,Letter(' ', ConsoleColor(BLACK, RED)));
    legendWidget.setString(5, 1,L"-double\n word", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setString(7, 0,L"--------", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setLetter(8, 0,Letter(' ', ConsoleColor(BLACK, DARK_BLUE)));
    legendWidget.setString(8, 1,L"-triple\n letter", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setString(10,0,L"--------", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setLetter(11,0,Letter(' ', ConsoleColor(BLACK, BLUE)));
    legendWidget.setString(11,1,L"-double\n letter", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setString(13,0,L"--------", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setLetter(14,0,Letter(' ', ConsoleColor(BLACK, GREY)));
    legendWidget.setString(14,1,L"-cursor", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setString(15,0,L"--------", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setLetter(16,0,Letter(L'░', ConsoleColor(BLACK, GREY)));
    legendWidget.setString(16,1,L"-blank", ConsoleColor(WHITE, DARK_TEAL));

    scoresWidget.resize(15,11);
    scoresWidget.move(35,0);
    scoresWidget.setBorder(Letter('#', ConsoleColor(GREEN, DARK_GREEN)), 1);
    scoresWidget.setBackgroundColor(DARK_TEAL);
    scoresWidget.setString(0,3,L"SCORES:", ConsoleColor(WHITE, DARK_TEAL));
    scoresWidget.setString(1,0,L"=============", ConsoleColor(WHITE, DARK_TEAL));
    scoresWidget.setString(3,0,L"=============", ConsoleColor(WHITE, DARK_TEAL));
    scoresWidget.setString(5,0,L"=============", ConsoleColor(WHITE, DARK_TEAL));
    scoresWidget.setString(7,0,L"=============", ConsoleColor(WHITE, DARK_TEAL));

    lettersWidget.resize(15,6);
    lettersWidget.move(35,13);
    lettersWidget.setBorder(Letter('#', ConsoleColor(GREEN, DARK_GREEN)), 1);
    lettersWidget.setString(0,4,L"LETTERS", ConsoleColor(GREEN, DARK_GREEN), true);
    lettersWidget.setString(0,0,L"─┬─┬─┬─┬─┬─┬─", ConsoleColor(WHITE, GREY));
    lettersWidget.setString(1,0,L" | | | | | | ", ConsoleColor(WHITE, GREY));
    lettersWidget.setString(2,0,L" | | | | | | ", ConsoleColor(WHITE, GREY));
    lettersWidget.setString(3,0,L"─┴─┴─┴─┴─┴─┴─", ConsoleColor(WHITE, GREY));
    lettersWidget.setBackgroundColor(GREY);
}

void Game::setupBoard()
{
    tiles.resize(SCRABBLE_BOARD_SIZE);
    for (auto &vtile: tiles)
        vtile.resize(SCRABBLE_BOARD_SIZE);

    const char* sboard =  // T - tword, D - dword, t - tletter, d - dletter
            "T  d   T   d  T"
            " D   t   t   D "
            "  D   d d   D  "
            "d  D   d   D  d"
            "    D     D    "
            " t   t   t   t "
            "  d   d d   d  "
            "T  d   D   d  T"
            "  d   d d   d  "
            " t   t   t   t "
            "    D     D    "
            "d  D   d   D  d"
            "  D   d d   D  "
            " D   t   t   D "
            "T  d   T   d  T";
    const int N = SCRABBLE_BOARD_SIZE;
    for (unsigned i = 0; i < N; i++) {
        for (unsigned j = 0; j < N; j++) {
            if (sboard[N*i+j] == 'T')
                tiles[i][j].setBonus(BONUS_TRIPLE_WORD);
            else if (sboard[N*i+j] == 'D')
                tiles[i][j].setBonus(BONUS_DOUBLE_WORD);
            else if (sboard[N*i+j] == 't')
                tiles[i][j].setBonus(BONUS_TRIPLE_LETTER);
            else if (sboard[N*i+j] == 'd')
                tiles[i][j].setBonus(BONUS_DOUBLE_LETTER);
        }
    }
}

void Game::generateLetterValues()
{
//    letterValue = {
//        {L'A', 1},
//        {L'E', 1},
//        {L'I', 1},
//        {L'N', 1},
//        {L'O', 1},
//        {L'R', 1},
//        {L'S', 1},
//        {L'W', 1},
//        {L'Z', 1},

//        {L'C', 2},
//        {L'D', 2},
//        {L'K', 2},
//        {L'L', 2},
//        {L'M', 2},
//        {L'P', 2},
//        {L'T', 2},
//        {L'Y', 2},

//        {L'B', 3},
//        {L'G', 3},
//        {L'H', 3},
//        {L'J', 3},
//        {L'Ł', 3},
//        {L'U', 3},

//        {L'Ą', 5},
//        {L'Ę', 5},
//        {L'F', 5},
//        {L'Ó', 5},
//        {L'Ś', 5},
//        {L'Ż', 5},

//        {L'Ć', 6},

//        {L'Ń', 7},

//        {L'Ź', 9},

//        {L' ', 0}, // placeholder for blank
//    };
    letterValue = {
        // reorganized
        {L'A', 1},
        {L'B', 3},
        {L'C', 2},
        {L'D', 2},
        {L'E', 1},
        {L'F', 5},
        {L'G', 3},
        {L'H', 3},
        {L'I', 1},
        {L'J', 3},
        {L'K', 2},
        {L'L', 2},
        {L'M', 2},
        {L'N', 1},
        {L'O', 1},
        {L'P', 2},
        {L'R', 1},
        {L'S', 1},
        {L'T', 2},
        {L'U', 3},
        {L'W', 1},
        {L'Y', 2},
        {L'Z', 1},

        {L'Ą', 5},
        {L'Ć', 6},
        {L'Ę', 5},
        {L'Ł', 3},
        {L'Ń', 7},
        {L'Ó', 5},
        {L'Ś', 5},
        {L'Ź', 9},
        {L'Ż', 5},

        {SCRABBLE_BLANK, 0},
    };

#ifdef TESTING
    wchar_t alphabet[] = L"AĄBCĆDEĘFGHIJKLŁMNŃOÓPRSŚTUWYZŹŻ";
    for (wchar_t wc: alphabet) {
        auto it = letterValue.find(wc);
        if (it == letterValue.end() and wc != 0)
            wprintf(L"Game::generateLetterValues(): Not found %lc\n", wc);
    }
#endif
}

void Game::paintTiles()
{
    for (unsigned i = 0; i < SCRABBLE_BOARD_SIZE; i++) {
        for (unsigned j = 0; j < SCRABBLE_BOARD_SIZE; j++) {
            CCOLOR textColor = GREY;
            CCOLOR bgColor = BLACK;
            // Set background color if tile is a bonus tile
            if (tiles[i][j].bonus == BONUS_DOUBLE_LETTER)
                bgColor = BLUE;
            else if (tiles[i][j].bonus == BONUS_TRIPLE_LETTER)
                bgColor = DARK_BLUE;
            else if (tiles[i][j].bonus == BONUS_DOUBLE_WORD)
                bgColor = RED;
            else if (tiles[i][j].bonus == BONUS_TRIPLE_WORD)
                bgColor = DARK_RED;

            // Set font color based on modified flag
            if (!tiles[i][j].empty()) {
                textColor = BLACK;
                bgColor = GREY;
            }
            if (tiles[i][j].modified)
                textColor = TEAL;

            // Apply colors to widget letters
            boardWidget.setLetter(int(i), int(j), Letter(tiles[i][j].get(),
                                               ConsoleColor(textColor, bgColor)));
        }
    }
}

void Game::resetTickVars()
{
    commited = false;
}

void Game::commitTiles()
{
    commited = true;
    if (removeMode) {
        currentPlayer->discardLetters(&letterBag);
        currentPlayer->takeLetters(&letterBag, MAX_LETTERS_ON_HAND);
        return;
    }
    currentPlayer->removeUsedLetters();
    currentPlayer->takeLetters(&letterBag, MAX_LETTERS_ON_HAND);

    // TODO: count scores
    countScore();
    currentPlayer->addScore(scoreDelta);

    for (unsigned i = 0; i < SCRABBLE_BOARD_SIZE; i++)
        for (unsigned j = 0; j < SCRABBLE_BOARD_SIZE; j++)
            tiles[i][j].commit(); // It is safe to commit already commited tiles
}

void Game::countScore()
{
    scoreDelta = 0;
    int placedTiles = 0;
    for (unsigned i = 0; i < SCRABBLE_BOARD_SIZE; i++) {
        int score = 0;
        int ok = 0;
        int count = 0;
        int wordMult = 1;
        for (unsigned j = 0; j < SCRABBLE_BOARD_SIZE; j++) {
            if (!tiles[i][j].empty()) {
                uint16 bonus = tiles[i][j].getBonus();
                int letterMult = 1;
                if (tiles[i][j].modified) {
                    ok += 1;
                    placedTiles += 1;
                    if (bonus == BONUS_DOUBLE_WORD)
                        wordMult *= 2;
                    else if (bonus == BONUS_TRIPLE_WORD)
                        wordMult *= 3;
                    else if (bonus == BONUS_DOUBLE_LETTER)
                        letterMult = 2;
                    else if (bonus == BONUS_TRIPLE_LETTER)
                        letterMult = 3;
                }
                score += letterValue[tiles[i][j].get()] * letterMult;
                count++;
            } else {
                if (ok and count > 1)
                    scoreDelta += wordMult*score;
                score = 0;
                count = 0;
                ok = 0;
                wordMult = 1;
            }
        }
    }
    // Same as above but the axes are swapped
    placedTiles = 0;
    for (unsigned j = 0; j < SCRABBLE_BOARD_SIZE; j++) {
        int score = 0;
        int ok = 0;
        int count = 0;
        int wordMult = 1;
        for (unsigned i = 0; i < SCRABBLE_BOARD_SIZE; i++) {
            if (!tiles[i][j].empty()) {
                uint16 bonus = tiles[i][j].getBonus();
                int letterMult = 1;
                if (tiles[i][j].modified) {
                    ok += 1;
                    placedTiles += 1;
                    if (bonus == BONUS_DOUBLE_WORD)
                        wordMult *= 2;
                    else if (bonus == BONUS_TRIPLE_WORD)
                        wordMult *= 3;
                    else if (bonus == BONUS_DOUBLE_LETTER)
                        letterMult = 2;
                    else if (bonus == BONUS_TRIPLE_LETTER)
                        letterMult = 3;
                }
                score += letterValue[tiles[i][j].get()] * letterMult;
                count++;
            } else {
                if (ok and count > 1)
                    scoreDelta += wordMult*score;
                score = 0;
                count = 0;
                ok = 0;
                wordMult = 1;
            }
        }
    }
    if (placedTiles == MAX_LETTERS_ON_HAND) {
        scoreDelta += 50;
    }
}

void Game::placeLetter(wchar_t ch)
{
    if (removeMode) {
        currentPlayer->markLetterAsDiscarded(ch);
    } else {
        placeTile(ch, unsigned(cursor.getX()), unsigned(cursor.getY()));
    }
}

void Game::placeTile(wchar_t ch, unsigned x, unsigned y)
{
    if (tiles[y][x].modified)
        undoTile(x, y);
    if (!tiles[y][x].empty())
        return;
    if (currentPlayer->hasLetter(ch)) {
        tiles[y][x].set(ch);
        currentPlayer->markLetterAsUsed(ch);
        countScore();
    }
}

void Game::undoTile()
{
    undoTile(unsigned(cursor.getX()), unsigned(cursor.getY()));
}

void Game::undoTile(unsigned x, unsigned y)
{
    if (tiles[y][x].modified) {
        currentPlayer->markLetterAsUnused(tiles[y][x].get());
        tiles[y][x].undo();
        countScore();
    }
}

void Game::undoAll()
{
    if (removeMode) {
        currentPlayer->keepLetters();
        return;
    }
    for (unsigned i = 0; i < SCRABBLE_BOARD_SIZE; i++) {
        for (unsigned j = 0; j < SCRABBLE_BOARD_SIZE; j++) {
            undoTile(j, i);
        }
    }
}

void Game::paintScoreDelta()
{
    wchar_t display[4] = {};
    ConsoleColor color(PINK, DARK_PINK);
    int sdcp = scoreDelta;
    display[2] = *digitToWStr(sdcp%10);

    sdcp /= 10;
    if (sdcp)
        display[1] = *digitToWStr(sdcp%10);
    else
        display[1] = L':';

    sdcp /= 10;
    if (sdcp)
        display[0] = *digitToWStr(sdcp%10);
    else
        display[0] = L':';
    boardWidget.setString(17,8,display,color,true);
}

void Game::nextPlayer()
{
    removeMode = 0;
    currentPlayer->deactivate();
    ++currentPlayer;
    if (currentPlayer == players.end())
        currentPlayer = players.begin();
    currentPlayer->activate();
    scoreDelta = 0;
}

void Game::updateLettersWidget()
{
    int lc = int(currentPlayer->letterCount());
    for (int i = 0; i < lc; i++) {
        auto lState = (*currentPlayer)[unsigned(i)];
        wchar_t letter = lState.first;
        if (lState.second == LETTER_USED)
            lettersWidget.setLetter(1, 2*i, letter, ConsoleColor(TEAL,GREY));
        else if (lState.second == LETTER_DISCARDED)
            lettersWidget.setLetter(1, 2*i, letter, ConsoleColor(RED,GREY));
        else
            lettersWidget.setLetter(1, 2*i, letter, ConsoleColor(BLACK,GREY));
        lettersWidget.setString(2, 2*i, digitToWStr(unsigned(letterValue[lState.first])),
                                                    ConsoleColor(DARK_GREY,GREY));
    }
}

void Game::updateScoreboard()
{
    const CCOLOR activeColor = TEAL;
    const CCOLOR inactiveColor = GREY;
    auto it = players.begin();
    for (int i = 2; it != players.end(); ++it, i+=2) {
        CCOLOR pColor;
        wchar_t buff[8] = {}; // score string buffer
        swprintf(buff, L"%d", it->getScore());
        if (it->isActive())
            pColor = activeColor;
        else
            pColor = inactiveColor;
        int xpos = scoresWidget.canvasWidth() - wstrlen(buff);
        scoresWidget.setString(i, 0, it->name(), ConsoleColor(pColor,DARK_TEAL));
        scoresWidget.setString(i, xpos, buff, ConsoleColor(WHITE,DARK_TEAL));
    }
}
