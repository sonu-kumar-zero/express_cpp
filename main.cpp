#include <iostream>
#include "core/app.hpp"

int main()
{
    try
    {
        App app;

        app.get("/",
                [](Request &req, Response &res)
                {
                    json data;
                    data["id"] = 123;
                    data["name"] = "Sonu Kumar";
                    res.json(data, 201);
                });

        app.get("/hello",
                [](Request &req, Response &res)
                {
                    res.send("GET Hello!");
                });

        app.post("/submit",
                 [](Request &req, Response &res)
                 {
                     res.json({{"status", "success"},
                               {"received", "something for now"}});
                 });

        std::cout << "Listening on 0.0.0.0:8080\n";
        app.listen("0.0.0.0", 8080);
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Fatal: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }
}