#include <enet/enet.h>
#include <iostream>
#include <string>
#include <functional>
#include <thread>

ENetAddress address;
ENetHost* server;
std::string serverUsername = "";
int ConnectedToClient = 0;

const char* kEnterServerUserName = "Enter a name for the server user: ";

bool CreateServer();

void UserInput(std::string, std::function<bool(std::string)>, std::string &storage);
void UserInputThread();

void DisplayConnectionMessage(ENetEvent);
void DisplayDisconnectMessage(ENetEvent);

std::string GetUsernameInputFormatted(std::string username);
void SendPacket(std::string message);
void SetupChatroomDisplay();

int main()
{
    std::cout << "Hello!  This program creates an ENet Server and waits for the user to exit before turning it off.\n\n";

    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        std::cout << "An error occurred while initializing ENet.\n" << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    if (!CreateServer())
    {
        fprintf(stderr,
            "An error occurred while trying to create an ENet server host.\n");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server startup complete!  Waiting for clients to connect. . .\n";

    ENetEvent event;
    while (enet_host_service(server, &event, 1200000) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
                DisplayConnectionMessage(event);
                //event.peer->data = (void*)("Client information");
                ConnectedToClient++;

                break;
            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << event.packet->data << std::endl;
                SendPacket((char*)event.packet->data);
                enet_packet_destroy(event.packet);

                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                DisplayDisconnectMessage(event);
                ConnectedToClient--;
                break;
        }
    }

    if (server != nullptr)
        enet_host_destroy(server);

    return EXIT_SUCCESS;
}

bool CreateServer()
{
    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    address.host = ENET_HOST_ANY;
    /* Bind the server to port 1234. */
    address.port = 1234;
    server = enet_host_create(&address /* the address to bind the server host to */,
        32      /* allow up to 32 clients and/or outgoing connections */,
        2      /* allow up to 2 channels to be used, 0 and 1 */,
        0      /* assume any amount of incoming bandwidth */,
        0      /* assume any amount of outgoing bandwidth */);



    return server != nullptr;
}

void UserInput(std::string message, std::function<bool(std::string)> condition, std::string &storage)
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

void DisplayConnectionMessage(ENetEvent event)
{
    std::cout << "A new client connected from "
        << event.peer->address.host
        << ":" << event.peer->address.port
        << "\n";
}

void DisplayDisconnectMessage(ENetEvent event)
{
    std::cout << "The client from "
        << event.peer->address.host
        << ":" << event.peer->address.port
        << " has disconnected.\n";
}

std::string GetUsernameInputFormatted(std::string username)
{
    std::string message = "";

    message += "[" + username + "] >> ";

    return message;
}

void SendPacket(std::string message)
{
    /*std::string message = "";
    std::string packetMessage = "";

    message = GetUsernameInputFormatted(serverUsername);

    UserInput(message, [](std::string input) { return input != ""; }, packetMessage);

    packetMessage = message + packetMessage;

    const char* sendMessage = packetMessage.c_str();*/

    /* Create a reliable packet of size 7 containing "packet\0" */
    ENetPacket* packet = enet_packet_create(message.c_str(),
        strlen(message.c_str()) + 1,
        ENET_PACKET_FLAG_RELIABLE);

    /* Extend the packet so and append the string "foo", so it now */
    /* contains "packetfoo\0"                                      */
    //enet_packet_resize(packet, strlen("packetfoo") + 1);
    //strcpy(&packet->data[strlen("packet")], "foo");
    /* Send the packet to the peer over channel id 0. */
    /* One could also broadcast the packet by         */
    /* enet_host_broadcast (host, 0, packet);         */

    enet_host_broadcast(server, 0, packet);

    /* One could just use enet_host_service() instead. */
    enet_host_flush(server);
}

void SetupChatroomDisplay()
{
    std::cout << "Welcome to the chat room!" << std::endl;
}