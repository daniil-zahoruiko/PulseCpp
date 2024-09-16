#include "pulse/app.hpp"
#include <iostream>

Response test_endpoint(Request r)
{
    std::cout << "User Endpoint\n";
    return Response(200, "Test response");
}

int main()
{
    App app = App();

    app.register_endpoint("/example", "POST", &test_endpoint);

    app.start("12345");

}