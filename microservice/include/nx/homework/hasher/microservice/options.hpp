#ifndef NX_HOMEWORK_HASHER_MICROSERVICE_DETAIL_INCLUDE_GUARD_NX_HOMEWORK_HASHER_MICROSERVICE_OPTIONS
#define NX_HOMEWORK_HASHER_MICROSERVICE_DETAIL_INCLUDE_GUARD_NX_HOMEWORK_HASHER_MICROSERVICE_OPTIONS

#include <cstdint>

namespace nx::homework::hasher::microservice {
///////////////////////////////////////////////////////////////////////////////

struct options {
    std::uint_least16_t port = 9425;

    explicit
    options(int argc, char const* const* argv);
};

///////////////////////////////////////////////////////////////////////////////
}

#endif
