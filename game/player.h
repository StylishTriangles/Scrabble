#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "letterbag.h"

class Player
{
public:
    Player(const wchar_t* name = L"Player", bool active = false);
    void getLetters(wchar_t *buffer) const;
    bool hasLetter(wchar_t l) const;
    void insertLetter(wchar_t l);
    void removeLetter(wchar_t l);
    void takeLetters(LetterBag* bag, int limit); // player will take as much letters as possible withoud exeeding limit
    bool markLetterAsUsed(wchar_t letter);  // returns true if letter was successfully marked
    bool markLetterAsUnused(wchar_t letter);// returns true if letter was successfully unmarked
    void removeUsedLetters();
    int letterCount() const;
    void addScore(int amount) {score += amount;}
    const int& getScore() const {return score;}
    bool isActive() const {return active;}
    void activate() {active = true;}
    void deactivate() {active = false;}

    const wchar_t* name() const {return myName;}
    void name(const wchar_t* name) {myName = name;} // wordplay setter

    const std::pair<wchar_t, bool> &operator [] (int letterNumber) const; // make it easy to iterate over letters

private:
    std::vector< std::pair<wchar_t,bool> > ownedLetters;
    const wchar_t *myName;
    bool active;
    int score;
};

#endif // PLAYER_H
