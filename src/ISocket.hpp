#ifndef ISOCKET_HPP
#define ISOCKET_HPP

#include <memory>

class IServerSocket
{
    public:
        virtual ~IServerSocket() {};

        virtual void open_socket() = 0;
};

#endif