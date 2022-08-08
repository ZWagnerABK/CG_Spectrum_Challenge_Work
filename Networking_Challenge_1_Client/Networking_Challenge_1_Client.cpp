#include <enet/enet.h>
#include <iostream>

ENetAddress address;
ENetHost* client;

bool CreateClient();
bool MenuSelection();

int main()
{
    std::cout << "Hello!  This program creates an ENet Client and waits for the user to exit before turning it off.\n\n";

    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        std::cout << "An error occurred while initializing ENet.\n" << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    if (!CreateClient())
    {
        fprintf(stderr,
            "An error occurred while trying to create an ENet client host.\n");
        exit(EXIT_FAILURE);
    }

    while (MenuSelection())
    {

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

bool MenuSelection()
{
    int input = -1;

    std::cout << "\nEnter 0 to destroy this client: " << std::endl;
    std::cin >> input;

    if (std::cin.fail() || input != 0)
    {
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        input = -1;

        return true;
    }
    else
    {
        return false;
    }
}