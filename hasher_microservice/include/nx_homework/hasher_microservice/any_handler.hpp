#ifndef NX_HOMEWORK_HASHER_MICROSERVICE_ANY_HANDLER_INCLUDED
#define NX_HOMEWORK_HASHER_MICROSERVICE_ANY_HANDLER_INCLUDED

#include <boost/asio/executor.hpp>
#include <boost/asio/associated_executor.hpp>

#include <utility>
#include <memory>

namespace nx_homework::hasher_microservice {
///////////////////////////////////////////////////////////////////////////////

template <typename... Args>
struct any_handler {
    template <typename Handler>
    any_handler(Handler&& handler)
    {
        struct box_impl
        : box
        {
            explicit
            box_impl(Handler&& handler)
            : handler_(std::forward<Handler>(handler))
            {}

            auto get_executor()
            const
            -> boost::asio::executor
            override
            { return boost::asio::get_associated_executor(handler_); }

            void invoke(Args&&... args)
            override
            { std::move(handler_)(std::forward<Args>(args)...); }

        private:
            std::decay_t<Handler> handler_;
        };

        box_ = std::make_unique<box_impl>(std::forward<Handler>(handler));
    }

    any_handler() = default;

    explicit
    operator bool()
    const noexcept
    { return !!box_; }

    auto get_executor()
    const
    -> boost::asio::executor
    { return box_->get_executor(); }

    void operator()(Args... args) {
        box_->invoke(std::forward<Args>(args)...);
    }

private:
    struct box {
        virtual
        ~box() 
        = default;

        virtual
        auto get_executor()
        const
        -> boost::asio::executor
        = 0;

        virtual
        void invoke(Args&&... args)
        = 0;
    };

    std::unique_ptr<box> box_;
};

///////////////////////////////////////////////////////////////////////////////
}

#endif
