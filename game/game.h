#ifndef GAME_H
#define GAME_H
#include <map>
#include <Windows.h>

#include "common.h"
#include "console/consolecolor.h"
#include "console/widget.h"
#include "game/letterbag.h"
#include "game/tile.h"

#define SCRABBLE_BOARD_SIZE 15 // H == W

class Game
{
public:
    Game();
    void run();
    void repaint();

private:
    void setupWidgets();
    void setupBoard();
    void generateLetterValues();
    void paintTiles(); // paint tiles on widget

    // game board control
    void placeTile(wchar_t ch, int x, int y);

    HANDLE hStdOut;
    Widget boardWidget, legendWidget;
    Cursor cursor;
    std::map<wchar_t, int> letterValue;
    LetterBag letterBag;
    std::vector< std::vector<Tile> > tiles;
};

#endif // GAME_H
