#ifndef CONSOLECOLOR_H
#define CONSOLECOLOR_H
#include <Windows.h>

enum CCOLOR {
    BLACK       = 0x0,
    DARK_BLUE   = 0x1,
    DARK_GREEN  = 0x2,
    DARK_TEAL   = 0x3,
    DARK_RED    = 0x4,
    DARK_PINK   = 0x5,
    DARK_YELLOW = 0x6,
    GREY        = 0x7,
    DARK_GREY   = 0x8,
    BLUE        = 0x9,
    GREEN       = 0xA,
    TEAL        = 0xB,
    RED         = 0xC,
    PINK        = 0xD,
    YELLOW      = 0xE,
    WHITE       = 0xF,
};

class ConsoleColor
{
public:
    ConsoleColor(CCOLOR foreground = GREY, CCOLOR background = BLACK); // default terminal colors

    void swapColors(); // swap background and foreground colors
    void setBackground(CCOLOR c) {bg = c;}
    void setForeground(CCOLOR c) {fg = c;}

    operator SHORT() const;
private:
    CCOLOR fg, bg;
};

#endif // CONSOLECOLOR_H
