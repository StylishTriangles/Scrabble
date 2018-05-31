#ifndef GAME_H
#define GAME_H
#include <map>
#include <Windows.h>

#include "common.h"
#include "console/consolecolor.h"
#include "console/widget.h"
#include "events/keyevent.h"
#include "game/letterbag.h"
#include "game/tile.h"

#define SCRABBLE_BOARD_SIZE 15 // H == W

class Game
{
public:
    enum Language {
        POLISH,
    };

    Game();
    void run();
    void repaint();
    void setLanguage(Language l);

    void keyPressEvent(KeyEvent* e);
    void keyReleaseEvent(KeyEvent* e);

private:
    void bindEvents();
    void setupWidgets();
    void setupBoard();
    void generateLetterValues();
    void paintTiles(); // paint tiles on widget

    // game board control
    void placeLetter(wchar_t ch);
    void placeTile(wchar_t ch, int x, int y);

    HANDLE hStdOut;
    Widget boardWidget, legendWidget, scoresWidget;
    Cursor cursor;
    std::map<wchar_t, int> letterValue;
    LetterBag letterBag;
    std::vector< std::vector<Tile> > tiles;
    Language lang;
    bool altDown;
    bool end;
};

#endif // GAME_H
