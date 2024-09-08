#ifndef APP_HPP
#define APP_HPP

#include "response.hpp"
#include "request.hpp"
#include <unordered_map>

typedef Response(*RequestHandler)(Request);

class App
{
    private:

    std::unordered_map<const char*, std::unordered_map<const char*, RequestHandler>> endpoint_mapping;

    public:

    void start(const char *port);

    void register_endpoint(const char *url, const char *method, Response (*action_function)(Request));
};

#endif