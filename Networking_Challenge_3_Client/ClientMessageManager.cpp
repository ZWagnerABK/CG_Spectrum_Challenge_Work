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

void ClientMessageManager::SetupConsole()
{
    std::string inputMessage(resources::kInputLine);

    m_Console->SetInputMessageLength(inputMessage.length());
    m_Console->SetupConsole();
}

void ClientMessageManager::SetClientUsername(std::string message, std::function<bool(std::string)> condition, std::string& storage)
{
    UserInput(message, condition, storage);

    m_clientUsername = storage;
}

//TODO:  Refactor this function so that it can store the message being typed before enter is hit.
//       Right now the program may run fine for me, but if this was truly two seperate clients entering input at the same time, the input line would break very fast.
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

//TODO:  Refactor this function so that it can store the message being typed before enter is hit.
//       Right now the program may run fine for me, but if this was truly two seperate clients entering input at the same time, the input line would break very fast.
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
        m_Console->IncremenetLogPosition();
        m_Console->RepositionInputCursor(true);
    }
    else 
    {
        std::cout << otherUsername << resources::kIncorrectGuessClientMessage << wrongGuess << std::endl;
        m_Console->IncremenetLogPosition();
        m_Console->RepositionInputCursor(false);
    }

   
}

void ClientMessageManager::DisplayCorrectGuessMessage(bool didIGuessWrong, std::string otherUsername, int rightGuess)
{
    m_Console->RepositionToLogPostion();

    if (didIGuessWrong)
    {
        std::cout << resources::kYouCorrectGuessClientMessage << rightGuess << ". " << resources::kEndingClientMessage <<  std::endl;
        m_Console->IncremenetLogPosition();
        m_Console->RepositionInputCursor(true);
    }
    else
    {
        std::cout << otherUsername << resources::kCorrectGuessClientMessage << std::endl;
        m_Console->IncremenetLogPosition();
        m_Console->RepositionInputCursor(false);
    }
}