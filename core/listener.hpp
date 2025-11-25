#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include "session.hpp"

namespace asio = boost::asio;
namespace beast = boost::beast;
using tcp = asio::ip::tcp;

class Listener : public std::enable_shared_from_this<Listener>
{
    asio::io_context &ioc_;
    tcp::acceptor acceptor_;
    Router &router_;

public:
    Listener(asio::io_context &ioc, tcp::endpoint endpoint, Router &router)
        : ioc_(ioc), acceptor_(asio::make_strand(ioc)), router_(router)
    {
        beast::error_code ec;

        acceptor_.open(endpoint.protocol(), ec);
        acceptor_.set_option(asio::socket_base::reuse_address(true), ec);
        acceptor_.bind(endpoint, ec);
        acceptor_.listen(asio::socket_base::max_listen_connections, ec);
    }

    void run() { do_accept(); }

private:
    void do_accept()
    {
        acceptor_.async_accept(
            asio::make_strand(ioc_),
            [self = shared_from_this()](beast::error_code ec, tcp::socket socket)
            {
                if (!ec)
                    std::make_shared<Session>(std::move(socket), self->router_)->start();
                self->do_accept();
            });
    }
};