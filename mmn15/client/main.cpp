#include <iostream>
#include "Client.h"

int main()
{
    std::cout << "Starting client" << std::endl;
    Client* client = new Client();
    client->run();
}
