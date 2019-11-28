#ifndef NX_HOMEWORK_HASHER_MICROSERVICE_DETAIL_INCLUDE_GUARD_NX_HOMEWORK_HASHER_MICROSERVICE_SERVER
#define NX_HOMEWORK_HASHER_MICROSERVICE_DETAIL_INCLUDE_GUARD_NX_HOMEWORK_HASHER_MICROSERVICE_SERVER

#include <boost/asio/io_context.hpp>

#include <cstdint>
#include <memory>

namespace nx::homework::hasher::microservice {
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
