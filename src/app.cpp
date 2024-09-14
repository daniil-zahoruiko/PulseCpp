#include "pulse/app.hpp"
#include "socket_factory.hpp"
#include <iostream>

void App::start(const char *port)
{
    AppContext app_context = AppContext(port, endpoint_mapping);
    std::unique_ptr<IServerSocket> socket = SocketFactory::create_server_socket(app_context);

    if(socket == nullptr)
    {
        std::cout << "Could not create a socket, aborting...\n";
        return;
    }

    std::cout << "App is running on port " << port << '\n';

    (*socket).wait_for_requests();
}

void App::register_endpoint(std::string url, std::string method, RequestHandler action_function)
{
    if(endpoint_mapping.count(url) == 0)
    {
        endpoint_mapping.insert({url, std::unordered_map<std::string, RequestHandler>()});
    }

    endpoint_mapping[url].insert({method, action_function});
}