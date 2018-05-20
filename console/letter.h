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
    ConsoleColor color() const;
    void display(HANDLE outputHandle) const;
private:
    wchar_t c;
    ConsoleColor col;
};

#endif // LETTER_H
