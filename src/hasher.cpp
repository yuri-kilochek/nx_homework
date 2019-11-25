#include <nx/homework/hasher/hasher.hpp>

#include <nettle/sha2.h>

#include <cstdint>

namespace nx::homework::hasher {
///////////////////////////////////////////////////////////////////////////////

struct hasher::impl {
    sha256_ctx ctx;
};

hasher::hasher()
: impl_(std::make_unique<impl>())
{ reset(); }

hasher::~hasher() = default;

void hasher::reset() {
    sha256_init(&impl_->ctx);
}

void hasher::append(void const* bytes, std::size_t byte_count) {
    sha256_update(&impl_->ctx, byte_count,
                               static_cast<std::uint8_t const*>(bytes));
}

void hasher::finish(std::vector<std::byte>& digest) {
    digest.resize(SHA256_DIGEST_SIZE);
    sha256_digest(&impl_->ctx, digest.size(),
                               reinterpret_cast<std::uint8_t*>(digest.data()));
}

///////////////////////////////////////////////////////////////////////////////
}
