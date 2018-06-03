#include "letterbag.h"
#include <algorithm>
#include <ctime>

LetterBag::LetterBag(std::initializer_list<std::pair<wchar_t, int> > init) :
    rng(time(NULL))
{
    constructBag(init);
    shuffle();
}

void LetterBag::shuffle()
{
    auto rf = [this](int lim) {
        return rng()%lim;
    };
    std::random_shuffle(bag.begin(), bag.end(), rf);
}

void LetterBag::constructBag(const std::initializer_list<std::pair<wchar_t, int> > &init)
{
    bag.clear();
    for (auto pwi: init) {
        while (pwi.second--) {
            bag.push_back(pwi.first);
        }
    }
}

wchar_t LetterBag::pop()
{
    wchar_t ret = bag.back();
    bag.pop_back();
    return ret;
}

void LetterBag::insert(wchar_t letter)
{
    int pos = rng() % bag.size();
    bag.insert(bag.begin()+pos, letter);
}

void LetterBag::loadPolishScrabble()
{
    constructBag({
    //  LETTER, COUNT
        {L'A', 9},
        {L'B', 2},
        {L'C', 3},
        {L'D', 3},
        {L'E', 7},
        {L'F', 1},
        {L'G', 2},
        {L'H', 2},
        {L'I', 8},
        {L'J', 2},
        {L'K', 3},
        {L'L', 3},
        {L'M', 3},
        {L'N', 5},
        {L'O', 6},
        {L'P', 3},
        {L'R', 4},
        {L'S', 4},
        {L'T', 3},
        {L'U', 2},
        {L'W', 4},
        {L'Y', 4},
        {L'Z', 5},

        {L'Ą', 1},
        {L'Ć', 1},
        {L'Ę', 1},
        {L'Ł', 2},
        {L'Ń', 1},
        {L'Ó', 1},
        {L'Ś', 1},
        {L'Ź', 1},
        {L'Ż', 1},

        {L'░', 2}, // placeholder for blank
    });
    shuffle();
}
