#include <iostream>
#include "core/app.hpp"
#include "core/logger.hpp"

int main()
{
    try
    {
        App app;
        Logger logger;

        app.get("/",
                [&logger](Request &req, Response &res)
                {
                    json data;
                    data["id"] = 123;
                    data["name"] = "zero";
                    logger.log("This is a log test.");
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

        app.post("/login",
                 [](Request &req, Response &res)
                 {
                     if (!req.isJson)
                     {
                         return res.send("Expected application/json", 400);
                     }

                     std::string username = req.jsonBody["username"];
                     std::string password = req.jsonBody["password"];

                     res.json(
                         {{"ok", true},
                          {"user", username}});
                 });

        app.get("/search",
                [](Request &req, Response &res)
                {
                    std::string name = req.query["name"];
                    std::string age = req.query["age"];

                    res.json({{"name", name},
                              {"age", age}});
                });

        app.del("/search",
                [](Request &req, Response &res)
                {
                    res.send("This is a delete route");
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