#include <nx_homework/hasher_microservice/options.hpp>
#include <nx_homework/hasher_microservice/server.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/system/system_error.hpp>
#include <boost/scope_exit.hpp>

#include <cstddef>
#include <csignal>
#include <thread>
#include <iostream>

namespace nx_homework::hasher_microservice {
///////////////////////////////////////////////////////////////////////////////

static
auto main(int argc, char const* const* argv)
-> int
{
    hasher_microservice::options options(argc, argv);

    boost::asio::io_context io_context;

    hasher_microservice::server server(io_context, options.port);

    boost::asio::signal_set signal_set(io_context, SIGINT, SIGTERM);
    signal_set.async_wait([&](auto ec, auto signal) {
        if (ec == boost::asio::error::operation_aborted) { return; }
        if (ec) { throw boost::system::system_error(ec); }

        server.terminate();
    });

    std::vector<std::thread> threads(std::thread::hardware_concurrency());
    BOOST_SCOPE_EXIT_ALL(&) {
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    };
    for (auto& thread : threads) {
        thread = std::thread([&]{
            io_context.run();
        });
    }
    std::cerr << "Started on port " << options.port << "\n";
    std::cerr << "Press Ctrl+C to terminate\n";

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
}

int main(int argc, char* argv[]) {
    return nx_homework::hasher_microservice::main(argc, argv);
}
