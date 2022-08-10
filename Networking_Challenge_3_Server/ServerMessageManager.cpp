#include "ServerMessageManager.h"
#include <iostream>

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