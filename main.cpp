#include <cstdio>
#include <Windows.h>

#include "console/widget.h"
#include "game/game.h"
using namespace std;

int main()
{
    SetConsoleTitleW(L"Scrabble");
    Game* g = new Game();
    g->addPlayer(L"Łukasz");
    g->addPlayer(L"Ptak");
    g->run();
    return 0;
}
