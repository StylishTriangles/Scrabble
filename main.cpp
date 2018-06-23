#include <cstdio>
#include <windows.h>

#include "console/widget.h"
#include "game/game.h"
using namespace std;

static wchar_t playerNames[4][128] = {
    L"Ptak",
    L"Torvalds",
    L"Gates",
    L"Knuth",
};

int main()
{
    SetConsoleTitleW(L"Scrabble");
    DWORD charsRead;
    char buffer[128];
    int playerCount = 2;
    HANDLE hout = CreateFileW(L"players.txt",GENERIC_READ,0,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);
    if (hout == INVALID_HANDLE_VALUE) {
        printf("players.txt not found!\nHow many players want to play: ");
        scanf("%d", &playerCount);
        if (playerCount >= 1 and playerCount <= 4) {
            goto RUNGAME;
        } else if (playerCount > 4) {
            printf("You don't have that many friends mate.\n");
            Sleep(1000); // Mock player before exiting
            return 0;
        } else {
            printf("Very funny.\n");
            Sleep(1000);
            return 0;
        }

    } else {
        ReadFile(hout,buffer,127,&charsRead,nullptr);
        CloseHandle(hout);
        int ix = 0;
        int dx = 0;
        for (unsigned i = 0; i < charsRead; i++) {
            if (buffer[i] == '\n') {
                playerNames[ix][dx] = 0;
                ++ix;
                dx = 0;
                continue;
            } if (buffer[i] == '\r') {
                continue;
            }
            playerNames[ix][dx] = wchar_t(buffer[i]);
            dx++;
        }
        playerCount = min(4,ix);
        if (playerCount == 0) {
            printf("Read 0 lines, players.txt is not formatted correctly.\n");
            Sleep(1000);
            return 0;
        }
    }

    RUNGAME:
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ConsoleColor(GREEN, DARK_GREEN));
    printf("Starting game...");
    Sleep(200); // Prevents enter key from being captured by EventManager
    system("cls");
    Game* g = new Game();
    for (int i = 0; i < playerCount; i++) {
        playerNames[i][MAX_PLAYER_NAME_LEN] = 0;
        g->addPlayer(playerNames[i]);
    }
    g->run();
    return 0;
}
