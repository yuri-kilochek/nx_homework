#include <nx/homework/hasher/microservice/server.hpp>

#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/v6_only.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/dispatch.hpp>

namespace nx::homework::hasher::microservice {
///////////////////////////////////////////////////////////////////////////////

struct server::impl {
    boost::asio::io_context::strand strand;
    boost::asio::ip::tcp::acceptor acceptor;

    explicit
    impl(boost::asio::io_context& io_context, std::uint_least16_t port)
    : strand(io_context)
    , acceptor(io_context)
    {
        acceptor.set_option(boost::asio::socket_base::reuse_address(true));
        acceptor.set_option(boost::asio::ip::v6_only(false));

        auto protocol = boost::asio::ip::tcp::v6();
        boost::asio::ip::tcp::endpoint endpoint{protocol, port};
        acceptor.bind(endpoint);
        acceptor.listen();
    }

    void accept_next(std::shared_ptr<impl> self) {
        acceptor.async_accept(bind_executor(strand, [
            this, self = std::move(self)
        ](auto ec, auto socket) mutable {
            if (ec == boost::asio::error::operation_aborted) { return; }
            if (!ec) {
                throw boost::system::system_error(ec,
                    "boost::asio::ip::tcp::acceptor::async_accept");
            }

            accept_next(std::move(self));

            // ...
        }));
    }

    void close(std::shared_ptr<impl> self) {
        dispatch(strand, [this, self = std::move(self)]{
            acceptor.close();
        });
    }
};

server::server(boost::asio::io_context& io_context, std::uint_least16_t port)
: impl_(std::make_shared<impl>(io_context, port))
{ impl_->accept_next(impl_); }

server::~server() {
    impl_->close(impl_);
}

///////////////////////////////////////////////////////////////////////////////
}
