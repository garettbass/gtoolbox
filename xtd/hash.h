#pragma once
#include <stddef.h>
#include <stdint.h>

namespace xtd {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    uint32_t hash32(const char* data, size_t size, uint32_t seed = 0);

    template <size_t SIZE>
    inline
    uint32_t hash32(const char (&data)[SIZE], uint32_t seed = 0)
    {
        return hash32(data, SIZE, seed);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    uint64_t hash64(const char* data, size_t size, uint32_t seed = 0);

    template <size_t SIZE>
    inline
    uint64_t hash64(const char (&data)[SIZE], uint32_t seed = 0)
    {
        return hash64(data, SIZE, seed);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    struct hash128 {

        union {
            uint64_t lo, hi;
            uint32_t h1, h2, h3, h4;
        };

        hash128(const char* data, size_t size, uint32_t seed = 0);

    };

} // namespace xtd