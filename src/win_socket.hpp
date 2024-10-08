#ifndef WIN_SOCKET_HPP
#define WIN_SOCKET_HPP

#include "IServerSocket.hpp"
#include "request_parser.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdexcpt.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class WinSocket : public IServerSocket
{
    private:

    AppContext app_context;

    WSAData wsa_data;
    SOCKET sockfd;

    char *buffer;

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
        error = nullptr;

        int res = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
                                NULL,
                                errorCode,
                                0,
                                (LPTSTR)&error,
                                0,
                                NULL);
                      
        return res != 0 && error != nullptr;
    }

    public:

    WinSocket(AppContext app_context)
    {
        this->app_context = app_context;

        buffer = nullptr;
        error = nullptr;
        sockfd = INVALID_SOCKET;
        int iResult = 0;

        iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);

        addrinfo *server_info = NULL, hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        iResult = getaddrinfo(NULL, this->app_context.get_port(), &hints, &server_info);

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
                    std::cerr << "Could not bind to socket with address " << p->ai_addr << '\n' << error.get();
                }
                else
                {
                    std::cerr << "An unknown error occurred while binding a socket.";
                }
                std::cerr << "Skipping\n";
                continue;
            }

            this->sockfd = sockfd;
            break;
        }

        freeaddrinfo(server_info);

        if(p == NULL)
        {
            WSACleanup();
            throw std::runtime_error("Could not bind any socket\n");
        }
    }

    void wait_for_requests()
    {
        if(listen(sockfd, SOMAXCONN) == SOCKET_ERROR)
        {
            if(fetch_error(WSAGetLastError()))
            {
                throw std::runtime_error(error.get());
            }
            else
            {
                throw std::runtime_error("An unknown error occurred while placing the socket in the listening state.");
            }
        }

        SOCKET client_socket = INVALID_SOCKET;
        sockaddr_storage client_info;
        buffer = new char[DEFAULT_BUFFER_LENGTH];

        RequestParser parser = RequestParser(buffer, DEFAULT_BUFFER_LENGTH);
        int iResult = 0;
        while(true)
        {
            int client_info_size = sizeof(client_info);
            client_socket = accept(sockfd, (sockaddr*)&client_info, &client_info_size);
            if(client_socket == INVALID_SOCKET)
            {
                continue;
            }

            iResult = 0;
            bool parsed = false;
            do
            {
                iResult = recv(client_socket, buffer, DEFAULT_BUFFER_LENGTH, 0);
                parsed = parser.process_next_chunk(iResult);
            } while(iResult > 0 && !parsed);

            std::cout << "Done parsing\n";
            Request request = parser.build_request();
            Response response = app_context.try_get_handler_for_request(request)(request);

            int response_length = 0;
            const char *raw_response = response.build_response(response_length);
            int bytes_sent = 0;
            while(bytes_sent < response_length)
            {
                send(client_socket, raw_response + bytes_sent, std::min(response_length - bytes_sent, DEFAULT_BUFFER_LENGTH), 0);
                bytes_sent += std::min(response_length - bytes_sent, DEFAULT_BUFFER_LENGTH);
            }
            
            closesocket(client_socket);
            delete[] raw_response;
            parser.reset();
        }
    }

    ~WinSocket()
    {
        if(sockfd != INVALID_SOCKET)
        {
            closesocket(sockfd);
        }

        if(buffer)
        {
            delete[] buffer;
        }

        WSACleanup();
    }
};

#endif