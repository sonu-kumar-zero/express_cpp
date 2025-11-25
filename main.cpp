#include <iostream>
#include "core/app.hpp"

int main()
{
    try
    {
        App app;

        app.get("/", [](Request &req, Response &res)
                {
            res.res.version(11);
            res.res.set(boost::beast::http::field::content_type,"text/plain");
                    res.send("Hello from modular C++ Framework!"); });

        std::cout << "Listening on 0.0.0.0:8080\n";
        app.listen("0.0.0.0", 8080);
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Fatal: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }
}