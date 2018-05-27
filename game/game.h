#ifndef GAME_H
#define GAME_H
#include <map>

#include "console/widget.h"

class Game
{
public:
    Game();
    void run();

private:
    void generateLetterValues();

    Widget boardWidget;
    std::map<wchar_t, int> letterValue;
};

#endif // GAME_H
