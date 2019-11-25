#ifndef NX_HOMEWORK_HASHER_INCLUDE_GUARD_NX_HOMEWORK_HASHER_HASHER
#define NX_HOMEWORK_HASHER_INCLUDE_GUARD_NX_HOMEWORK_HASHER_HASHER

#include <nx/homework/hasher/api.hpp>

#include <cstddef>
#include <vector>
#include <memory>

namespace nx::homework::hasher {
///////////////////////////////////////////////////////////////////////////////

struct NX_HOMEWORK_HASHER_API hasher {
    hasher();
    ~hasher();

    void reset();
    void append(void const* bytes, std::size_t byte_count);
    void finish(std::vector<std::byte>& digest);

private:
    struct impl;
    std::unique_ptr<impl> impl_;
};

///////////////////////////////////////////////////////////////////////////////
}

#endif
