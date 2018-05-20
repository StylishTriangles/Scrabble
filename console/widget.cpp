#include "widget.h"

Widget::Widget(Widget *parent) :
    parent(parent), location(0,0,1,1)
{
    resize(1,1);
}

void Widget::move(int x, int y)
{
    location.x = x;
    location.y = y;
}

void Widget::display(HANDLE outputHandle) const
{
    COORD pos = makeCOORD(location.x, location.y);
    while (pos.Y < location.y + location.h) {
        SetConsoleCursorPosition(outputHandle, pos);
        for (const Letter &l: screen[pos.Y-location.y]) {
            l.display(outputHandle);
        }
        pos.Y++;
    }
}

void Widget::resize(int w, int h)
{
    location.w = w;
    location.h = h;
    screen.resize(h);
    for (auto &line: screen) {
        line.resize(w);
    }
}

void Widget::setLetter(int relativeY, int relativeX, Letter l)
{
    screen[relativeY][relativeX] = l;
}
