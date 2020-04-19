#include "client.h"

int main()
{
    using namespace client_ns;
    Client *client = new Client;
    if (client)
        client->run();
}