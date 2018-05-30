#ifndef CURSOR_H
#define CURSOR_H

#include "common.h"

class Cursor
{
public:
    Cursor();
    void setPos(int x, int y);

    int getX() const;
    void setX(const int &value);
    void addX(const int &value);

    int getY() const;
    void setY(const int &value);
    void addY(const int &value);

    void up();
    void down();
    void left();
    void right();

private:
    int x;
    int y;
};

#endif // CURSOR_H
