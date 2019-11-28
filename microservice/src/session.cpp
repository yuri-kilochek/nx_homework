#include <nx/homework/hasher/microservice/session.hpp>
#include <nx/homework/hasher/microservice/hex_encode.hpp>
#include <nx/homework/hasher/hasher.hpp>

#include <boost/system/system_error.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/executor.hpp>
#include <boost/asio/executor.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/write.hpp>

#include <vector>
#include <algorithm>
#include <iterator>

namespace nx::homework::hasher::microservice {
///////////////////////////////////////////////////////////////////////////////

struct session::impl {
    boost::asio::strand<boost::asio::executor> strand;
    boost::asio::ip::tcp::socket socket;

    bool is_reading = false;
    std::vector<char> in_buffer;

    bool is_hashing = false;
    std::vector<char> hash_in_buffer;
    homework::hasher::hasher hasher;
    std::vector<char> hash_out_buffer;
    std::vector<char> digest;

    bool is_writing = false;
    std::vector<char> out_buffer;

    any_handler<boost::system::error_code> wait_handler;

    explicit
    impl(boost::asio::ip::tcp::socket&& socket)
    : strand(socket.get_executor())
    , socket(std::move(socket))
    {}

    void start_reading(std::shared_ptr<impl> self) {
        in_buffer.resize(16 * 1024); // 16KiB
        socket.async_read_some(boost::asio::buffer(in_buffer),
            bind_executor(strand, [
                this, self = std::move(self)
            ](auto ec, auto size) mutable {
                is_reading = false;

                if (ec == boost::asio::error::operation_aborted
                 || ec == boost::asio::error::bad_descriptor
                 || ec == boost::asio::error::broken_pipe)
                { return; }

                if (ec == boost::asio::error::connection_reset
                 || ec == boost::asio::error::eof)
                {
                    complete_async_wait({});
                    socket.close();
                    return;
                }

                if (ec) { throw boost::system::system_error(ec); }

                in_buffer.resize(size);

                update(std::move(self));
            }));
        is_reading = true;
    }

    void start_hashing(std::shared_ptr<impl> self) {
        post(strand.get_inner_executor(), [
            this, self = std::move(self)
        ]() mutable {
            auto begin = hash_in_buffer.begin();
            while (true) {
                auto end = std::find(begin, hash_in_buffer.end(), u8'\n');

                hasher.append(&*begin, end - begin);

                if (end == hash_in_buffer.end()) { break; }

                hasher.finish(digest);

                hex_encode(digest.begin(), digest.end(),
                           std::back_inserter(hash_out_buffer));
                hash_out_buffer.push_back(u8'\n');

                begin = end + 1;
            }
            hash_in_buffer.clear();

            dispatch(strand, [this, self = std::move(self)]() mutable {
                is_hashing = false;
                update(std::move(self));
            });
        });

        is_hashing = true;
    }

    void start_writing(std::shared_ptr<impl> self) {
        async_write(socket, boost::asio::buffer(out_buffer),
            bind_executor(strand, [
                this, self = std::move(self)
            ](auto ec, auto) mutable {
                is_writing = false;

                if (ec == boost::asio::error::operation_aborted
                 || ec == boost::asio::error::bad_descriptor
                 || ec == boost::asio::error::broken_pipe)
                { return; }

                if (ec == boost::asio::error::connection_reset
                 || ec == boost::asio::error::eof)
                {
                    complete_async_wait({});
                    socket.close();
                    return;
                }

                if (ec) { throw boost::system::system_error(ec); }

                out_buffer.clear();

                update(std::move(self));
            }));
        is_writing = true;
    }

    void update(std::shared_ptr<impl> self) {
    reading:
        if (!is_reading && in_buffer.empty()) {
            start_reading(self);
        }

    reading_to_hashing:
        if (!is_reading && !in_buffer.empty()
         && !is_hashing && hash_in_buffer.empty())
        {
            std::swap(in_buffer, hash_in_buffer);
            goto reading;
        }

    hashing:
        if (!is_hashing && !hash_in_buffer.empty() && hash_out_buffer.empty()) {
            start_hashing(self);
            goto reading_to_hashing;
        }

    hashing_to_writing:
        if (!is_hashing && !hash_out_buffer.empty()
         && !is_writing && out_buffer.empty())
        {
            std::swap(hash_out_buffer, out_buffer);
            goto hashing;
        }

        if (!is_writing && !out_buffer.empty()) {
            start_writing(self);
            goto hashing_to_writing;
        }
    }

    void terminate() {
        complete_async_wait(boost::asio::error::operation_aborted);
        socket.close();
    }

    void async_wait(any_handler<boost::system::error_code>&& handler) {
        complete_async_wait(boost::asio::error::operation_aborted);
        wait_handler = std::move(handler);
    }

    void complete_async_wait(boost::system::error_code ec) {
        if (!wait_handler) { return; }
        auto executor = wait_handler.get_executor();
        post(executor, [wait_handler = std::move(wait_handler), ec]() mutable {
            std::move(wait_handler)(ec);
        });
    }
};

session::session(boost::asio::ip::tcp::socket&& socket)
: impl_(std::make_shared<impl>(std::move(socket)))
{ impl_->update(impl_); }

session::~session() {
    terminate();
}

void session::terminate() {
    dispatch(impl_->strand, [impl_ = impl_]{
        impl_->terminate();
    });
}

void session::async_wait_impl(any_handler<boost::system::error_code> handler) {
    dispatch(impl_->strand, [
        impl_ = impl_, handler = std::move(handler)
    ]() mutable{
        impl_->async_wait(std::move(handler));
    });
}

///////////////////////////////////////////////////////////////////////////////
}
