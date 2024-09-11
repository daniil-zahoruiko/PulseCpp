#ifndef SOCKET_FACTORY_HPP
#define SOCKET_FACTORY_HPP

#include "ISocket.hpp"
#include "app_context.hpp"
#include "win_socket.hpp"
#include "unix_socket.hpp"

class SocketFactory
{
    public:

    static std::unique_ptr<IServerSocket> create_server_socket(AppContext app_context)
    {
        try
        {
            #if _WIN32
            return std::make_unique<WinSocket>(app_context);
            #elif __unix__ 
            return std::make_unique<UnixSocket>(app_context);
            #elif __APPLE__
            return std::make_unique<UnixSocket>(app_context);
            #endif
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << e.what();
            return nullptr;
        }
        
    }
};

#endif