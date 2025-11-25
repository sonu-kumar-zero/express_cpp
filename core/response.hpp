#pragma once
#include <boost/beast/http.hpp>
#include <string>

namespace http = boost::beast::http;

struct Response
{
    http::response<http::string_body> res;

    void send(const std::string &body, unsigned status = 200)
    {
        res.result((http::status)status);
        res.body() = body;
        res.prepare_payload();
    }
};