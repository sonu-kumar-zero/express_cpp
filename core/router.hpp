#pragma once
#include "request.hpp"
#include "response.hpp"

#include <unordered_map>
#include <functional>

using Handler = std::function<void(Request &, Response &)>;

class Router
{
    std::unordered_map<std::string, Handler> get_routes_;

public:
    void get(const std::string &path, Handler h)
    {
        get_routes_[path] = std::move(h);
    }

    bool dispatch(const std::string &method,
                  const std::string &path,
                  Request &req,
                  Response &res)
    {
        if (method == "GET")
        {
            auto it = get_routes_.find(path);
            if (it != get_routes_.end())
            {
                it->second(req, res);
                return true;
            }
        }
        return false;
    }
};