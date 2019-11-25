#ifndef NX_HOMEWORK_HASHER_INCLUDE_GUARD_NX_HOMEWORK_HASHER_API
#define NX_HOMEWORK_HASHER_INCLUDE_GUARD_NX_HOMEWORK_HASHER_API

#include <boost/config.hpp>

///////////////////////////////////////////////////////////////////////////////

#if defined(NX_HOMEWORK_HASHER_EXPORT) && defined(NX_HOMEWORK_HASHER_IMPORT)
    #error "NX_HOMEWORK_HASHER_EXPORT and NX_HOMEWORK_HASHER_IMPORT must not be defined simultaneously."
#elif defined(NX_HOMEWORK_HASHER_EXPORT)
    #define NX_HOMEWORK_HASHER_API BOOST_SYMBOL_EXPORT BOOST_SYMBOL_VISIBLE
#elif defined(NX_HOMEWORK_HASHER_IMPORT)
    #define NX_HOMEWORK_HASHER_API BOOST_SYMBOL_IMPORT
#else
    #define NX_HOMEWORK_HASHER_API
#endif

///////////////////////////////////////////////////////////////////////////////

#endif
