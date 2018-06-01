#include "player.h"

Player::Player(const wchar_t *name, bool active) :
    myName(name),
    active(active),
    score(0)
{

}

void Player::getLetters(wchar_t* buffer) const
{
    for (const wchar_t &wc: ownedLetters)
        *buffer++ = wc;
}

bool Player::hasLetter(wchar_t l) const
{
    for (const wchar_t& wc: ownedLetters) {
        if (wc == l) {
            return true;
        }
    }
    return false;
}

void Player::insertLetter(wchar_t l)
{
    ownedLetters.push_back(l);
}

void Player::removeLetter(wchar_t l)
{
    auto it = ownedLetters.begin();
    while (it != ownedLetters.end()) {
        if (*it == l) {
            ownedLetters.erase(it);
            return;
        }
        ++it;
    }
}

int Player::letterCount() const
{
    return ownedLetters.size();
}
