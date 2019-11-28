#ifndef NX_HOMEWORK_HASHER_DETAIL_INCLUDE_GUARD_NX_HOMEWORK_HASHER_HASHER
#define NX_HOMEWORK_HASHER_DETAIL_INCLUDE_GUARD_NX_HOMEWORK_HASHER_HASHER

#include <nx/homework/hasher/api.hpp>

#include <cstddef>
#include <vector>
#include <memory>

namespace nx::homework::hasher {
///////////////////////////////////////////////////////////////////////////////

struct NX_HOMEWORK_HASHER_API hasher {
    hasher();

    ~hasher();

    void append(void const* data, std::size_t size);

    void finish(std::vector<char>& digest);

private:
    struct impl;
    std::unique_ptr<impl> impl_;
};

///////////////////////////////////////////////////////////////////////////////
}

#endif
