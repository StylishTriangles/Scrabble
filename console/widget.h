#ifndef WIDGET_H
#define WIDGET_H
#include <Windows.h>
#include <vector>

#include "common.h"
#include "letter.h"

class Widget
{
public:
    Widget(Widget* parent = nullptr);
    void move(int x, int y);
    void display(HANDLE outputHandle) const;
    void resize(int w, int h);
    void setLetter(int relativeX, int relativeY, Letter l);
private:
    Widget* parent;
    MyRectangle location;
    std::vector< std::vector<Letter> > screen;
};

#endif // WIDGET_H
