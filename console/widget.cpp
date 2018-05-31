#include "widget.h"
#include <algorithm>

Widget::Widget(Widget *parent) :
    parent(parent), location(0,0,1,1)
{
    static int IDenum = 0;
    IDenum++;
    ID = IDenum;

    resizeScreen(1,1);
}

void Widget::move(int x, int y)
{
    if (cursor) {
        cursor->addX(x - location.x);
        cursor->addY(y - location.y);
    }
    location.x = x;
    location.y = y;
}

void Widget::display(HANDLE outputHandle)
{
    Letter cpy;
    COORD cursorPos;
    if (cursorEnabled and cursor != nullptr) {
        cursorPos = {(SHORT)(cursor->getX()+borderWidth),
                     (SHORT)(cursor->getY()+borderWidth)};
        SetConsoleCursorPosition(outputHandle, cursorPos);
        // backup symbol under cursor
        cpy = Letter(screen[cursorPos.Y][cursorPos.X]);
        // modify symbol
        screen[cursorPos.Y][cursorPos.X].color().swapColors();
    }
    COORD pos = makeCOORD(location.x, location.y);
    while (pos.Y < location.y + location.h) {
        SetConsoleCursorPosition(outputHandle, pos);
        for (const Letter &l: screen[pos.Y-location.y]) {
            l.display(outputHandle);
        }
        pos.Y++;
    }
    if (cursorEnabled and cursor != nullptr) {
        // restore symbol under cursor
        screen[cursorPos.Y][cursorPos.X] = cpy;
    }

}

void Widget::resize(int w, int h)
{
    int16 oldBW = borderWidth;
    clearBorder();
    resizeScreen(w, h);
    setBorder(borderLetter, oldBW);
}

void Widget::setLetter(int relativeY, int relativeX, Letter l, bool paintOnBorder)
{
    if (!paintOnBorder) {
        relativeX += borderWidth;
        relativeY += borderWidth;
    }
    screen[relativeY][relativeX] = l;
}

void Widget::setBorder(Letter l, int16 width)
{
    borderLetter = l;
    borderWidth = width;
    for (int i = 0; i < std::min(width, this->height()); i++) {
        for (int j = 0; j < this->width(); j++) {
            screen[i][j] = l;
            screen[this->height()-i-1][j] = l;
        }
    }
    for (int i = 0; i < std::min(width, this->width()); i++) {
        for (int j = 0; j < this->height(); j++) {
            screen[j][i] = l;
            screen[j][this->width()-i-1] = l;
        }
    }
}

void Widget::clearBorder(ConsoleColor color)
{
    setBorder(Letter(' ', color), borderWidth);
    borderWidth = 0;
}

void Widget::setCursor(Cursor *c)
{
    cursor = c;
}

void Widget::resizeScreen(int w, int h)
{
    location.w = w;
    location.h = h;
    screen.resize(h);
    for (auto &line: screen) {
        line.resize(w);
    }
}
