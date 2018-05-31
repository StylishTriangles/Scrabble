#ifndef LETTERBAG_H
#define LETTERBAG_H

#include <initializer_list>
#include <random>
#include <vector>

class LetterBag
{
public:
    LetterBag(std::initializer_list< std::pair<wchar_t, int> > init = {});

    bool empty() const {return bag.empty();}
    wchar_t popLetter();
    void insert(wchar_t letter);
    void loadPolishScrabble();
private:
    // all shuffle related tasks are performed automatically
    // therefore user can't shuffle manually
    void shuffle();
    void constructBag(const std::initializer_list<std::pair<wchar_t, int> > &init);

    std::vector<wchar_t> bag;
    std::mt19937 rng;
};

#endif // LETTERBAG_H
