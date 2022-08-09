#include <enet/enet.h>
#include <iostream>
#include <string>
#include <functional>

ENetAddress address;
ENetHost* client;
std::string clientUserName = "";

bool CreateClient();
ENetPeer* ConnectToServer();

const char* kENetDefaultErrorMessage = "An error occurred while trying to create an ENet client host.\n";
const char* kENetInitializingErrorMessage = "An error occurred while initializing ENet.\n";
const char* kENetNoPeerAvialable = "No available peers for initiating an ENet connection.\n";

const char* kENetConnectionSuccess = "Connection to 127.0.0.1:1234 succeeded.";
const char* kENetConnectionFailure = "Connection 127.0.0.1:1234 failed.";

void UserInput(std::string, std::function<bool(std::string)>, std::string& storage);
void UserInput(std::string, std::function<bool(std::string)>);

void SendPacket();

std::string GetUsernameInputFormatted(std::string username);
void SetupChatroomDisplay();

int main()
{
    std::cout << "Hello!  This program creates an ENet Client and waits for the user to exit before turning it off.\n\n";

    if (enet_initialize() != 0)
    {
        fprintf(stderr, kENetInitializingErrorMessage);
        std::cout << kENetInitializingErrorMessage << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    if (!CreateClient())
    {
        fprintf(stderr,
            kENetDefaultErrorMessage);
        exit(EXIT_FAILURE);
    }

    UserInput("Enter a name for the client user: ", [](std::string input) { return input != ""; }, clientUserName);

    if (!CreateClient())
    {
        fprintf(stderr,
            kENetDefaultErrorMessage);
        exit(EXIT_FAILURE);
    }

    ENetEvent event;
    ENetPeer* peer = ConnectToServer();

    if (peer == NULL)
    {
        fprintf(stderr,
            kENetNoPeerAvialable);
        exit(EXIT_FAILURE);
    }
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        std::cout << kENetConnectionSuccess << std::endl;
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(peer);
        std::cout << kENetConnectionFailure << std::endl;
    }

    while (enet_host_service(client, &event, 30000) > 0)
    {
        switch (event.type)
        { 
            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << event.packet->data << std::endl;
                enet_packet_destroy(event.packet); 

                SendPacket();
                break;
        }
    }

    if (client != nullptr)
        enet_host_destroy(client);

    return EXIT_SUCCESS;
}

bool CreateClient()
{
    client = enet_host_create(NULL /* create a client host */,
        1 /* only allow 1 outgoing connection */,
        2 /* allow up 2 channels to be used, 0 and 1 */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);

    return client != nullptr;
}

ENetPeer* ConnectToServer()
{
    ENetAddress address;
    /* Connect to some.server.net:1234. */
    enet_address_set_host(&address, "127.0.0.1");
    address.port = 1234;
    /* Initiate the connection, allocating the two channels 0 and 1. */
    return enet_host_connect(client, &address, 2, 0);
}

void UserInput(std::string message, std::function<bool(std::string)> condition)
{
    std::string input;
    bool exit = false;

    do
    {
        std::cout << message;
        getline(std::cin, input);

        if (std::cin.fail() || condition(input) == false)
        {
            std::cin.clear();
            std::cin.sync();
            //std::cin.ignore(INT_MAX, '\n');
            input = "";
        }
        else
        {
            exit = true;
        }
    } while (!exit);
}

void UserInput(std::string message, std::function<bool(std::string)> condition, std::string& storage)
{
    std::string input;
    bool exit = false;

    do
    {
        std::cout << message;
        getline(std::cin, input);

        if (std::cin.fail() || condition(input) == false)
        {
            std::cin.clear();
            std::cin.sync();
            //std::cin.ignore(INT_MAX, '\n');
            input = "";
        }
        else
        {
            storage = input;
            exit = true;
        }
    } while (!exit);
}

void SetupChatroomDisplay()
{
    std::cout << "Welcome to the chat room!" << std::endl;
}
 
void SendPacket()
{
    std::string message = "";
    std::string packetMessage = "";

    message = GetUsernameInputFormatted(clientUserName);

    UserInput(message, [](std::string input) { return input != ""; }, packetMessage);

    packetMessage = message + packetMessage;

    const char* sendMessage = packetMessage.c_str();

    /* Create a reliable packet of size 7 containing "packet\0" */
    ENetPacket* packet = enet_packet_create(sendMessage,
        strlen(sendMessage) + 1,
        ENET_PACKET_FLAG_RELIABLE);

    /* Extend the packet so and append the string "foo", so it now */
    /* contains "packetfoo\0"                                      */
    //enet_packet_resize(packet, strlen("packetfoo") + 1);
    //strcpy(&packet->data[strlen("packet")], "foo");
    /* Send the packet to the peer over channel id 0. */
    /* One could also broadcast the packet by         */
    /* enet_host_broadcast (host, 0, packet);         */

    enet_host_broadcast(client, 0, packet);

    /* One could just use enet_host_service() instead. */
    enet_host_flush(client);
}


std::string GetUsernameInputFormatted(std::string username)
{
    std::string message = "";

    message += "[" + username + "] >> ";

    return message;
}