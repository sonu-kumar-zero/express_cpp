#pragma once
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

struct Request
{
    http::request<http::string_body> req;
};