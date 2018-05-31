#include "game.h"

#define TESTING
#ifdef TESTING
#include <cstdio>
#endif

#include <thread>

#include "events/eventmanager.h"

Game::Game() :
    hStdOut(GetStdHandle(STD_OUTPUT_HANDLE)),
    altDown(false), end(false)
{
    generateLetterValues();

    letterBag.loadPolishScrabble();

    setupBoard();

    setupWidgets();

    bindEvents();
    // Start event manager
    EventManager::start();
}

void Game::run()
{

    // control cursor visibility manually
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hStdOut, &cci);
    cci.bVisible = false;
    SetConsoleCursorInfo(hStdOut, &cci);
    boardWidget.toggleCursor();

    while(!end) {
        // Input
        EventManager::pollEvents();

        // Display
        paintTiles();
        repaint();

        // Wait
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Game::repaint()
{
    boardWidget.display(hStdOut);
    //legendWidget.display(hStdOut);
}

void Game::setLanguage(Game::Language l)
{
    lang = l;
}

void Game::keyPressEvent(KeyEvent *e)
{
    const short &key = e->key();
    if (e->key() == VK_RMENU)
        altDown = true;
    else if (e->key() == VK_LEFT)
        cursor.left();
    else if (e->key() == VK_RIGHT)
        cursor.right();
    else if (e->key() == VK_UP)
        cursor.up();
    else if (e->key() == VK_DOWN)
        cursor.down();

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
    legendWidget.move(25,0);
    legendWidget.setBorder(Letter(' ', ConsoleColor(GREY, DARK_PINK)), 1);
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
            if (tiles[i][j].modified)
                textColor = TEAL;

            // Apply colors to widget letters
            boardWidget.setLetter(i, j, Letter(tiles[i][j].get(),
                                               ConsoleColor(textColor, bgColor)));
        }
    }
}

void Game::placeLetter(wchar_t ch)
{
    placeTile(ch, cursor.getX(), cursor.getY());
}

void Game::placeTile(wchar_t ch, int x, int y)
{
    tiles[y][x].set(ch);
}
