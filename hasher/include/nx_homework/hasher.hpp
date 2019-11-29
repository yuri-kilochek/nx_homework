#ifndef NX_HOMEWORK_HASHER_INCLUDED
#define NX_HOMEWORK_HASHER_INCLUDED

#include <nx_homework/hasher/detail/api.hpp>

#include <cstddef>
#include <vector>
#include <memory>

namespace nx_homework {
///////////////////////////////////////////////////////////////////////////////

struct NX_HOMEWORK_HASHER_DETAIL_API hasher {
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
