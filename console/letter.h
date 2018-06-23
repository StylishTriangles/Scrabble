#ifndef LETTER_H
#define LETTER_H
#include "common.h"
#include "consolecolor.h"

class Letter
{
public:
    Letter() = default;
    Letter(char c, ConsoleColor col = ConsoleColor());
    Letter(wchar_t c, ConsoleColor col = ConsoleColor());

    wchar_t character() const;
    ConsoleColor const& color() const {return col;}
    ConsoleColor& color() {return col;}
    void setColor(ConsoleColor color) {col = color;}
    void display(HANDLE outputHandle) const;
private:
    wchar_t c;
    ConsoleColor col;
};

#endif // LETTER_H
