#include <cstdio>
#include <Windows.h>

#include "console/widget.h"
#include "game/game.h"
using namespace std;

int main()
{
    int dbg = sizeof(Game);
    SetConsoleTitleW(L"Scrabble");
    Game* g = new Game();
    g->run();
    return 0;
}
