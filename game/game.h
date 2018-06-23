#ifndef GAME_H
#define GAME_H
#include <list>
#include <map>
#include <windows.h>

#include "common.h"
#include "console/consolecolor.h"
#include "console/widget.h"
#include "events/keyevent.h"
#include "game/letterbag.h"
#include "game/player.h"
#include "game/tile.h"

#define SCRABBLE_BOARD_SIZE 15 // H == W
#define MAX_PLAYER_NAME_LEN 9
#define MAX_LETTERS_ON_HAND 7
#define SCRABBLE_BLANK (L'░') // Don't change this line

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
    void disableSeizureMode();

    // game board control
    void commitTiles();
    void countScore();
    void placeLetter(wchar_t ch);
    void placeTile(wchar_t ch, unsigned x, unsigned y);
    void undoTile();
    void undoTile(unsigned x, unsigned y);
    void undoAll();
    void paintScoreDelta();

    // player control
    void nextPlayer(); // set next player as active
    void updateLettersWidget();

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
    int scoreDelta;
    bool altDown;
    bool end;
    bool commited;
    bool removeMode;
};

#endif // GAME_H
