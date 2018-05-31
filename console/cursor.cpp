#include "cursor.h"

Cursor::Cursor()
{

}

void Cursor::setPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

int Cursor::getX() const
{
    return x;
}

void Cursor::setX(const int &value)
{
    x = value;
}

void Cursor::addX(const int &value)
{
    x += value;
}

int Cursor::getY() const
{
    return y;
}

void Cursor::setY(const int &value)
{
    y = value;
}

void Cursor::addY(const int &value)
{
    y += value;
}

void Cursor::up()
{
    y--;
}

void Cursor::down()
{
    y++;
}

void Cursor::left()
{
    x--;
}

void Cursor::right()
{
    x++;
}
