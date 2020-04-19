#include "server.h"

int main()
{
    using namespace server_ns;
    Server *server = new Server;
    if (server)
        server->run();

    return 0;
}