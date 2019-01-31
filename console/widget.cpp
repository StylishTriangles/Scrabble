#include "widget.h"
#include <algorithm>

/**
 *  The default Widget constructor.
 *	@param parent: parent Widget.
 **/
Widget::Widget(Widget *parent) :
    parent(parent), location(0,0,1,1), borderWidth(0), cursor(nullptr), cursorEnabled(false)
{
    static int IDenum = 0;
    IDenum++;
    ID = IDenum;

    resizeScreen(1,1);
}

/**
 *  Move widget to specified console coordinates.
 *  @param x: New X-position.
 *  @param y: New Y-position.
 **/
void Widget::move(int x, int y)
{
    if (cursor) {
        cursor->addX(x - location.x);
        cursor->addY(y - location.y);
    }
    location.x = int16(x);
    location.y = int16(y);
}

/**
 *  Display this widget.
 *  @param outputHandle: device to output to specified by HANDLE (usually STANDARD_OUTPUT_HANDLE).
 **/
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

/**
 *  Resize the widget.
 *  @param w: New width.
 *  @param h: New height.
 **/
void Widget::resize(int16 w, int16 h)
{
    uint16 oldBW = borderWidth;
    clearBorder();
    resizeScreen(w, h);
    setBorder(borderLetter, oldBW);
}

/**
 *  Set a letter on given coordinates.
 *  @param relativeY: Y-coord relative to widget position (0 is the first non-border tile).
 *  @param relativeX: X-coord relative to widget position (0 is the first non-border tile).
 *  @param l: Object of type Letter to be printed on widget.
 *  @param paintOnBorder: If switched to true the string can be painted on border. Provided coords will map to top left corner of widget.
 **/
void Widget::setLetter(int relativeY, int relativeX, Letter l, bool paintOnBorder)
{
    if (!paintOnBorder) {
        relativeX += borderWidth;
        relativeY += borderWidth;
    }
    screen[unsigned(relativeY)][unsigned(relativeX)] = l;
}

/**
 *  Set a letter on given coordinates.
 *  @param relativeY: Y-coord relative to widget position (0 is the first non-border tile).
 *  @param relativeX: X-coord relative to widget position (0 is the first non-border tile).
 *  @param l: Character to be printed on widget.
 *	@param col: Color of the character.
 *  @param paintOnBorder: If switched to true the string can be painted on border. Provided coords will map to top left corner of widget.
 **/
void Widget::setLetter(int relativeY, int relativeX, wchar_t l, ConsoleColor col, bool paintOnBorder)
{
    setLetter(relativeY, relativeX, Letter(l, col), paintOnBorder);
}

/**
 *  Set a string on given coordinates.
 *  @param relativeY: Y-coord relative to widget position (0 is the first non-border tile).
 *  @param relativeX: X-coord relative to widget position (0 is the first non-border tile).
 *  @param cwstr: String to be printed on widget.
 *	@param col: Color of the string.
 *  @param paintOnBorder: If switched to true the string can be painted on border. Provided coords will map to top left corner of widget.
 **/
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

/**
 *  Set color of the background, but not of the border.
 **/
void Widget::setBackgroundColor(CCOLOR c)
{
    for (unsigned i = borderWidth; i < unsigned(height()) - borderWidth; i++) {
        for (unsigned j = borderWidth; j < unsigned(width()) - borderWidth; j++) {
            screen[i][j].color().setBackground(c);
        }
    }
}

/**
 *  Add border to widget.
 *  @param l: Letter used in border painting.
 *	@param width: Width of the border.
 **/
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

/**
 *  Set border's color.
 **/
void Widget::setBorderColor(ConsoleColor col)
{
    borderLetter.setColor(col);
    for (unsigned i = 0; i < std::min(borderWidth, uint16(this->height())); i++) {
        for (unsigned j = 0; j < unsigned(this->width()); j++) {
            screen[i][j].setColor(col);
            screen[unsigned(this->height())-i-1][j].setColor(col);
        }
    }
    for (unsigned i = 0; i < std::min(borderWidth, uint16(this->width())); i++) {
        for (unsigned j = 0; j < unsigned(this->height()); j++) {
            screen[j][i].setColor(col);
            screen[j][unsigned(this->width())-i-1].setColor(col);
        }
    }
}

/**
 *  Remove border from widget.
 **/
void Widget::clearBorder(ConsoleColor color)
{
    setBorder(Letter(' ', color), borderWidth);
    borderWidth = 0;
}

/**
 *  Set the cursor to be displayed on widget.
 **/
void Widget::setCursor(Cursor *c)
{
    cursorEnabled = true;
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
