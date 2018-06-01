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
    altDown(false), end(false)
{}

void Game::run()
{
    // setup
    generateLetterValues();
    letterBag.loadPolishScrabble();
    setupBoard();
    setupWidgets();
    bindEvents();
    // Start event manager
    EventManager::start();

    // control cursor visibility manually
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hStdOut, &cci);
    cci.bVisible = false;
    SetConsoleCursorInfo(hStdOut, &cci);
    boardWidget.toggleCursor();

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
        paintTiles();

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
    else if (key == VK_BACK)
        undoTile();
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
            placeLetter(key);
        }
    }
}

void Game::keyReleaseEvent(KeyEvent *e)
{
    if (e->key() == VK_RMENU)
        altDown = false;
}

void Game::bindEvents()
{
    EventManager::connect(KeyPress, [this](Event* e) {
        keyPressEvent(dynamic_cast<KeyEvent*>(e));
    });
    EventManager::connect(KeyRelease, [this](Event* e) {
        keyReleaseEvent(dynamic_cast<KeyEvent*>(e));
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
    legendWidget.setString(0,0,L"LEGEND:", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setString(1,0,L"========", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setLetter(2,0,Letter(' ', ConsoleColor(BLACK, DARK_RED)));
    legendWidget.setString(2,1,L"-triple\n word", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setString(4,0,L"--------", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setLetter(5,0,Letter(' ', ConsoleColor(BLACK, RED)));
    legendWidget.setString(5,1,L"-double\n word", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setString(7,0,L"--------", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setLetter(8,0,Letter(' ', ConsoleColor(BLACK, DARK_BLUE)));
    legendWidget.setString(8,1,L"-triple\n letter", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setString(10,0,L"--------", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setLetter(11,0,Letter(' ', ConsoleColor(BLACK, BLUE)));
    legendWidget.setString(11,1,L"-double\n letter", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setString(13,0,L"--------", ConsoleColor(WHITE, DARK_TEAL));
    legendWidget.setLetter(14,0,Letter(' ', ConsoleColor(BLACK, GREY)));
    legendWidget.setString(14,1,L"-cursor", ConsoleColor(WHITE, DARK_TEAL));

    scoresWidget.resize(14,11);
    scoresWidget.move(35,0);
    scoresWidget.setBorder(Letter('#', ConsoleColor(GREEN, DARK_GREEN)), 1);
    scoresWidget.setBackgroundColor(DARK_TEAL);
    scoresWidget.setString(0,3,L"SCORES:", ConsoleColor(WHITE, DARK_TEAL));
    scoresWidget.setString(1,0,L"============", ConsoleColor(WHITE, DARK_TEAL));
    scoresWidget.setString(3,0,L"============", ConsoleColor(WHITE, DARK_TEAL));
    scoresWidget.setString(5,0,L"============", ConsoleColor(WHITE, DARK_TEAL));
    scoresWidget.setString(7,0,L"============", ConsoleColor(WHITE, DARK_TEAL));
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
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
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

        {L' ', 0}, // placeholder for blank
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
    for (int i = 0; i < SCRABBLE_BOARD_SIZE; i++) {
        for (int j = 0; j < SCRABBLE_BOARD_SIZE; j++) {
            CCOLOR textColor = WHITE;
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
            if (tiles[i][j].modified)
                textColor = TEAL;

            // Apply colors to widget letters
            boardWidget.setLetter(i, j, Letter(tiles[i][j].get(),
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
    // TODO: count scores

    for (int i = 0; i < SCRABBLE_BOARD_SIZE; i++)
        for (int j = 0; j < SCRABBLE_BOARD_SIZE; j++)
            tiles[i][j].commit(); // It is safe to commit already commited tiles
}

void Game::placeLetter(wchar_t ch)
{
    placeTile(ch, cursor.getX(), cursor.getY());
}

void Game::placeTile(wchar_t ch, int x, int y)
{
    tiles[y][x].set(ch);
}

void Game::undoTile()
{
    tiles[cursor.getY()][cursor.getX()].undo();
}

void Game::nextPlayer()
{
    currentPlayer->deactivate();
    ++currentPlayer;
    if (currentPlayer == players.end())
        currentPlayer = players.begin();
    currentPlayer->activate();
}

void Game::updateScoreboard()
{
    const CCOLOR activeColor = TEAL;
    const CCOLOR inactiveColor = GREY;
    auto it = players.begin();
    for (int i = 2; it != players.end(); ++it, i+=2) {
        CCOLOR pColor;
        wchar_t buff[8] = {};
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
