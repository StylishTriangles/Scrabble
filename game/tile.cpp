#include "tile.h"

/**
 *  @brief Default constructor for Tile.
 **/
Tile::Tile() :
    modified(false), bonus(BONUS_NONE),
    letter(L' '), backup(letter)
{

}

/**
 *  @brief Set character representing this Tile.
 *	@param ch: Character to represent this Tile object.
 **/
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
