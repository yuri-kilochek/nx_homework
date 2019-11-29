#ifndef NX_HOMEWORK_HASHER_MICROSERVICE_SERVER_INCLUDED
#define NX_HOMEWORK_HASHER_MICROSERVICE_SERVER_INCLUDED

#include <boost/asio/io_context.hpp>

#include <cstdint>
#include <memory>

namespace nx_homework::hasher_microservice {
///////////////////////////////////////////////////////////////////////////////

struct server {
    explicit
    server(boost::asio::io_context& io_context, std::uint_least16_t port);

    server(server const&)
    = delete;

    auto operator=(server const&)
    -> server&
    = delete;

    ~server();

    void terminate();

private:
    struct impl;
    std::shared_ptr<impl> impl_;
};

///////////////////////////////////////////////////////////////////////////////
}

#endif
