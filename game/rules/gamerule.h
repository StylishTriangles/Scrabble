#ifndef GAMERULE_H
#define GAMERULE_H

/**
 * The GameRule base class.
 * GameRules are crafted specifically for this instance of Game class.
 * They check rules without using any layer of abstraction but by using the current implementation of Game.
 */
class GameRule
{
public:
    GameRule();
    virtual ~GameRule() = default;
    virtual execute(Game*) = 0;
};

#endif // GAMERULE_H
