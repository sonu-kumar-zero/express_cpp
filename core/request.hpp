#pragma once
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace http = boost::beast::http;
using json = nlohmann::json;

struct Request
{
    http::request<http::string_body> req;

    std::string body;
    json jsonBody;
    bool isJson = false;

    void parse()
    {
        // save raw body string
        body = req.body();

        // check if JSON
        auto ct = req[http::field::content_type];
        if (!ct.empty() && ct.find("application/json") != std::string::npos)
        {
            try
            {
                jsonBody = json::parse(body);
                isJson = true;
            }
            catch (...)
            {
                // malformed JSON
                isJson = false;
            }
        }
    }
};