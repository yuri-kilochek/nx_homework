#include <nx/homework/hasher/microservice/session.hpp>

namespace nx::homework::hasher::microservice {
///////////////////////////////////////////////////////////////////////////////

struct session::impl {

};

session::session(boost::asio::ip::tcp::socket&& socket) {

}

session::~session() {
    terminate();
}

void session::terminate() {

}

void session::async_wait_impl(any_handler<boost::system::error_code> handler) {

}

///////////////////////////////////////////////////////////////////////////////
}
