#include <iostream>
#include "Client.h"

int main()
{
    try {
        std::cout << "Starting client" << std::endl;
        Client* client = new Client();
        client->run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error while running client: " << e.what() << std::endl;
        exit(1);
    }
}
