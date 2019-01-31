#include "letter.h"
#include <cstdio>

/**
 *  @brief Construct Letter with ASCII character.
 *	@param c: Character to represent this Letter object.
 *	@param col: Character's color.
 **/
Letter::Letter(char c, ConsoleColor col) :
    c(wchar_t(c)), col(col)
{

}

/**
 *  @brief Construct Letter with a wide character.
 *	@param c: Wide character to represent this Letter object.
 *	@param col: Character's color.
 **/
Letter::Letter(wchar_t c, ConsoleColor col) :
    c(c), col(col)
{

}

/**
 *  @brief Display character to current cursor position.
 *	@param outputHandle: Handle to output display data to (usually standard output).
 **/
void Letter::display(HANDLE outputHandle) const
{
    SetConsoleTextAttribute(outputHandle, col);
//    putwc(c, stdout);
    DWORD throwaway;
    WriteConsoleW(outputHandle, &c, 1, &throwaway, nullptr);
}

/**
 *  @brief Return character representing this Letter.
 **/
wchar_t Letter::character() const
{
    return c;
}
