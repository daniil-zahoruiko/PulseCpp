#include "ISocket.hpp"
#include "win_socket.hpp"
#include "unix_socket.hpp"

class SocketFactory
{
    public:

    static std::unique_ptr<IServerSocket> create_server_socket(const char* port)
    {
        try
        {
            #if _WIN32
            return std::make_unique<WinSocket>(port);
            #elif __unix__ 
            return std::make_unique<UnixSocket>(port);
            #elif __APPLE__
            return std::make_unique<UnixSocket>(port);
            #endif
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << e.what();
            return nullptr;
        }
        
    }
};