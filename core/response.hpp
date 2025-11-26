#pragma once
#include <boost/beast/http.hpp>
#include <string>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

struct Response
{
    http::response<http::string_body> res;

    Response()
    {
        // Default headers
        res.version(11);
        res.keep_alive(false);
    }

    void setStatus(unsigned status)
    {
        res.result((http::status)status);
    }

    void send(const std::string &body, unsigned status = 200)
    {
        setStatus(status);
        res.set(http::field::content_type, "text/plain; charset=utf-8");
        res.body() = body;
        res.prepare_payload();
    }

    void json(const json &obj, unsigned status = 200)
    {
        setStatus(status);
        res.set(http::field::content_type, "application/json");
        res.body() = obj.dump(); // convert JSON -> string
        res.prepare_payload();
    }
};