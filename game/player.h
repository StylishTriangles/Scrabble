#ifndef PLAYER_H
#define PLAYER_H

#include <list>

class Player
{
public:
    Player(const wchar_t* name = L"Player", bool active = false);
    void getLetters(wchar_t *buffer) const;
    bool hasLetter(wchar_t l) const;
    void insertLetter(wchar_t l);
    void removeLetter(wchar_t l);
    int letterCount() const;

    const wchar_t* name() const {return myName;}
    void name(const wchar_t* name) {myName = name;} // wordplay setter

private:
    std::list<wchar_t> ownedLetters;
    const wchar_t *myName;
    bool active;
};

#endif // PLAYER_H
