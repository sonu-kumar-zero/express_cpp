#pragma once

#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <memory>

#include "router.hpp"
#include "listener.hpp"
#include "logger.hpp"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class App
{
    asio::io_context ioc_;
    Router router_;
    std::vector<std::thread> workers_;
    std::shared_ptr<Listener> listener_;
    Logger logger_;

public:
    App(unsigned threads = std::thread::hardware_concurrency())
        : ioc_(threads), router_(logger_) {}

    void get(const std::string &path, Handler h)
    {
        router_.get(path, std::move(h));
    }

    void post(const std::string &path, Handler h)
    {
        router_.post(path, std::move(h));
    }

    void put(const std::string &path, Handler h)
    {
        router_.put(path, std::move(h));
    }

    void del(const std::string &path, Handler h)
    {
        router_.del(path, std::move(h));
    }

    void listen(const std::string &address, unsigned short port)
    {
        tcp::endpoint ep(asio::ip::make_address(address), port);

        listener_ = std::make_shared<Listener>(ioc_, ep, router_, logger_);
        listener_->run();

        logger_.log("Server is Starting...");

        unsigned n = std::max(1u, (unsigned)std::thread::hardware_concurrency());
        for (unsigned i = 0; i < n; i++)
            workers_
                .emplace_back(
                    [this]()
                    {
                        ioc_.run();
                    });

        for (auto &t : workers_)
            if (t.joinable())
                t.join();
    }
};