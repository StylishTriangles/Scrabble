#include "player.h"

Player::Player(const wchar_t *name, bool active) :
    myName(name),
    active(active),
    score(0)
{

}

void Player::getLetters(wchar_t* buffer) const
{
    for (auto &wc: ownedLetters)
        *buffer++ = wc.first;
}

bool Player::hasLetter(wchar_t l) const
{
    for (auto &wc: ownedLetters) {
        if (wc.first == l and wc.second == false) {
            return true;
        }
    }
    return false;
}

void Player::insertLetter(wchar_t l)
{
    ownedLetters.push_back({l,0});
}

void Player::removeLetter(wchar_t l)
{
    auto it = ownedLetters.begin();
    while (it != ownedLetters.end()) {
        if (it->first == l) {
            ownedLetters.erase(it);
            return;
        }
        ++it;
    }
}

void Player::takeLetters(LetterBag *bag, unsigned limit)
{
    while (!bag->empty() and letterCount() < limit)
        insertLetter(bag->pop());
}

bool Player::markLetterAsUsed(wchar_t letter)
{
    for (auto &wc: ownedLetters) {
        if (wc.first == letter and !(wc.second & LETTER_USED)) {
            wc.second |= LETTER_USED;
            return true;
        }
    }
    return false;
}

bool Player::markLetterAsUnused(wchar_t letter)
{
    for (auto it = ownedLetters.rbegin(); it != ownedLetters.rend(); ++it) {
        if (it->first == letter and (it->second & LETTER_USED)) {
            it->second ^= LETTER_USED;
            return true;
        }
    }
    return false;
}

void Player::removeUsedLetters()
{
    if (ownedLetters.cbegin() == ownedLetters.cend())
        return;
    auto it = ownedLetters.cend();
    do {
        --it;
        if (it->second & LETTER_USED)
            ownedLetters.erase(it);
    } while (it != ownedLetters.cbegin());
}

bool Player::markLetterAsDiscarded(wchar_t letter)
{
    for (auto &wc: ownedLetters) {
        if (wc.first == letter and !(wc.second & LETTER_DISCARDED)) {
            wc.second |= LETTER_DISCARDED;
            return true;
        }
    }
    return false;
}

bool Player::markLetterAsKept(wchar_t letter)
{
    for (auto it = ownedLetters.rbegin(); it != ownedLetters.rend(); ++it) {
        if (it->first == letter and (it->second & LETTER_DISCARDED)) {
            it->second ^= LETTER_DISCARDED;
            return true;
        }
    }
    return false;
}

void Player::discardLetters(LetterBag *bag)
{
    if (ownedLetters.cbegin() == ownedLetters.cend())
        return;
    auto it = ownedLetters.cend();
    do {
        --it;
        if (it->second & LETTER_DISCARDED) {
            bag->insert(it->first);
            ownedLetters.erase(it);
        }
    } while (it != ownedLetters.cbegin());
}

void Player::keepLetters()
{
    for (unsigned i = 0; i < letterCount(); i++) {
        if (ownedLetters[i].second & LETTER_DISCARDED) {
            ownedLetters[i].second ^= LETTER_DISCARDED;
        }
    }
}

unsigned Player::letterCount() const
{
    return ownedLetters.size();
}

const std::pair<wchar_t, int> &Player::operator [](unsigned letterNumber) const
{
    return ownedLetters[letterNumber];
}
