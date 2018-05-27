#include "game.h"
#define TESTING
#ifdef TESTING
#include <cstdio>
#endif

Game::Game()
{
    generateLetterValues();
}

void Game::run()
{
    while(true) {

    }
}

void Game::generateLetterValues()
{
//    letterValue = {
//        {L'A', 1},
//        {L'E', 1},
//        {L'I', 1},
//        {L'N', 1},
//        {L'O', 1},
//        {L'R', 1},
//        {L'S', 1},
//        {L'W', 1},
//        {L'Z', 1},

//        {L'C', 2},
//        {L'D', 2},
//        {L'K', 2},
//        {L'L', 2},
//        {L'M', 2},
//        {L'P', 2},
//        {L'T', 2},
//        {L'Y', 2},

//        {L'B', 3},
//        {L'G', 3},
//        {L'H', 3},
//        {L'J', 3},
//        {L'Ł', 3},
//        {L'U', 3},

//        {L'Ą', 5},
//        {L'Ę', 5},
//        {L'F', 5},
//        {L'Ó', 5},
//        {L'Ś', 5},
//        {L'Ż', 5},

//        {L'Ć', 6},

//        {L'Ń', 7},

//        {L'Ź', 9},

//        {L' ', 0}, // placeholder for blank
//    };
    letterValue = {
        // reorganized
        {L'A', 1},
        {L'B', 3},
        {L'C', 2},
        {L'D', 2},
        {L'E', 1},
        {L'F', 5},
        {L'G', 3},
        {L'H', 3},
        {L'I', 1},
        {L'J', 3},
        {L'K', 2},
        {L'L', 2},
        {L'M', 2},
        {L'N', 1},
        {L'O', 1},
        {L'P', 2},
        {L'R', 1},
        {L'S', 1},
        {L'T', 2},
        {L'U', 3},
        {L'W', 1},
        {L'Y', 2},
        {L'Z', 1},

        {L'Ą', 5},
        {L'Ć', 6},
        {L'Ę', 5},
        {L'Ł', 3},
        {L'Ń', 7},
        {L'Ó', 5},
        {L'Ś', 5},
        {L'Ź', 9},
        {L'Ż', 5},

        {L' ', 0}, // placeholder for blank
    };

#ifdef TESTING
    wchar_t alphabet[] = L"AĄBCĆDEĘFGHIJKLŁMNŃOÓPRSŚTUWYZŹŻ";
    for (wchar_t wc: alphabet) {
        auto it = letterValue.find(wc);
        if (it == letterValue.end() and wc != 0)
            wprintf(L"Game::generateLetterValues(): Not found %lc\n", wc);
    }
#endif
}
