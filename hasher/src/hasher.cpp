#include <nx_homework/hasher.hpp>

#include <sodium.h>

#include <stdexcept>

namespace nx_homework {
///////////////////////////////////////////////////////////////////////////////

struct hasher::impl {
    crypto_hash_sha256_state state;
};

hasher::hasher()
: impl_(std::make_unique<impl>())
{
    static
    int const dummy = []{
        if (sodium_init() < 0) {
            throw std::runtime_error("sodium_init() failed");
        }
        return 0;
    }();
    crypto_hash_sha256_init(&impl_->state);
}

hasher::~hasher() = default;

void hasher::append(void const* data, std::size_t size) {
    crypto_hash_sha256_update(&impl_->state,
                              static_cast<unsigned char const*>(data), size);
}

void hasher::finish(std::vector<char>& digest) {
    digest.resize(crypto_hash_sha256_BYTES);
    crypto_hash_sha256_final(&impl_->state,
                             reinterpret_cast<unsigned char*>(digest.data()));
}

///////////////////////////////////////////////////////////////////////////////
}
