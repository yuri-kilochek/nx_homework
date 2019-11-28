#ifndef NX_HOMEWORK_HASHER_MICROSERVICE_DETAIL_INCLUDE_GUARD_NX_HOMEWORK_HASHER_MICROSERVICE_HEX_ENCODE
#define NX_HOMEWORK_HASHER_MICROSERVICE_DETAIL_INCLUDE_GUARD_NX_HOMEWORK_HASHER_MICROSERVICE_HEX_ENCODE

namespace nx::homework::hasher::microservice {
///////////////////////////////////////////////////////////////////////////////

template <typename InputIt, typename OutputIt>
auto hex_encode(InputIt in, InputIt in_end, OutputIt out) {
    constexpr
    char digits[] = u8"0123456789ABCDEF";

    while (in != in_end) {
        unsigned byte = static_cast<unsigned char>(*in++);
        *out++ = digits[byte >> 4u & 0xFu];
        *out++ = digits[byte       & 0xFu];
    }

    return out;
}

///////////////////////////////////////////////////////////////////////////////
}

#endif
