#include "player.h"

Player::Player(const wchar_t *name, bool active) :
    myName(name),
    active(active)
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
    ownedLetters.push_front(l);
}

void Player::removeLetter(wchar_t l)
{
    ownedLetters.remove(l);
}

int Player::letterCount() const
{
    return ownedLetters.size();
}
