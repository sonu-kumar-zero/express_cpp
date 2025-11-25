#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "router.hpp"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    Router &router_;
    http::request<http::string_body> req_;

public:
    explicit Session(tcp::socket sock, Router &router)
        : socket_(std::move(sock)), router_(router)
    {
        socket_.set_option(tcp::no_delay(true));
    }

    void start()
    {
        read_request();
    };

private:
    void read_request()
    {
        auto self = shared_from_this();
        http::async_read(socket_, buffer_, req_,
                         [this, self](beast::error_code ec, std::size_t)
                         {
                             if (ec == http::error::end_of_stream)
                                 return do_close();
                             if (ec)
                                 return;

                             handle_request();
                         });
    }

    void handle_request()
    {
        Request request{std::move(req_)};
        Response response;

        response.res = http::response<http::string_body>(
            http::status::not_found, 11);
        response.res.set(http::field::server, "cpp-express");
        response.res.set(http::field::content_type, "text/plain");

        bool ok = router_.dispatch(
            std::string(request.req.method_string()),
            std::string(request.req.target()),
            request,
            response);

        if (!ok)
            response.send("Not Found", 404);

        write_response(std::move(response.res));
    };

    void write_response(http::response<http::string_body> &&res)
    {
        auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
        auto self = shared_from_this();

        http::async_write(socket_, *sp,
                          [this, self, sp](beast::error_code ec, std::size_t)
                          {
                              if (ec)
                                  return;

                              if (sp->need_eof())
                                  return do_close();

                              req_ = {};
                              read_request();
                          });
    }

    void do_close()
    {
        beast::error_code ec;
        socket_.shutdown(tcp::socket::shutdown_send, ec);
    }
};