#ifndef APP_CONTEXT_HPP
#define APP_CONTEXT_HPP

class AppContext
{
    private:

    const char *port;

    std::unordered_map<const char*, std::unordered_map<const char*, RequestHandler>> endpoint_mapping;

    public:

    AppContext() { }

    AppContext(const char *port, std::unordered_map<const char*, std::unordered_map<const char*, RequestHandler>> endpoint_mapping)
    {
        this->port = port;
        this->endpoint_mapping = endpoint_mapping;
    }

    const char *get_port()
    {
        return port;
    }

    RequestHandler get_handler(const char *url, const char *method)
    {
        if(endpoint_mapping.count(url) == 0)
        {
            return nullptr;
        }

        if(endpoint_mapping[url].count(method) == 0)
        {
            return nullptr;
        }

        return endpoint_mapping[url][method];
    }
};

#endif