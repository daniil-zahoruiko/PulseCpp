#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#define DEFAULT_BUFFER_LENGTH 512

class RequestParser
{
    private:

    const int MAX_REQUEST_LINE_LEN = 8192;  // recommended by HTTP 1.1 spec
    const int MAX_HEADER_LEN = 16384;   // a reasonable limit on header length

    char *const socket_buffer;
    const int socket_buffer_length;

    char *request_line_buffer;
    char *header_buffer;

    public:


    RequestParser(char *&buffer, int buffer_length) : socket_buffer(buffer), socket_buffer_length(buffer_length)
    {
        request_line_buffer = new char[MAX_REQUEST_LINE_LEN];
        header_buffer = new char[MAX_HEADER_LEN];
    }

    void process_next_chunk(int len)
    {

    }

    Request build_request()
    {
        
    }

    ~RequestParser()
    {
        delete[] request_line_buffer;
        delete[] header_buffer;
    }
};

#endif