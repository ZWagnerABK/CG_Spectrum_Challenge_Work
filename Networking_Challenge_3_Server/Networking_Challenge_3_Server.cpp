#include <enet/enet.h>
#include <iostream>

#include "GuessingGame.h"
#include "ServerMessageManager.h"
#include "Common.h"

ENetAddress g_address;
ENetHost* g_server;
int g_numberOfClients = 0;
ServerMessageManager messageManager;
int g_nextClientID = 1;

bool CreateServer();

void BroadcastStartPacket();
void BroadcastGuessResultPacket(common::GuessPacket* guessPacket, bool guessedRight);
void SendIDPacket(const ENetEvent& event, int id);

void HandleRecievePacket(const ENetEvent& event, GuessingGame& guessingGame);

int main()
{
    std::cout << "Hello!  Something will go here eventually.\n\n";

    if (enet_initialize() != 0)
    {
        fprintf(stderr, resources::kErrorWhileInitializingENETMessage);
        std::cout << resources::kErrorWhileInitializingENETMessage << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    if (!CreateServer())
    {
        fprintf(stderr,
            resources::kErrorWhileCreatingENetServerHostMessage);
        exit(EXIT_FAILURE);
    }

    std::cout << resources::kServerStartupCompleteMessage;

    GuessingGame guessingGame(0, 100);

    ENetEvent event;
    while (enet_host_service(g_server, &event, 1200000) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                messageManager.DisplayConnectionMessage(event);
                enet_uint32 hostPlusPort = event.peer->address.host + event.peer->address.port;                
                g_numberOfClients++;

                SendIDPacket(event, g_nextClientID);
                g_nextClientID++;

                if (g_numberOfClients == 2)
                {
                    messageManager.DisplayGameStartMessage(guessingGame.GetGuessingNumber());
                    BroadcastStartPacket();
                }

                break;
            }
           
            case ENET_EVENT_TYPE_RECEIVE:
            {
                HandleRecievePacket(event, guessingGame);
                break;
            }
           
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                messageManager.DisplayDisconnectMessage(event);
                g_numberOfClients--;

                if (g_numberOfClients == 0)
                {
                    goto endServer;
                }
                break;
            }
        }
    }

endServer:
    if (g_server != nullptr)
        enet_host_destroy(g_server);

    enet_deinitialize();

    return EXIT_SUCCESS;
}

bool CreateServer()
{
    /* Bind the server to the default localhost.     */
   /* A specific host address can be specified by   */
   /* enet_address_set_host (& address, "x.x.x.x"); */
    g_address.host = ENET_HOST_ANY;
    /* Bind the server to port 1234. */
    g_address.port = 1234;
    g_server = enet_host_create(&g_address /* the address to bind the server host to */,
        32      /* allow up to 32 clients and/or outgoing connections */,
        2      /* allow up to 2 channels to be used, 0 and 1 */,
        0      /* assume any amount of incoming bandwidth */,
        0      /* assume any amount of outgoing bandwidth */);



    return g_server != nullptr;
}

void SendIDPacket(const ENetEvent& event, int id)
{
    common::IDPacket* idPacket = new common::IDPacket();
    idPacket->id = id;
    idPacket->Type = common::PacketHeaderTypes::PHT_ID;

    ENetPacket* packet = enet_packet_create(idPacket,
        sizeof(common::IDPacket),
        ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(event.peer, 0, packet);

    enet_host_flush(g_server);
    delete idPacket;
}

void BroadcastStartPacket()
{
    common::StartPacket* startPacket = new common::StartPacket();
    startPacket->start = true;
    startPacket->Type = common::PacketHeaderTypes::PHT_START;

    ENetPacket* packet = enet_packet_create(startPacket,
        sizeof(common::StartPacket),
        ENET_PACKET_FLAG_RELIABLE);

    enet_host_broadcast(g_server, 0, packet);
    enet_host_flush(g_server);

    delete startPacket;
}

void BroadcastGuessResultPacket(common::GuessPacket* guessPacket, bool guessedRight)
{
    common::GuessResultPacket* guessResultPacket = new common::GuessResultPacket();
    guessResultPacket->guessedRight = guessedRight;
    guessResultPacket->Type = common::PacketHeaderTypes::PHT_GUESS_RESULT;
    guessResultPacket->guess = guessPacket->guess;
    guessResultPacket->clientID = guessPacket->clientID;
    guessResultPacket->clientUsername = guessPacket->userName;

    ENetPacket* packet = enet_packet_create(guessResultPacket,
        sizeof(common::GuessResultPacket),
        ENET_PACKET_FLAG_RELIABLE);

    enet_host_broadcast(g_server, 0, packet);
    enet_host_flush(g_server);

    delete guessResultPacket;
}

void HandleRecievePacket(const ENetEvent& event, GuessingGame& guessingGame)
{
    common::GamePacket* gamePacket = (common::GamePacket*)(event.packet->data);
    if (gamePacket)
    {
        std::cout << "Received Game Packet " << std::endl;

        switch (gamePacket->Type)
        {
            case common::PacketHeaderTypes::PHT_GUESS:
            {
                common::GuessPacket* guessPacket = (common::GuessPacket*)(event.packet->data);
                if (guessPacket)
                {
                    if (!guessingGame.IsGuessCorrect(guessPacket->guess))
                    {
                        messageManager.DisplayIncorrectGuessMessage(guessPacket->guess, guessPacket->userName);
                        BroadcastGuessResultPacket(guessPacket, false);
                    }
                    else
                    {
                        messageManager.DisplayCorrectGuessMessage(guessPacket->userName);
                        BroadcastGuessResultPacket(guessPacket, true);
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    else
    {
        std::cout << resources::kInvalidPacketMessage << std::endl;
    }

    /* Clean up the packet now that we're done using it. */
    enet_packet_destroy(event.packet);
    {
        enet_host_flush(g_server);
    }
}