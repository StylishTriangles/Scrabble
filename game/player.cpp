#include "player.h"

/**
 *  @brief Create a new Player instance.
 *	@param name: Player's name.
 *	@param active: Flag used to track currently playing Player.
 **/
Player::Player(const wchar_t *name, bool active) :
    myName(name),
    active(active),
    score(0)
{

}

/**
 *  @brief Fetch all lettters from hand into buffer.
 **/
void Player::getLetters(wchar_t* buffer) const
{
    for (auto &wc: ownedLetters)
        *buffer++ = wc.first;
}

/**
 *  @brief Check if player has specifed letter.
 *	@param l: Character to search for.
 **/
bool Player::hasLetter(wchar_t l) const
{
    for (auto &wc: ownedLetters) {
        if (wc.first == l and wc.second == false) {
            return true;
        }
    }
    return false;
}

/**
 *  @brief Insert new letter into Player's hand.
 *	@param l: letter to insert.
 **/
void Player::insertLetter(wchar_t l)
{
    ownedLetters.push_back({l,0});
}

/**
 *  @brief Remove letter from hand.
 *	@param l: Letter to remove.
 **/
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

/**
 *  @brief Take letters from bag.
 *	@param bag: Bag to take letters from.
 *	@param limit: Maximum number of letters player can have on hand.
 **/
void Player::takeLetters(LetterBag *bag, unsigned limit)
{
    while (!bag->empty() and letterCount() < limit)
        insertLetter(bag->pop());
}

/**
 *  @brief Mark letter which was used this turn.
 *	@param letter: Mark this letter as used.
 *	@return true if letter was successfully marked.
 **/
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

/**
 *  @brief Unmark letter as used this turn.
 *	@param letter: Unmark this letter as used.
 *	@return true if letter was successfully unmarked.
 **/
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

/**
 *  @brief Delete used letters from hand.
 **/
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

/**
 *  @brief Mark letter which player wants to discard.
 *	@param letter: Mark this letter as discarded.
 *	@return true if letter was successfully marked.
 **/
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

/**
 *  @brief Undo marking letter as discarded.
 *	@param letter: Undo marking this letter as discarded.
 *	@return true if letter was successfully unmarked.
 **/
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

/**
 *  @brief Put unwanted letters back into the bag.
 *	@param bag: Bag to put letters in.
 **/
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

/**
 *  @brief Undo marking all letters as discarded.
 **/
void Player::keepLetters()
{
    for (unsigned i = 0; i < letterCount(); i++) {
        if (ownedLetters[i].second & LETTER_DISCARDED) {
            ownedLetters[i].second ^= LETTER_DISCARDED;
        }
    }
}

/**
 *  @brief Return current amount of letters on hand.
 **/
unsigned Player::letterCount() const
{
    return ownedLetters.size();
}

/**
 *  @brief Directly access letters vector.
 *	@param letterNumber: Position in 0-indexed array.
 *	@return Pair containing character on specified position and character's flags.
 **/
const std::pair<wchar_t, int> &Player::operator [](unsigned letterNumber) const
{
    return ownedLetters[letterNumber];
}
