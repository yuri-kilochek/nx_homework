#ifndef NX_HOMEWORK_HASHER_MICROSERVICE_OPTIONS_INCLUDED
#define NX_HOMEWORK_HASHER_MICROSERVICE_OPTIONS_INCLUDED

#include <cstdint>

namespace nx_homework::hasher_microservice {
///////////////////////////////////////////////////////////////////////////////

struct options {
    std::uint_least16_t port = 9425;

    explicit
    options(int argc, char const* const* argv);
};

///////////////////////////////////////////////////////////////////////////////
}

#endif
