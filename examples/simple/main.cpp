#include "pulse/app.hpp"

Response test_endpoint(Request r)
{
    return Response();
}

int main()
{
    App app = App();

    app.register_endpoint("/example", "POST", &test_endpoint);

    app.start("12345");

}