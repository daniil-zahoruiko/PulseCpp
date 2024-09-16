#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>

class Response
{
    private:

    char *raw;
    std::string content_type;
    int status, content_length;

    std::string build_header_string(std::string name, std::string value);
    std::string build_header_string(std::string name, int value);

    public:

    Response();

    Response(int status, std::string data);

    Response(int status, std::string data, std::string content_type);

    ~Response();

    const char *build_response(int &length);
};

#endif