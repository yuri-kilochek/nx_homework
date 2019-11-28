#include <nx/homework/hasher/microservice/server.hpp>
#include <nx/homework/hasher/microservice/session.hpp>

#include <boost/system/system_error.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/executor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/v6_only.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/dispatch.hpp>

#include <list>
#include <iterator>

namespace nx::homework::hasher::microservice {
///////////////////////////////////////////////////////////////////////////////

struct server::impl {
    boost::asio::strand<boost::asio::executor> strand;
    boost::asio::ip::tcp::acceptor acceptor;
    std::list<session> sessions;

    explicit
    impl(boost::asio::io_context& io_context, std::uint_least16_t port)
    : strand(io_context.get_executor())
    , acceptor(io_context)
    {
        auto protocol = boost::asio::ip::tcp::v6();
        acceptor.open(protocol);
        acceptor.set_option(boost::asio::socket_base::reuse_address{true});
        acceptor.set_option(boost::asio::ip::v6_only{false});
        acceptor.bind(boost::asio::ip::tcp::endpoint{protocol, port});
        acceptor.listen();
    }

    void accept_next(std::shared_ptr<impl> self) {
        acceptor.async_accept(bind_executor(strand, [
            this, self = std::move(self)
        ](auto ec, auto socket) mutable {
            if (ec == boost::asio::error::operation_aborted) { return; }
            if (ec) { throw boost::system::system_error(ec); }

            accept_next(self);

            auto& session = sessions.emplace_back(std::move(socket));
            session.async_wait(bind_executor(strand, [
                this, self = std::move(self),
                it = std::prev(sessions.end())
            ](auto ec) {
                if (ec && ec != boost::asio::error::operation_aborted) {
                    throw boost::system::system_error(ec);
                }

                sessions.erase(it);
            }));
        }));
    }

    void terminate() {
        acceptor.close();
        for (auto& session : sessions) {
            session.terminate();
        }
    }
};

server::server(boost::asio::io_context& io_context, std::uint_least16_t port)
: impl_(std::make_shared<impl>(io_context, port))
{ impl_->accept_next(impl_); }

server::~server() {
    terminate();
}

void server::terminate() {
    dispatch(impl_->strand, [impl_ = impl_]{
        impl_->terminate();
    });
}

///////////////////////////////////////////////////////////////////////////////
}
