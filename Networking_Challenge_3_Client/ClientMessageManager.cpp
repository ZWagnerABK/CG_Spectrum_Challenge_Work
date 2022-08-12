#include "ClientMessageManager.h"
#include "Common.h"

ClientMessageManager::ClientMessageManager()
{
    m_Console = std::make_unique<ClientConsole>();
}

void ClientMessageManager::SetupGuessingRoomDisplay()
{
    m_Console->ResetConsole();

    std::cout << resources::kWelcomeMessage << std::endl;

    m_Console->RepositionInputCursor(true);
}

void ClientMessageManager::SetClientUsername(std::string message, std::function<bool(std::string)> condition, std::string& storage)
{
    UserInput(message, condition, storage);

    m_clientUsername = storage;
    m_Console->SetClientUsernameLength(storage.length());
}

void ClientMessageManager::UserInput(std::string message, std::function<bool(std::string)> condition, std::string& storage)
{
    std::string input;
    bool exit = false;

    do
    {
        std::cout << message;
        getline(std::cin, input);
        m_Console->EraseConsoleLine();

        if (std::cin.fail() || condition(input) == false)
        {
            std::cin.clear();
            std::cin.sync();
            //std::cin.ignore(INT_MAX, '\n');
            input = "";

            m_Console->RepositionInputCursor(true);
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

void ClientMessageManager::UserInput(std::string message, std::function<bool(int)> condition, int& storage)
{
    int input = -1;
    bool exit = false;

    do
    {
        std::cout << message;
        std::cin >> input;
        m_Console->EraseConsoleLine();

        if (std::cin.fail() || condition(input) == false)
        {
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            std::cin.sync();

            input = -1;

            m_Console->RepositionInputCursor(true);
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

void ClientMessageManager::DisplayWrongGuessMessage(bool didIGuessWrong, std::string otherUsername, int wrongGuess)
{
    m_Console->RepositionToLogPostion();

    if (didIGuessWrong)
    {
        std::cout << resources::kYouIncorrectGuessClientMessage << wrongGuess << std::endl;
    }
    else 
    {
        std::cout << otherUsername << resources::kIncorrectGuessClientMessage << wrongGuess << std::endl;
    }

    m_Console->RepositionInputCursor(false);
}

void ClientMessageManager::DisplayCorrectGuessMessage(bool didIGuessWrong, std::string otherUsername, int rightGuess)
{
    m_Console->RepositionToLogPostion();

    if (didIGuessWrong)
    {
        std::cout << resources::kYouCorrectGuessClientMessage << rightGuess << "." << resources::kEndingClientMessage <<  std::endl;
    }
    else
    {
        std::cout << otherUsername << resources::kCorrectGuessClientMessage << std::endl;
    }

    m_Console->RepositionInputCursor(false);
}