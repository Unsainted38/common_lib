#ifndef CONSOLE_UTILITIES_H
#define CONSOLE_UTILITIES_H

#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <cstdio>
#endif

void clearConsole()
{
#ifdef Q_OS_WIN
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hConsole == INVALID_HANDLE_VALUE)
        return;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        return;

    DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD count;
    COORD homeCoords = {0, 0};

    FillConsoleOutputCharacter(
        hConsole,
        ' ',
        cellCount,
        homeCoords,
        &count
        );

    FillConsoleOutputAttribute(
        hConsole,
        csbi.wAttributes,
        cellCount,
        homeCoords,
        &count
        );

    SetConsoleCursorPosition(hConsole, homeCoords);
#else
    std::printf("\033[2J\033[H");
    std::fflush(stdout);
#endif
}

#endif // CONSOLE_UTILITIES_H
