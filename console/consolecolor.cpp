#include "consolecolor.h"
#include <algorithm>

ConsoleColor::ConsoleColor(CCOLOR foreground, CCOLOR background) :
    fg(foreground), bg(background)
{

}

void ConsoleColor::swapColors()
{
    std::swap(fg, bg);
}

ConsoleColor::operator SHORT() const
{
    return (SHORT)((bg<<4)+fg);
}
