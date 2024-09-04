#ifndef UNIX_SOCKET_HPP
#define UNIX_SOCKET_HPP

#include "ISocket.hpp"
#include <stdexcpt.h>
#include <iostream>

class UnixSocket : public IServerSocket
{
    public:
    UnixSocket(const char *port)
    {

    }

    void open_socket()
    {
        
    }

    ~UnixSocket()
    {

    }
};

#endif