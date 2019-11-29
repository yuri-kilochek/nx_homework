#include <nx_homework/hasher.hpp>

#include <nettle/sha2.h>

#include <cstdint>

namespace nx_homework {
///////////////////////////////////////////////////////////////////////////////

struct hasher::impl {
    sha256_ctx ctx;
};

hasher::hasher()
: impl_(std::make_unique<impl>())
{ sha256_init(&impl_->ctx); }

hasher::~hasher() = default;

void hasher::append(void const* data, std::size_t size) {
    sha256_update(&impl_->ctx, size, static_cast<std::uint8_t const*>(data));
}

void hasher::finish(std::vector<char>& digest) {
    digest.resize(SHA256_DIGEST_SIZE);
    sha256_digest(&impl_->ctx, digest.size(),
                               reinterpret_cast<std::uint8_t*>(digest.data()));
}

///////////////////////////////////////////////////////////////////////////////
}
