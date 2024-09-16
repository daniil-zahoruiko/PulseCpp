#ifndef ISOCKET_HPP
#define ISOCKET_HPP

#include <memory>

class IServerSocket
{
    public:
        virtual ~IServerSocket() {};

        virtual void wait_for_requests() = 0;
};

#endif