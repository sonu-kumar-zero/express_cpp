#pragma once
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace http = boost::beast::http;
using json = nlohmann::json;

struct Request
{
    http::request<http::string_body> req;

    std::string body;
    json jsonBody;
    bool isJson = false;

    // for query parmeters
    std::unordered_map<std::string, std::string> query;

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

        // --- Parse query parameters ---
        parseQuery();
    }

private:
    void parseQuery()
    {
        std::string target = std::string(req.target());
        auto pos = target.find('?');
        if (pos == std::string::npos)
            return;

        std::string qs = target.substr(pos + 1);
        while (!qs.empty())
        {
            auto amp = qs.find('&');
            std::string pair = (amp == std::string::npos) ? qs : qs.substr(0, amp);

            auto eq = pair.find('=');
            if (eq != std::string::npos)
            {
                std::string key = pair.substr(0, eq);
                std::string val = pair.substr(eq + 1);
                query[key] = val;
            };

            if (amp == std::string::npos)
                break;
            qs.erase(0, amp + 1);
        }
    }
};