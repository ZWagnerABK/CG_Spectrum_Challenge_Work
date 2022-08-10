#include "ClientMessageManager.h"
#include <iostream>

ClientMessageManager::ClientMessageManager()
{
    m_ConsoleManager = std::make_unique<ClientConsoleManager>();
}

void ClientMessageManager::SetupChatroomDisplay()
{
    m_ConsoleManager->ResetConsole();

    std::cout << m_kWelcomeToChatRoomMessage << std::endl;
}

void ClientMessageManager::SetClientUsername(std::string message, std::function<bool(std::string)> condition, std::string& storage)
{
    UserInput(message, condition, storage);

    m_clientUsername = storage;
    m_ConsoleManager->SetClientUsernameLength(storage.length());
}

void ClientMessageManager::UserInput(std::string message, std::function<bool(std::string)> condition, std::string& storage)
{
    std::string input;
    bool exit = false;

    do
    {
        std::cout << message;
        getline(std::cin, input);
        m_ConsoleManager->EraseConsoleLine();

        if (std::cin.fail() || condition(input) == false)
        {
            std::cin.clear();
            std::cin.sync();
            //std::cin.ignore(INT_MAX, '\n');
            input = "";

            m_ConsoleManager->RepositionInputCursor(true);
            //std::cout << "\x1b[1F";
            //std::cout << "\x1b[2K";
        }
        else
        {
            storage = input;
            exit = true;
        }

    } while (!exit);
}