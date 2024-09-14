#ifndef APP_HPP
#define APP_HPP

#include "response.hpp"
#include "request.hpp"
#include <unordered_map>
#include <string>

typedef Response(*RequestHandler)(Request);

class App
{
    private:

    std::unordered_map<std::string, std::unordered_map<std::string, RequestHandler>> endpoint_mapping;

    public:

    void start(const char *port);

    void register_endpoint(std::string url, std::string method, Response (*action_function)(Request));
};

#endif