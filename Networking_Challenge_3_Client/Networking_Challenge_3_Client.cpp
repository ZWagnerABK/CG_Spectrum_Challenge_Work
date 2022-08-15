#include <enet/enet.h>
#include <iostream>
#include <functional>
#include <string>
#include <thread>

#include "ClientMessageManager.h";
#include "Common.h"

ClientMessageManager messageManager;

ENetAddress g_address;
ENetHost* g_client = nullptr;
ENetPeer* peer = nullptr;

std::string g_clientUserName = "";

int g_guessedNumber = -1;
int g_clientID = -1;

bool g_connectedToServer = false;
bool g_startUserInput = false;
bool g_endClient = false;
bool g_endThreads = false;
bool g_consoleExit = false;

int InitializeClient();

bool CreateClient();

ENetPeer* ConnectToServer(ENetEvent &event);

void HandleReceivePacket(const ENetEvent& event);
void UserInputThread();
void BroadcastGuessPacket();
void ClientDisconnect();

BOOL WINAPI CtrlHandler(DWORD dwCtrlType);

//TODO:  Not sure if I'm happy with the way I did all message display across the project.  Would reinvestigate for a refactor.
int main()
{
    if(InitializeClient() == 1)
        return EXIT_FAILURE;

    //Thought it would be a better idea to store the username in the ClientMessageManager rather than constanly pass it in as a parameter.
    messageManager.SetClientUsername(resources::kEnterUserName, [](std::string input){ return input != " "; }, g_clientUserName);

    ENetEvent event;
    peer = ConnectToServer(event);

    std::thread inputThread(UserInputThread);

    while (enet_host_service(g_client, &event, 1200000) > 0 && !g_consoleExit)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
            {
                HandleReceivePacket(event);
                if(g_endClient)
                    goto endClient;

                break;
            }  
        }
    }

endClient:

    inputThread.join();
    ClientDisconnect();

    if (g_client != nullptr)
        enet_host_destroy(g_client);

    enet_deinitialize();

    return EXIT_SUCCESS;
}

bool CreateClient()
{
    g_client = enet_host_create(NULL /* create a client host */,
        1 /* only allow 1 outgoing connection */,
        2 /* allow up 2 channels to be used, 0 and 1 */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);

    return g_client != nullptr;
}

ENetPeer* ConnectToServer(ENetEvent &event)
{
    ENetPeer* peer;
    ENetAddress address;

    /* Connect to some.server.net:1234. */
    enet_address_set_host(&address, "127.0.0.1");
    address.port = 1234;
    /* Initiate the connection, allocating the two channels 0 and 1. */

    peer = enet_host_connect(g_client, &address, 2, 0);

    if (peer == NULL)
    {
        fprintf(stderr,
            resources::kENetNoPeerAvialable);
        exit(EXIT_FAILURE);
    }
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(g_client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        std::cout << resources::kENetConnectionSuccess << std::endl;

        g_connectedToServer = true;
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(peer);
        std::cout << resources::kENetConnectionFailure << std::endl;
    }

    return peer;
}

int InitializeClient()
{
    if (enet_initialize() != 0)
    {
        fprintf(stderr, resources::kENetInitializingErrorMessage);
        std::cout << resources::kENetInitializingErrorMessage << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    messageManager.SetupConsole();
    //TODO:  Find a way to move this into the ClientConsole class. Ran into issue where it requires a static handler class.
    SetConsoleCtrlHandler(CtrlHandler, TRUE);

    if (!CreateClient())
    {
        fprintf(stderr,
            resources::kENetDefaultErrorMessage);
        exit(EXIT_FAILURE);
    }

    return 0;
}

void UserInputThread()
{
    while (!g_endThreads)
    {
        if (g_startUserInput == true)
        {
            g_guessedNumber = -1;
            messageManager.UserInput(resources::kInputLine, [](int input) { return input >= 0; }, g_guessedNumber);
            BroadcastGuessPacket();
        }
    }
}

void HandleReceivePacket(const ENetEvent& event)
{
    common::GamePacket* gamePacket = (common::GamePacket*)(event.packet->data);
    if (gamePacket)
    {
        switch (gamePacket->Type)
        {
            case common::PacketHeaderTypes::PHT_ID:
            {
                common::IDPacket* idPacket = (common::IDPacket*)(event.packet->data);
                if (idPacket)
                {
                    g_clientID = idPacket->id;
                }
                break;
            }
            case common::PacketHeaderTypes::PHT_START:
            {
                common::StartPacket* startPacket = (common::StartPacket*)(event.packet->data);
                if (startPacket && startPacket->start == true)
                {
                    messageManager.SetupGuessingRoomDisplay();
                    g_startUserInput = true;
                }
                break;
            }
            case common::PacketHeaderTypes::PHT_GUESS_RESULT:
            {
                common::GuessResultPacket* guessResultPacket = (common::GuessResultPacket*)(event.packet->data);
                if (guessResultPacket)
                {
                    if (guessResultPacket->guessedRight == false)
                    {
                        messageManager.DisplayWrongGuessMessage(guessResultPacket->clientID == g_clientID, guessResultPacket->clientUsername, guessResultPacket->guess);
                        g_startUserInput = true;
                    }
                    else
                    {
                        messageManager.DisplayCorrectGuessMessage(guessResultPacket->clientID == g_clientID, guessResultPacket->clientUsername, guessResultPacket->guess);
                        if (g_clientID == guessResultPacket->clientID)
                        {
                            g_endClient = true;
                            g_endThreads = true;
                        }
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
        enet_host_flush(g_client);
    }
}

void BroadcastGuessPacket()
{
    common::GuessPacket* guessPacket = new common::GuessPacket();
    guessPacket->guess = g_guessedNumber;
    guessPacket->Type = common::PacketHeaderTypes::PHT_GUESS;
    guessPacket->userName = g_clientUserName;
    guessPacket->clientID = g_clientID;

    ENetPacket* packet = enet_packet_create(guessPacket,
        sizeof(common::GuessPacket),
        ENET_PACKET_FLAG_RELIABLE);

    enet_host_broadcast(g_client, 0, packet);
    enet_host_flush(g_client);

    g_startUserInput = false;

    delete guessPacket;
}

//Used as a handler for SetConsoleCtrlHandler to deal with disconnecting from the server on specific exit conditions related to the console.
//Noted in another TODO that ideally this would be in the ClientConsole class, was not sure how to implement it there without major refactoring.
BOOL WINAPI CtrlHandler(DWORD dwCtrlType)
{
    switch (dwCtrlType)
    {
        case CTRL_CLOSE_EVENT:
        {
            ClientDisconnect();
            return FALSE;
        }
        case CTRL_C_EVENT:
        {
            ClientDisconnect();
            return TRUE;
        }
        case CTRL_BREAK_EVENT:
            return FALSE;
        case CTRL_LOGOFF_EVENT:
            return FALSE;
        case CTRL_SHUTDOWN_EVENT:
            return FALSE;
        default:
            return FALSE;
            break;
    }
}

void ClientDisconnect()
{
    if (peer != nullptr)
    {
        enet_peer_disconnect(peer, 0);
        enet_host_flush(g_client);
    }
}