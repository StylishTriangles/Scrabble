#ifndef WIDGET_H
#define WIDGET_H
#include <Windows.h>
#include <vector>
#include <set>

#include "common.h"
#include "letter.h"

class Widget
{
public:
    Widget(Widget* parent = nullptr);
    void move(int x, int y);
    void display(HANDLE outputHandle) const;
    void resize(int w, int h);
    void setLetter(int relativeY, int relativeX, Letter l);
    void setBorder(Letter l, int16 width = 1);
    void clearBorder(ConsoleColor color = ConsoleColor());

    void addChild(Widget* w);
    int getID() const {return ID;}

    int16 width() const {return location.w;}
    int16 height() const {return location.h;}

    class compareID
    {
        bool operator () (Widget const& lhs, Widget const& rhs) {return lhs.getID() < rhs.getID();}
    };
private:
    void resizeScreen(int w, int h);

    Widget* parent;
    std::set< Widget*, compareID> children;
    int ID;

    MyRectangle location;
    std::vector< std::vector<Letter> > screen;
    Letter borderLetter;
    int16 borderWidth;
};

#endif // WIDGET_H
