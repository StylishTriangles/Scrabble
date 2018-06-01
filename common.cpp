#include "common.h"

int wstrlen(const wchar_t *str)
{
    int rcx = 0;
    while (*str) {
        ++rcx;
        ++str;
    }
    return rcx;
}
