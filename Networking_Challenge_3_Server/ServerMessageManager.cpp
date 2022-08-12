#include <iostream>

#include "ServerMessageManager.h"
#include "Common.h"

void ServerMessageManager::DisplayConnectionMessage(ENetEvent event)
{
    std::cout << "A new client connected from "
        << event.peer->address.host
        << ":" << event.peer->address.port
        << "\n";
}

void ServerMessageManager::DisplayDisconnectMessage(ENetEvent event)
{
    std::cout << "The client from "
        << event.peer->address.host
        << ":" << event.peer->address.port
        << " has disconnected.\n";
}

void ServerMessageManager::DisplayGameStartMessage(int number)
{
    std::cout << resources::kGameStartingMessage << number << "." << std::endl;
}

void ServerMessageManager::DisplayIncorrectGuessMessage(int guessNumber, std::string clientUsername)
{
    std::cout << clientUsername << resources::kIncorrectGuessServerMessage << guessNumber << std::endl;
}

void ServerMessageManager::DisplayCorrectGuessMessage(std::string clientUsername)
{
    std::cout << clientUsername << resources::kCorrectGuessServerMessage << std::endl;
}