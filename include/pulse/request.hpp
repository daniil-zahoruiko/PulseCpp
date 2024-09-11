#ifndef REQUEST_HPP
#define REQUEST_HPP

class Request
{
    public:

    const char *url;
    const char *method;

    Request(const char *url, const char *method) : url(url), method(method) {}
};

#endif