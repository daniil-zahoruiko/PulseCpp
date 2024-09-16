#include "pulse/response.hpp"
#include "definitions.hpp"
#include <vector>
#include <string.h>
#include <iostream>

Response::Response()
{
    status = 200;

    content_type = "text/html;charset=UTF-8";

    content_length = 0;
    raw = nullptr;
}

Response::Response(int status, std::string data) : status(status)
{
    content_type = "text/html;charset=UTF-8";

    content_length = data.size();

    raw = new char[content_length + 1];
    data.copy(raw, content_length);
    raw[content_length] = '\0';
}

Response::Response(int status, std::string data, std::string content_type) : status(status), content_type(content_type)
{
    content_length = data.size();

    raw = new char[content_length];
    data.copy(raw, content_length);
}

const char* Response::build_response(int &length)
{
    std::vector<std::string> headers;

    headers.insert(headers.end(), build_header_string("Content-Type", content_type));
    headers.insert(headers.end(), build_header_string("Content-Length", content_length));
    headers.insert(headers.end(), build_header_string("Access-Control-Allow-Origin", "*"));

    std::string status_line = "HTTP/1.1" + std::string(1, SP_ASCII) + std::to_string(status) + CRLF;

    std::cout << status << '\n';

    int total_size = status_line.size();
    for(int i = 0; i < headers.size(); i++) total_size += headers[i].size();

    total_size += 2; // for CRLF at the end of headers

    total_size += content_length;

    char *res = new char[total_size];

    status_line.copy(res, status_line.size());
    int curr = status_line.size();
    for(int i = 0; i < headers.size(); i++)
    {
        headers[i].copy(res + curr, headers[i].size());
        curr += headers[i].size();
    }
    
    res[curr++] = CR_ASCII;
    res[curr++] = LF_ASCII;

    memcpy(res + curr, raw, content_length);
    length = total_size;
    return res;
}

std::string Response::build_header_string(std::string name, std::string value)
{
    return name + ": " + value + CRLF;
}

std::string Response::build_header_string(std::string name, int value)
{
    return build_header_string(name, std::to_string(value));
}

Response::~Response()
{
    if(raw)
    {
        delete[] raw;
    }
}