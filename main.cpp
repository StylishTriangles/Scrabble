#include <iostream>
#include <Windows.h>

#include "console/widget.h"
#define TESTING
#include "game/game.h"
using namespace std;

int main()
{
    SetConsoleTitleW(L"Scrabble");
    Game g;
    g.run();
    return 0;
}
