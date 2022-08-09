#include <enet/enet.h>
#include <iostream>
#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>

ENetAddress g_address;
ENetHost* g_client;
std::string g_clientUserName = "";
int g_clientUserNameInputLength = 0;
bool g_connectedToServer = false;

int g_currentLogXPos = 0;
int g_currentLogYPos = 0;

std::mutex g_consoleDraw;

std::queue<std::string> g_newLogsQueue;

const char* g_kENetDefaultErrorMessage = "An error occurred while trying to create an ENet client host.\n";
const char* g_kENetInitializingErrorMessage = "An error occurred while initializing ENet.\n";
const char* g_kENetNoPeerAvialable = "No available peers for initiating an ENet connection.\n";

const char* g_kENetConnectionSuccess = "Connection to 127.0.0.1:1234 succeeded.";
const char* g_kENetConnectionFailure = "Connection 127.0.0.1:1234 failed.";

const short g_kLogStartXPos = 0;
const short g_kLogStartYPos = 7;

bool CreateClient();
ENetPeer* ConnectToServer();

void UserInput(std::string, std::function<bool(std::string)>, std::string& storage);

void UserInputThread();
void LogQueueThread();

void SendPacket();
void RepositionInputCursor(bool initial);
void AddMessageToLog(std::string message);
void AddMessageToLogQueue(std::string message);

std::string GetUsernameInputFormatted(std::string username);
void SetupChatroomDisplay();

void EraseConsoleLine();

int main()
{
    std::cout << "Hello!  This program creates an ENet Client and waits for the user to exit before turning it off.\n\n";

    if (enet_initialize() != 0)
    {
        fprintf(stderr, g_kENetInitializingErrorMessage);
        std::cout << g_kENetInitializingErrorMessage << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    if (!CreateClient())
    {
        fprintf(stderr,
            g_kENetDefaultErrorMessage);
        exit(EXIT_FAILURE);
    }

    UserInput("Enter a name for the client user: ", [](std::string input) { return input != ""; }, g_clientUserName);

    if (!CreateClient())
    {
        fprintf(stderr,
            g_kENetDefaultErrorMessage);
        exit(EXIT_FAILURE);
    }

    ENetEvent event;
    ENetPeer* peer = ConnectToServer();

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

    SetupChatroomDisplay();
    std::thread inputThread(UserInputThread);
    std::thread logQueueThread(LogQueueThread);

    while (enet_host_service(g_client, &event, 1200000) > 0)
    {
        switch (event.type)
        { 
            case ENET_EVENT_TYPE_RECEIVE:
                AddMessageToLogQueue((char*)event.packet->data);
                enet_packet_destroy(event.packet); 

                break;
        }
    }

    if (g_client != nullptr)
        enet_host_destroy(g_client);

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

ENetPeer* ConnectToServer()
{
    ENetAddress address;
    /* Connect to some.server.net:1234. */
    enet_address_set_host(&address, "127.0.0.1");
    address.port = 1234;
    /* Initiate the connection, allocating the two channels 0 and 1. */
    return enet_host_connect(g_client, &address, 2, 0);
}

void UserInput(std::string message, std::function<bool(std::string)> condition, std::string& storage)
{
    std::string input;
    bool exit = false;

    do
    {
        std::cout << message;
        getline(std::cin, input);
        EraseConsoleLine();      

        if (std::cin.fail() || condition(input) == false)
        {
            std::cin.clear();
            std::cin.sync();
            //std::cin.ignore(INT_MAX, '\n');
            input = "";

            RepositionInputCursor(true);
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

void UserInputThread()
{
    while (true)
    {
        if (g_connectedToServer == true)
        {
            SendPacket();
        }
    }
}

void LogQueueThread()
{
    while (true)
    {
        if (g_connectedToServer == true && g_newLogsQueue.size() > 0)
        {
            std::lock_guard<std::mutex> consoleDrawGuard(g_consoleDraw);
            AddMessageToLog(g_newLogsQueue.front());
            g_newLogsQueue.pop();
        }
    }
}

void SetupChatroomDisplay()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(h, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(h, consoleMode);


    COORD c = { g_kLogStartXPos, g_kLogStartYPos };
    SetConsoleCursorPosition(h, c);

    g_currentLogXPos = g_kLogStartXPos;
    g_currentLogYPos = g_kLogStartYPos + 1;

    std::cout << "Welcome to the chat room!" << std::endl;
}
 
void SendPacket()
{
    RepositionInputCursor(true);

    std::string message = "";
    std::string packetMessage = "";

    message = GetUsernameInputFormatted(g_clientUserName);

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

    enet_host_broadcast(g_client, 0, packet);

    /* One could just use enet_host_service() instead. */
    enet_host_flush(g_client);
}

std::string GetUsernameInputFormatted(std::string username)
{
    std::string message = "";

    message += "[" + username + "] >> ";

    g_clientUserNameInputLength = message.length();

    return message;
}

void RepositionInputCursor(bool initial = false)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(h, &csbi);
    
    int bottomRow = csbi.srWindow.Bottom - 1;
    COORD c = { 0, 0 };

    if (initial)
    {
        c.X = 0;
        c.Y = bottomRow;
    }
    else
    {
        c.X = g_clientUserNameInputLength;
        c.Y = bottomRow;
    }
    
    SetConsoleCursorPosition(h, c);
}

void AddMessageToLogQueue(std::string message)
{
    g_newLogsQueue.push(message);
}

void AddMessageToLog(std::string message)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(h, &csbi);

    COORD c = { g_currentLogXPos, g_currentLogYPos };

    if ((csbi.srWindow.Bottom - 3) == g_currentLogYPos)
    {
        SMALL_RECT srctWindow;

        srctWindow.Top = csbi.srWindow.Top + 1;
        srctWindow.Bottom = csbi.srWindow.Bottom + 1;
        srctWindow.Left = 0;
        srctWindow.Right = csbi.srWindow.Right;

        SetConsoleWindowInfo(h, TRUE, &srctWindow);
        EraseConsoleLine();
    }

    SetConsoleCursorPosition(h, c);
    std::cout << message;
    g_currentLogYPos++;

    RepositionInputCursor(true);
    std::cout << GetUsernameInputFormatted(g_clientUserName);
}

void EraseConsoleLine()
{
    std::cout << "\33[2K";// Delete current line
    // i=1 because we included the first line
    std::cout
        << "\33[1A" // Move cursor up one
        << "\33[2K"; // Delete the entire line
    std::cout << "\r"; // Resume the cursor at beginning of line
}