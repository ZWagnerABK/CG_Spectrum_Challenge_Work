#include <enet/enet.h>
#include <iostream>

#include "GuessingGame.h"
#include "ServerMessageManager.h"

ENetAddress g_address;
ENetHost* g_server;
int g_numberOfClients = 0;

const char* g_kServerStartupCompleteMessage = "Server startup complete! Waiting for clients to connect. . .\n";
const char* g_kErrorWhileInitializingENETMessage = "An error occurred while initializing ENet.\n";
const char* g_kErrorWhileCreatingENetServerHostMessage = "An error occurred while trying to create an ENet server host.\n";

bool CreateServer();

int main()
{
    std::cout << "Hello!  Something will go here eventually.\n\n";

    if (enet_initialize() != 0)
    {
        fprintf(stderr, g_kErrorWhileInitializingENETMessage);
        std::cout << g_kErrorWhileInitializingENETMessage << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    if (!CreateServer())
    {
        fprintf(stderr,
           g_kErrorWhileCreatingENetServerHostMessage);
        exit(EXIT_FAILURE);
    }

    std::cout << g_kServerStartupCompleteMessage;

    GuessingGame guessingGame(0, 100);
    ServerMessageManager serverMessageManager;

    ENetEvent event;
    while (enet_host_service(g_server, &event, 1200000) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            serverMessageManager.DisplayConnectionMessage(event);
            g_numberOfClients++;

            break;
        case ENET_EVENT_TYPE_RECEIVE:
            /*std::cout << event.packet->data << std::endl;
            SendPacket((char*)event.packet->data);
            enet_packet_destroy(event.packet);*/

            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            serverMessageManager.DisplayDisconnectMessage(event);
            g_numberOfClients--;
            break;
        }
    }

    if (g_server != nullptr)
        enet_host_destroy(g_server);

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