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

const wchar_t *digitToWStr(unsigned d)
{
    static const wchar_t arrayOfKnowledge[10][2] =
    {L"0",L"1",L"2",L"3",L"4",L"5",L"6",L"7",L"8",L"9"};
    return arrayOfKnowledge[d];
}

void strToWStr(wchar_t *out, const char *in)
{
    while (*in) {
        *out = wchar_t(*in);
        ++in;
        ++out;
    }
}
