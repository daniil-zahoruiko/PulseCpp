#ifndef UNIX_SOCKET_HPP
#define UNIX_SOCKET_HPP

#include "IServerSocket.hpp"
#include <stdexcpt.h>
#include <iostream>

class UnixSocket : public IServerSocket
{
    public:
    UnixSocket(AppContext app_context)
    {
        
    }

    void wait_for_requests()
    {

    }

    ~UnixSocket()
    {

    }
};

#endif