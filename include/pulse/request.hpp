#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

struct Request
{
    std::string url;
    std::string method;

    const char *raw;

    Request(std::string url, std::string method, const char *raw) : url(url), method(method), raw(raw) {}
};

#endif