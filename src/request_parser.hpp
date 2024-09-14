#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include <cstring>
#include <string>
#include <iostream>

#define DEFAULT_BUFFER_LENGTH 512
#define CR_ASCII 13
#define LF_ASCII 10
#define SP_ASCII 32

class RequestParser
{
    private:

    const int MAX_REQUEST_LINE_LEN = 8192;  // recommended by HTTP 1.1 spec
    const int MAX_HEADER_LEN = 16384;   // a reasonable limit on header length

    const std::string CRLF{ CR_ASCII, LF_ASCII };
    const std::string CONTENT_LENGTH_HEADER_NAME = "Content-Length: ";

    char *const socket_buffer;
    const int socket_buffer_length;

    char *request_line_buffer, *header_buffer, *content_buffer;
    int request_line_length, header_length, content_length, content_length_expected;

    bool is_request_line_received, is_headers_received;

    bool check_previous_with_chunk_concat(char *buffer, int buffer_len, char target, int ind, int shift)
    {
        return (ind >= shift && socket_buffer[ind - shift] == target) || (ind < shift && buffer_len >= shift && buffer[buffer_len - shift] == target);
    }

    bool check_if_done()
    {
        return is_request_line_received && is_headers_received && content_length >= content_length_expected;
    }

    public:

    RequestParser(char *&buffer, int buffer_length) : socket_buffer(buffer), socket_buffer_length(buffer_length)
    {
        request_line_buffer = new char[MAX_REQUEST_LINE_LEN];
        header_buffer = new char[MAX_HEADER_LEN];
        content_buffer = nullptr;

        reset();
    }

    bool process_next_chunk(int len)
    {
        if(is_request_line_received && is_headers_received)
        {
            memcpy(content_buffer + content_length, socket_buffer, len);
            content_length += len;
            return check_if_done();
        }

        int curr = 0, bytes_cnt = std::min(len, socket_buffer_length);
        for(int i = 0; i < bytes_cnt; i++)
        {
            if(!is_request_line_received)
            {
                if(socket_buffer[i] == LF_ASCII && check_previous_with_chunk_concat(request_line_buffer, request_line_length, CR_ASCII, i, 1))
                {
                    memcpy(request_line_buffer + request_line_length, socket_buffer, i + 1);
                    is_request_line_received = true;
                    curr = i + 1;
                    request_line_length += curr;
                }
            }
            else if(!is_headers_received)
            {
                if(socket_buffer[i] == LF_ASCII && check_previous_with_chunk_concat(header_buffer, header_length, CR_ASCII, i, 1) 
                                                && check_previous_with_chunk_concat(header_buffer, header_length, LF_ASCII, i, 2) 
                                                && check_previous_with_chunk_concat(header_buffer, header_length, CR_ASCII, i, 3))
                {
                    memcpy(header_buffer + header_length, socket_buffer + curr, i + 1 - curr);
                    is_headers_received = true;
                    header_length += (i + 1 - curr);
                    curr = i + 1;

                    std::string header_str = (std::string)header_buffer;
                    std::size_t content_length_header_pos = header_str.find(CONTENT_LENGTH_HEADER_NAME);
                    if(content_length_header_pos == std::string::npos)
                    {
                        return true;
                    }

                    content_length_expected = std::stoi(header_buffer + content_length_header_pos + CONTENT_LENGTH_HEADER_NAME.size());

                    content_buffer = new char[content_length_expected];
                }
            }
        }

        if(curr < bytes_cnt)
        {
            if(!is_request_line_received)
            {
                memcpy(request_line_buffer + request_line_length, socket_buffer + curr, bytes_cnt - curr);
                request_line_length += bytes_cnt - curr;
            }
            else if(!is_headers_received)
            {
                memcpy(header_buffer + header_length, socket_buffer + curr, bytes_cnt - curr);
                header_length += bytes_cnt - curr;
            }
            else
            {
                memcpy(content_buffer, socket_buffer + curr, bytes_cnt - curr);
                content_length += bytes_cnt - curr;
            }
        }

        return check_if_done();
    }

    Request build_request()
    {
        std::string request_line_str = (std::string)request_line_buffer;

        int sp1 = request_line_str.find_first_of(' ');
        int sp2 = request_line_str.find_first_of(' ', sp1 + 1);

        std::string method = request_line_str.substr(0, sp1);
        std::string url = request_line_str.substr(sp1 + 1, sp2 - sp1 - 1);
        std::string version = request_line_str.substr(sp2 + 1);

        return Request(url, method, content_buffer);
    }

    void reset()
    {
        memset(request_line_buffer, 0, MAX_REQUEST_LINE_LEN);
        memset(header_buffer, 0, MAX_HEADER_LEN);

        request_line_length = 0;
        header_length = 0;
        content_length = 0;
        content_length_expected = 0;

        is_request_line_received = false;
        is_headers_received = false;

        if(content_buffer)
        {
            delete[] content_buffer;
            content_buffer = nullptr;
        }
    }

    ~RequestParser()
    {
        delete[] request_line_buffer;
        delete[] header_buffer;

        if(content_buffer)
        {
            delete [] content_buffer;
        }
    }
};

#endif