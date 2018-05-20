#include "letter.h"
#include <cstdio>

Letter::Letter(char c, ConsoleColor col) :
    c(c), col(col)
{

}

Letter::Letter(wchar_t c, ConsoleColor col) :
    c(c), col(col)
{

}

ConsoleColor Letter::color() const
{
    return col;
}

void Letter::display(HANDLE outputHandle) const
{
    SetConsoleTextAttribute(outputHandle, col);
    DWORD throwaway;
    WriteConsoleW(outputHandle, &c, 1, &throwaway, NULL);
}

wchar_t Letter::character() const
{
    return c;
}
