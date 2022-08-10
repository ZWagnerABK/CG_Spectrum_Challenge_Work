#include <enet/enet.h>
#include <iostream>
#include <functional>
#include <string>

#include "ClientMessageManager.h";

ENetAddress g_address;
ENetHost* g_client;
std::string g_clientUserName = "";
bool g_connectedToServer = false;

const char* g_kENetDefaultErrorMessage = "An error occurred while trying to create an ENet client host.\n";
const char* g_kENetInitializingErrorMessage = "An error occurred while initializing ENet.\n";
const char* g_kENetNoPeerAvialable = "No available peers for initiating an ENet connection.\n";
const char* g_kEnterUserName = "Enter a name for the client user: ";

const char* g_kENetConnectionSuccess = "Connection to 127.0.0.1:1234 succeeded.  Waiting for other player to join.";
const char* g_kENetConnectionFailure = "Connection 127.0.0.1:1234 failed.";

int InitializeClient();
bool CreateClient();
void ConnectToServer(ENetEvent &event);

void UserInput(std::string message, std::function<bool(std::string)> condition, std::string& storage);

int main()
{
    std::cout << "Hello!  This will say something later.\n\n";

    if(InitializeClient() == 1)
        return EXIT_FAILURE;

    ClientMessageManager messageManager;

    messageManager.SetClientUsername(g_kEnterUserName, [](std::string input){ return input != " "; }, g_clientUserName);

    ENetEvent event;
    ConnectToServer(event);

    while (enet_host_service(g_client, &event, 1200000) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                //AddMessageToLogQueue((char*)event.packet->data);
                enet_packet_destroy(event.packet);
                //messageManager.SetupChatroomDisplay();

                break;
        }
    }
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

void ConnectToServer(ENetEvent &event)
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
            g_kENetNoPeerAvialable);
        exit(EXIT_FAILURE);
    }
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(g_client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        std::cout << g_kENetConnectionSuccess << std::endl;

        g_connectedToServer = true;
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(peer);
        std::cout << g_kENetConnectionFailure << std::endl;
    }
}

int InitializeClient()
{
    if (enet_initialize() != 0)
    {
        fprintf(stderr, g_kENetInitializingErrorMessage);
        std::cout << g_kENetInitializingErrorMessage << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(h, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(h, consoleMode);

    if (!CreateClient())
    {
        fprintf(stderr,
            g_kENetDefaultErrorMessage);
        exit(EXIT_FAILURE);
    }

    return 0;
}