#include "tile.h"

Tile::Tile() :
    modified(false), bonus(BONUS_NONE),
    letter(L' '), backup(letter)
{

}

void Tile::set(wchar_t ch)
{
    if (ch != letter) {
        if (!modified) {
            backup = letter;
            modified = true;
        }
        letter = ch;
    }
}
