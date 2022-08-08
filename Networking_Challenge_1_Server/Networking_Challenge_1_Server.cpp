#include <enet/enet.h>
#include <iostream>

ENetAddress address;
ENetHost* server;

bool CreateServer();
bool MenuSelection();

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

    while (MenuSelection())
    {

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

bool MenuSelection()
{
    int input = -1;

    std::cout << "\nEnter 0 to destroy this server: " << std::endl;
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
