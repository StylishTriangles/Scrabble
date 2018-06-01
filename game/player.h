#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

class Player
{
public:
    Player(const wchar_t* name = L"Player", bool active = false);
    void getLetters(wchar_t *buffer) const;
    bool hasLetter(wchar_t l) const;
    void insertLetter(wchar_t l);
    void removeLetter(wchar_t l);
    int letterCount() const;
    void addScore(int amount) {score += amount;}
    const int& getScore() const {return score;}
    bool isActive() const {return active;}
    void activate() {active = true;}
    void deactivate() {active = false;}

    const wchar_t* name() const {return myName;}
    void name(const wchar_t* name) {myName = name;} // wordplay setter

private:
    std::vector<wchar_t> ownedLetters;
    const wchar_t *myName;
    bool active;
    int score;
};

#endif // PLAYER_H
