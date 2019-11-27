#include <nx/homework/hasher/microservice/server.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/system/system_error.hpp>
#include <boost/scope_exit.hpp>

#include <cstddef>
#include <thread>
#include <optional>

namespace nx::homework::hasher::microservice {
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
    auto concurrency = std::thread::hardware_concurrency();

    boost::asio::io_context io_context(concurrency);

    std::optional<server> server(std::in_place, io_context, 9009);

    boost::asio::signal_set signal_set(io_context, SIGINT, SIGTERM);
    signal_set.async_wait([&](auto ec, auto signal){
        if (ec == boost::asio::error::operation_aborted) { return; }
        if (!ec) {
            throw boost::system::system_error(ec,
                "boost::asio::signal_set::async_wait");
        }

        server.reset();
    });

    std::vector<std::thread> threads(concurrency);
    BOOST_SCOPE_EXIT_ALL(&) {
        for (auto& thread : threads) {
            if (!thread.joinable()) { continue; }
            thread.join();
        }
    };
    for (auto& thread : threads) {
        thread = std::thread([&]{
            io_context.run();
        });
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
}

int main(int argc, char* argv[]) {
    return nx::homework::hasher::microservice::main(argc, argv);
}
