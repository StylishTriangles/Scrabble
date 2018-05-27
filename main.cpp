#include <iostream>
#include <Windows.h>

#include "console/widget.h"
#define TESTING
#include "game/game.h"
using namespace std;

int main()
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Widget w;
    w.resize(19,19);
    w.setBorder(Letter('#', ConsoleColor(GREEN, DARK_GREEN)), 2);
    w.setLetter(2,2,Letter(L'a', ConsoleColor(RED)));
    w.setLetter(2,3,'b');
    w.setLetter(3,2,'c');
    w.setLetter(3,3,'d');
    w.display(hStdOut);

    Game g;
    return 0;
}
