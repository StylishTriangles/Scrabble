#ifndef TILE_H
#define TILE_H

#include "common.h"
#include "console/letter.h"

#define BONUS_NONE 0
#define BONUS_DOUBLE_WORD 1
#define BONUS_TRIPLE_WORD 2
#define BONUS_DOUBLE_LETTER 3
#define BONUS_TRIPLE_LETTER 4

class Tile
{
public:
    Tile();
    bool empty() {return (letter != L' ') & (letter != 0);}
    bool isBonus() {return bonus != BONUS_NONE;}
    void set(wchar_t ch);
    void setBonus(uint16 bonusType) {bonus = bonusType;}
    void commit() {modified = false; backup = letter;} // Remove modified flag
    void undo() {modified = false; letter = backup;}

    wchar_t get() const {return letter;}
    uint16 getBonus() const {return bonus;}

    bool modified; // true if the tile is being modified this turn
    uint16 bonus;
private:
    wchar_t letter;
    wchar_t backup;
};

#endif // TILE_H
