#include "ClientConsole.h"
#include <iostream>
#include <windows.h>

void ClientConsole::SetInputMessageLength(int inputMessageLength)
{
    m_inputMessageLength = inputMessageLength;
}

void ClientConsole::SetupConsole()
{
    //Turn on Virtual Terminal Processing so \33 commands will work for Windows 10 and above
    //Apperantly turned off by default
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(h, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(h, consoleMode);
}

void ClientConsole::ResetConsole()
{
    system("cls");

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { m_logStartXPos, m_logStartYPos };

    m_currentLogXPos = m_logStartXPos;
    m_currentLogYPos = m_logStartYPos + 1;
}

void ClientConsole::RepositionToLogPostion()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { m_currentLogXPos , m_currentLogYPos };

    SetConsoleCursorPosition(h, c);
}

void ClientConsole::IncremenetLogPosition()
{
    m_currentLogYPos++;
}

void ClientConsole::EraseConsoleLine()
{
    std::cout << "\33[2K";// Delete current line
    // i=1 because we included the first line
    std::cout
        << "\33[1A" // Move cursor up one
        << "\33[2K"; // Delete the entire line
    std::cout << "\r"; // Resume the cursor at beginning of line
}

void ClientConsole::RepositionInputCursor(bool initial = false)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(h, &csbi);

    int bottomRow = csbi.srWindow.Bottom - 1;
    COORD c = { 0, 0 };

    if (initial)
    {
        //Place cursor in position for input message to be written to the screen.
        c.X = 0;
        c.Y = bottomRow;
    }
    else
    {
        //Place the cursor at the end of the input message already on the screen.
        c.X = m_inputMessageLength;
        c.Y = bottomRow;
    }

    SetConsoleCursorPosition(h, c);
}