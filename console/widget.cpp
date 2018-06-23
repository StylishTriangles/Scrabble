#include "widget.h"
#include <algorithm>

Widget::Widget(Widget *parent) :
    parent(parent), location(0,0,1,1), borderWidth(0)
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
    location.x = int16(x);
    location.y = int16(y);
}

void Widget::display(HANDLE outputHandle)
{
    Letter cpy;
    COORD cursorPos;
    if (cursorEnabled and cursor != nullptr) {
        cursorPos = {SHORT(cursor->getX()+borderWidth),
                     SHORT(cursor->getY()+borderWidth)};
        SetConsoleCursorPosition(outputHandle, cursorPos);
        // backup symbol under cursor
        cpy = Letter(screen[unsigned(cursorPos.Y)][unsigned(cursorPos.X)]);
        // modify symbol
        screen[unsigned(cursorPos.Y)][unsigned(cursorPos.X)].color().swapColors();
    }
    COORD pos = makeCOORD(location.x, location.y);
    while (pos.Y < location.y + location.h) {
        SetConsoleCursorPosition(outputHandle, pos);
        for (const Letter &l: screen[unsigned(pos.Y-location.y)]) {
            l.display(outputHandle);
        }
        pos.Y++;
    }
    if (cursorEnabled and cursor != nullptr) {
        // restore symbol under cursor
        screen[unsigned(cursorPos.Y)][unsigned(cursorPos.X)] = cpy;
    }

}

void Widget::resize(int16 w, int16 h)
{
    uint16 oldBW = borderWidth;
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
    screen[unsigned(relativeY)][unsigned(relativeX)] = l;
}

void Widget::setLetter(int relativeY, int relativeX, wchar_t l, ConsoleColor col, bool paintOnBorder)
{
    setLetter(relativeY, relativeX, Letter(l, col), paintOnBorder);
}

void Widget::setString(int relativeY, int relativeX, const wchar_t *cwstr, ConsoleColor col, bool paintOnBorder)
{
    int maxX = width();
    int maxY = height();
    if (!paintOnBorder) {
        relativeX += borderWidth;
        relativeY += borderWidth;
        maxX-=borderWidth;
        maxY-=borderWidth;
    }
    int lineBegin = relativeX;
    paintOnBorder &= 1;
    while (*cwstr) {
        if (relativeX >= maxX or *cwstr == L'\n') {
            relativeY++;
            relativeX = lineBegin;
        }
        if (*cwstr == L'\n') {
            ++cwstr;
            continue;
        }
        if (relativeY >= maxY) {
            return;
        }
        screen[unsigned(relativeY)][unsigned(relativeX)] = Letter(*cwstr, col);
        ++relativeX;
        ++cwstr;
    }
}

void Widget::setBackgroundColor(CCOLOR c)
{
    for (unsigned i = borderWidth; i < unsigned(height()) - borderWidth; i++) {
        for (unsigned j = borderWidth; j < unsigned(width()) - borderWidth; j++) {
            screen[i][j].color().setBackground(c);
        }
    }
}

void Widget::setBorder(Letter l, uint16 width)
{
    borderLetter = l;
    borderWidth = width;
    for (unsigned i = 0; i < std::min(width, uint16(this->height())); i++) {
        for (unsigned j = 0; j < unsigned(this->width()); j++) {
            screen[i][j] = l;
            screen[unsigned(this->height())-i-1][j] = l;
        }
    }
    for (unsigned i = 0; i < std::min(width, uint16(this->width())); i++) {
        for (unsigned j = 0; j < unsigned(this->height()); j++) {
            screen[j][i] = l;
            screen[j][unsigned(this->width())-i-1] = l;
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

void Widget::resizeScreen(int16 w, int16 h)
{
    location.w = w;
    location.h = h;
    screen.resize(uint32(h));
    for (auto &line: screen) {
        line.resize(uint32(w));
    }
}
