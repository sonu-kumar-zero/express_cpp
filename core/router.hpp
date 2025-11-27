#pragma once
#include "request.hpp"
#include "response.hpp"

#include <unordered_map>
#include <functional>
#include <set>

using Handler = std::function<void(Request &, Response &)>;

class Router
{
    std::unordered_map<std::string, Handler> get_routes_;
    std::unordered_map<std::string, Handler> post_routes_;
    std::unordered_map<std::string, Handler> put_routes_;
    std::unordered_map<std::string, Handler> delete_routes_;

public:
    void get(const std::string &path, Handler h)
    {
        get_routes_[path] = std::move(h);
    }

    void post(const std::string &path, Handler h)
    {
        post_routes_[path] = std::move(h);
    }

    void put(const std::string &path, Handler h)
    {
        put_routes_[path] = std::move(h);
    };

    void del(const std::string &path, Handler h)
    {
        delete_routes_[path] = std::move(h);
    }

    bool dispatch(const std::string &method,
                  const std::string &path,
                  Request &req,
                  Response &res)
    {
        if (method == "HEAD")
        {
            auto it = get_routes_.find(path);
            if (it != get_routes_.end())
            {
                it->second(req, res);
                res.clearBody(); // remove content for HEAD
                return true;
            }
        }

        if (method == "GET")
        {
            auto it = get_routes_.find(path);
            if (it != get_routes_.end())
            {
                it->second(req, res);
                return true;
            }
        }
        else if (method == "POST")
        {
            auto it = post_routes_.find(path);
            if (it != post_routes_.end())
            {
                it->second(req, res);
                return true;
            }
        }
        else if (method == "PUT")
        {
            auto it = put_routes_.find(path);
            if (it != put_routes_.end())
            {
                it->second(req, res);
                return true;
            }
        }
        else if (method == "DELETE")
        {
            auto it = delete_routes_.find(path);
            if (it != delete_routes_.end())
            {
                it->second(req, res);
                return true;
            }
        }
        else if (method == "OPTIONS")
        {
            std::set<std::string> allowed;

            if (get_routes_.count(path))
            {
                allowed.insert("GET");
                allowed.insert("HEAD");
            }
            if (post_routes_.count(path))
                allowed.insert("POST");
            if (put_routes_.count(path))
                allowed.insert("PUT");
            if (delete_routes_.count(path))
                allowed.insert("DELETE");

            allowed.insert("OPTIONS"); // always allowed

            // Build Allow header
            std::string allow;
            for (auto &m : allowed)
            {
                if (!allow.empty())
                    allow += ", ";
                allow += m;
            }

            res.setStatus(204);
            res.setHeader("Allow", allow);

            // CORS Support
            res.setHeader("Access-Control-Allow-Origin", "*");
            res.setHeader("Access-Control-Allow-Methods", allow);
            res.setHeader("Access-Control-Allow-Headers", "Content-Type");
            return true;
        }

        return false;
    }
};