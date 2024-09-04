#include "pulse/app.hpp"
#include "socket_factory.hpp"
#include <iostream>

void App::start(const char *port)
{
    std::unique_ptr<IServerSocket> socket = SocketFactory::create_server_socket(port);

    if(socket == nullptr)
    {
        std::cout << "Could not create a socket, aborting...\n";
        return;
    }

    std::cout << "App is running on port " << port << '\n';
}