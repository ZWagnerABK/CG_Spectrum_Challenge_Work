#include "ClientConsoleManager.h"
#include <iostream>
#include <windows.h>

void ClientConsoleManager::SetClientUsernameLength(int length)
{
    m_clientUsernameLength = length;
}

void ClientConsoleManager::ResetConsole()
{
    system("cls");

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { m_logStartXPos, m_logStartYPos };

    m_currentLogXPos = m_logStartXPos;
    m_currentLogYPos = m_logStartYPos + 1;
}

void ClientConsoleManager::EraseConsoleLine()
{
    std::cout << "\33[2K";// Delete current line
    // i=1 because we included the first line
    std::cout
        << "\33[1A" // Move cursor up one
        << "\33[2K"; // Delete the entire line
    std::cout << "\r"; // Resume the cursor at beginning of line
}

void ClientConsoleManager::RepositionInputCursor(bool initial = false)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(h, &csbi);

    int bottomRow = csbi.srWindow.Bottom - 1;
    COORD c = { 0, 0 };

    if (initial)
    {
        c.X = 0;
        c.Y = bottomRow;
    }
    else
    {
        c.X = m_clientUsernameLength;
        c.Y = bottomRow;
    }

    SetConsoleCursorPosition(h, c);
}