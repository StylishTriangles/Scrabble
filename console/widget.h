#ifndef WIDGET_H
#define WIDGET_H
#include <windows.h>
#include <vector>
#include <set>

#include "common.h"
#include "cursor.h"
#include "letter.h"

class Widget
{
public:
    Widget(Widget* parent = nullptr);
    void move(int x, int y);
    void display(HANDLE outputHandle);
    void resize(int16 w, int16 h);
    void setLetter(int relativeY, int relativeX, Letter l, bool paintOnBorder = false);
    void setLetter(int relativeY, int relativeX, wchar_t l, ConsoleColor col = ConsoleColor(), bool paintOnBorder = false);
    void setString(int relativeY, int relativeX, const wchar_t* cwstr,
                   ConsoleColor col = ConsoleColor(), bool paintOnBorder = false);
    void setBackgroundColor(CCOLOR c);
    void setBorder(Letter l, uint16 width = 1);
    ConsoleColor getBorderColor() const {return borderLetter.color();}
    void setBorderColor(ConsoleColor col);
    void clearBorder(ConsoleColor color = ConsoleColor());

    void addChild(Widget* w);
    int getID() const {return ID;}

    int16 width() const {return location.w;}
    int16 height() const {return location.h;}
    int16 canvasWidth() const {return location.w-borderWidth*2;}
    int16 canvasHeight() const {return location.h-borderWidth*2;}

    void setCursor(Cursor* c);
    void toggleCursor() {cursorEnabled^=1;}

    class compareID
    {
        bool operator () (Widget const& lhs, Widget const& rhs) {return lhs.getID() < rhs.getID();}
    };

private:
    void resizeScreen(int16 w, int16 h);

    Widget* parent;
    std::set< Widget*, compareID > children;
    int ID;

    MyRectangle location;
    std::vector< std::vector<Letter> > screen;
    Letter borderLetter;
    uint16 borderWidth;

    Cursor* cursor;
    bool cursorEnabled;
};

#endif // WIDGET_H
