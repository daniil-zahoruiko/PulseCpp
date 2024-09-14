#ifndef APP_CONTEXT_HPP
#define APP_CONTEXT_HPP

class AppContext
{
    private:

    const char *port;

    std::unordered_map<std::string, std::unordered_map<std::string, RequestHandler>> endpoint_mapping;

    public:

    AppContext() { }

    AppContext(const char *port, std::unordered_map<std::string, std::unordered_map<std::string, RequestHandler>> endpoint_mapping)
    {
        this->port = port;
        this->endpoint_mapping = endpoint_mapping;
    }

    const char *get_port()
    {
        return port;
    }

    RequestHandler try_get_handler_for_request(Request request)
    {
        if(endpoint_mapping.count(request.url) == 0)
        {
            return nullptr;
        }

        if(endpoint_mapping[request.url].count(request.method) == 0)
        {
            return nullptr;
        }

        return endpoint_mapping[request.url][request.method];
    }
};

#endif