#ifndef COMMON_H
#define COMMON_H
#include <cinttypes>
#include <Windows.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

struct MyRectangle {
    MyRectangle(int x, int y, int width, int height) :
        x(x), y(y), w(width), h(height) {}
    int16 x, y;
    int16 w, h;
};

inline COORD makeCOORD(short x, short y) {
    return {x,y};
}


#endif // COMMON_H
