#ifndef GAME_H
#define GAME_H
#include <list>
#include <map>
#include <Windows.h>

#include "common.h"
#include "console/consolecolor.h"
#include "console/widget.h"
#include "events/keyevent.h"
#include "game/letterbag.h"
#include "game/player.h"
#include "game/tile.h"

#define SCRABBLE_BOARD_SIZE 15 // H == W
#define MAX_PLAYER_NAME_LEN 8

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
    void addPlayer(const wchar_t* name);

    void keyPressEvent(KeyEvent* e);
    void keyReleaseEvent(KeyEvent* e);

private:
    void bindEvents();
    void setupWidgets();
    void setupBoard();
    void generateLetterValues();
    void paintTiles(); // paint tiles on widget
    void resetTickVars();

    // game board control
    void commitTiles();
    void placeLetter(wchar_t ch);
    void placeTile(wchar_t ch, int x, int y);
    void undoTile();

    // player control
    void nextPlayer(); // set next player as active

    // scoreboard control
    void updateScoreboard();

    HANDLE hStdOut;
    Widget boardWidget, legendWidget, scoresWidget, lettersWidget;
    Cursor cursor;
    std::map<wchar_t, int> letterValue;
    LetterBag letterBag;
    std::vector< std::vector<Tile> > tiles;
    std::list< Player > players;
    std::list< Player >::iterator currentPlayer;
    Language lang;
    bool altDown;
    bool end;
    bool commited;
};

#endif // GAME_H
