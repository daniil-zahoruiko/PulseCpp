#ifndef WIN_SOCKET_HPP
#define WIN_SOCKET_HPP

#include "ISocket.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdexcpt.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class WinSocket : public IServerSocket
{
    private:

    WSAData wsa_data;

    struct error_deleter
    {
        void operator()(char *ptr) const 
        {
            if(ptr)
            {
                LocalFree(ptr);
            }
        }
    };

    std::unique_ptr<char, error_deleter> error;

    bool fetch_error(int errorCode)
    {
        if(error)
        {
            error.release();
        }

        int res = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
                                NULL,
                                errorCode,
                                0,
                                (LPTSTR)&error,
                                0,
                                NULL);
                      
        return res != 0;
    }

    public:

    WinSocket(const char *port)
    {
        error = nullptr;
        int iResult = 0;

        iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);

        addrinfo *server_info = NULL, hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        iResult = getaddrinfo(NULL, port, &hints, &server_info);

        if(iResult != 0)
        {
            WSACleanup();
            if(fetch_error(iResult))
            {
                throw std::runtime_error(error.get());
            }
            else
            {
                throw std::runtime_error("An unknown error occurred while obtaining server information.\n");
            }
        }

        addrinfo *p = NULL;
        for(p = server_info; p != NULL; p = p->ai_next)
        {
            SOCKET sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if(sockfd == INVALID_SOCKET)
            {
                if(fetch_error(WSAGetLastError()))
                {
                    std::cerr << "Could not create a socket\n" << error.get();
                }
                else
                {
                    std::cerr << "An unknown error occurred while creating a socket.";
                }
                std::cerr << "Skipping...\n";
                continue;
            }

            iResult = bind(sockfd, p->ai_addr, p->ai_addrlen);
            if(iResult == SOCKET_ERROR)
            {
                if(fetch_error(WSAGetLastError()))
                {
                    std::cerr << "Could not bind to socket with address " << p->ai_addr << '\n' << *error;
                }
                else
                {
                    std::cerr << "An unknown error occurred while binding a socket.";
                }
                std::cerr << "Skipping\n";
                continue;
            }

            break;
        }

        freeaddrinfo(server_info);

        if(p == NULL)
        {
            WSACleanup();
            throw std::runtime_error("Could not bind any socket\n");
        }
    }

    void open_socket()
    {

    }

    ~WinSocket()
    {
        WSACleanup();
    }
};

#endif