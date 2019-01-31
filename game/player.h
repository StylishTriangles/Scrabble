#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "letterbag.h"

#define LETTER_USED        0x1
#define LETTER_DISCARDED   0x2

class Player
{
public:
    Player(const wchar_t* name = L"Player", bool active = false);
    void getLetters(wchar_t *buffer) const;
    bool hasLetter(wchar_t l) const;
    void insertLetter(wchar_t l);
    void removeLetter(wchar_t l);
    void takeLetters(LetterBag* bag, unsigned limit); // player will take as much letters as possible withoud exeeding limit
    bool markLetterAsUsed(wchar_t letter);  // returns true if letter was successfully marked
    bool markLetterAsUnused(wchar_t letter);// returns true if letter was successfully unmarked
    void removeUsedLetters();
    bool markLetterAsDiscarded(wchar_t letter);
    bool markLetterAsKept(wchar_t letter);
    void discardLetters(LetterBag* bag); // discard marked letters and put them into bag
    void keepLetters(); // don't discard letters
    unsigned letterCount() const;
    void addScore(int amount) {score += amount;} /*!< Increase score by specified amount. */
    const int& getScore() const {return score;} /*!< Return Player's score. */
    bool isActive() const {return active;} /*!< Return true if player is playing his turn. */
    void activate() {active = true;} /*!< Player starts playing his turn. */
    void deactivate() {active = false;} /*!< Player stops playing his turn. */

    const wchar_t* name() const {return myName;} /*!< Return Player's name. */
    void name(const wchar_t* name) {myName = name;} /*!< Set Player's name. */

    const std::pair<wchar_t, int> &operator [] (unsigned letterNumber) const; // make it easy to iterate over letters

private:
    std::vector< std::pair<wchar_t,int> > ownedLetters;
    const wchar_t *myName;
    bool active;
    int score;
};

#endif // PLAYER_H
