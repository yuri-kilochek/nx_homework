#ifndef NX_HOMEWORK_HASHER_MICROSERVICE_SESSION_INCLUDED
#define NX_HOMEWORK_HASHER_MICROSERVICE_SESSION_INCLUDED

#include <nx_homework/hasher_microservice/any_handler.hpp>

#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/async_result.hpp>

#include <utility>
#include <memory>

namespace nx_homework::hasher_microservice {
///////////////////////////////////////////////////////////////////////////////

struct session {
    explicit
    session(boost::asio::ip::tcp::socket&& socket);

    session(session const&)
    = delete;

    auto operator=(session const&)
    -> session&
    = delete;

    ~session();

    void terminate();

    template <typename CompletionToken>
    auto async_wait(CompletionToken&& token) {
        return boost::asio::async_initiate<
            CompletionToken, void(boost::system::error_code)>(
                [this](auto&& handler) {
                    async_wait_impl(std::forward<decltype(handler)>(handler));
                }, token);
    }

private:
    struct impl;
    std::shared_ptr<impl> impl_;

    void async_wait_impl(any_handler<boost::system::error_code> handler);
};

///////////////////////////////////////////////////////////////////////////////
}

#endif
