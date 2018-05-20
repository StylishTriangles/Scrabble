#include <iostream>
#include <Windows.h>

#include "console/widget.h"

using namespace std;

int main()
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Widget w;
    w.resize(2,2);
    w.setLetter(0,0,Letter('a', ConsoleColor(RED)));
    w.setLetter(0,1,'b');
    w.setLetter(1,0,'c');
    w.setLetter(1,1,'d');
    w.move(10,10);
    w.display(hStdOut);
    return 0;
}
