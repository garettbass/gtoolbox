#include "hash.h"

//------------------------------------------------------------------------------

#define forceinline inline __attribute__((always_inline))

forceinline static constexpr uint32_t rotl32(uint32_t x, uint32_t k)
{
    return (((x)<<(k)) | ((x)>>(32-(k))));
}

//forceinline static constexpr uint64_t rotl64(uint64_t x, uint64_t k)
//{
//    return (((x)<<(k)) | ((x)>>(64-(k))));
//}

//------------------------------------------------------------------------------

forceinline static uint32_t fmix32(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

//------------------------------------------------------------------------------

namespace xtd {

    //--------------------------------------------------------------------------
    // based on MurmurHash3_x86_32, rev 136 (http://code.google.com/p/smhasher/)

    uint32_t hash32(
        const char* const data,
        const size_t      size,
        const uint32_t    seed)
    {
        enum : uint32_t {
            C1 = 0xcc9e2d51,
            C2 = 0x1b873593,
            C3 = 0xe6546b69,
        };

        uint32_t hash = seed;

        const uint8_t* const bytes = (const uint8_t *)data;

        const intptr_t quadCount(size / 4);

        const uint8_t* const tailBytesBegin(bytes + (quadCount * 4));

        const uint32_t* const bodyQuadsEnd((const uint32_t *)tailBytesBegin);

        // hash body quads

        for (intptr_t i(-quadCount); i; ++i)
        {
            uint32_t quad = bodyQuadsEnd[i];

            quad *= C1;
            quad  = rotl32(quad, 15);
            quad *= C2;

            hash ^= quad;
            hash  = rotl32(hash, 13);
            hash *= C3;
        }

        // hash tail bytes

        uint32_t quad(0);

        switch(size & 3) {
            case 3:
                quad ^= tailBytesBegin[2] << 16;

            case 2:
                quad ^= tailBytesBegin[1] <<  8;

            case 1:
                quad ^= tailBytesBegin[0];

                quad *= C1;
                quad  = rotl32(quad, 15);
                quad *= C2;

                hash ^= quad;
        };

        // finalize

        hash ^= size;

        hash = fmix32(hash);

        return hash;
    }

    //--------------------------------------------------------------------------

    uint64_t hash64(
        const char* const data,
        const size_t      size,
        const uint32_t    seed)
    {
        enum : uint32_t {
            C1 = 0xcc9e2d51,
            C2 = 0x1b873593,
            C3 = 0xe6546b69,
        };

        uint32_t hash_hi =  seed;
        uint32_t hash_lo = ~seed;

        const uint8_t* const bytes = (const uint8_t *)data;

        const intptr_t quadCount(size / 4);

        const uint8_t* const tailBytesBegin(bytes + (quadCount * 4));

        const uint32_t* const bodyQuadsEnd((const uint32_t *)tailBytesBegin);

        // hash body quads

        for (intptr_t i(-quadCount); i; ++i)
        {
            uint32_t quad = bodyQuadsEnd[i];

            quad *= C1;
            quad  = rotl32(quad, 15);
            quad *= C2;

            hash_hi ^= quad;
            hash_hi  = rotl32(hash_hi, 13);
            hash_hi *= C3;

            hash_lo ^= quad;
            hash_lo  = rotl32(hash_lo, 13);
            hash_lo *= C3;
        }

        // hash tail bytes

        uint32_t quad(0);

        switch(size & 3) {
            case 3:
                quad ^= tailBytesBegin[2] << 16;

            case 2:
                quad ^= tailBytesBegin[1] <<  8;

            case 1:
                quad ^= tailBytesBegin[0];

                quad *= C1;
                quad  = rotl32(quad, 15);
                quad *= C2;

                hash_hi ^= quad;
                hash_lo ^= quad;
        };

        // finalize

        hash_hi ^= size;
        hash_lo ^= size;

        hash_hi = fmix32(hash_hi);
        hash_lo = fmix32(hash_lo);

        return (uint64_t(hash_hi) << 32) + hash_lo;
    }

    //--------------------------------------------------------------------------

    hash128::hash128(
        const char* const data,
        const size_t      size,
        const uint32_t    seed)
    {
        enum : uint32_t {
            c1 = 0x239b961b,
            c2 = 0xab0e9789,
            c3 = 0x38b34ae5,
            c4 = 0xa1e38b93,
        };

        h1 = seed;
        h2 = seed;
        h3 = seed;
        h4 = seed;

        const uint8_t* const bytes = (const uint8_t*)data;

        const intptr_t quadCount(size / 16);

        const uint8_t* const tailBytes(bytes + (quadCount * 16));

        const uint32_t* const bodyQuadsEnd((const uint32_t *)tailBytes);

        // hash body quads

        for (intptr_t i(-quadCount); i; ++i)
        {
            uint32_t k1 = bodyQuadsEnd[i * 4 + 0];
            uint32_t k2 = bodyQuadsEnd[i * 4 + 1];
            uint32_t k3 = bodyQuadsEnd[i * 4 + 2];
            uint32_t k4 = bodyQuadsEnd[i * 4 + 3];

            k1 *= c1; k1 = rotl32(k1,15); k1 *= c2; h1 ^= k1;

            h1 = rotl32(h1, 19); h1 += h2; h1 = h1 * 5 + 0x561ccd1b;

            k2 *= c2; k2 = rotl32(k2,16); k2 *= c3; h2 ^= k2;

            h2 = rotl32(h2, 17); h2 += h3; h2 = h2 * 5 + 0x0bcaa747;

            k3 *= c3; k3 = rotl32(k3,17); k3 *= c4; h3 ^= k3;

            h3 = rotl32(h3, 15); h3 += h4; h3 = h3 * 5 + 0x96cd1c35;

            k4 *= c4; k4 = rotl32(k4,18); k4 *= c1; h4 ^= k4;

            h4 = rotl32(h4,13); h4 += h1; h4 = h4 * 5 + 0x32ac3b17;
        }

        // hash tail bytes

        uint32_t k1 = 0;
        uint32_t k2 = 0;
        uint32_t k3 = 0;
        uint32_t k4 = 0;

        switch(size & 15)
        {
        case 15: k4 ^= tailBytes[14] << 16;
        case 14: k4 ^= tailBytes[13] << 8;
        case 13: k4 ^= tailBytes[12] << 0;
                 k4 *= c4; k4 = rotl32(k4,18); k4 *= c1; h4 ^= k4;

        case 12: k3 ^= tailBytes[11] << 24;
        case 11: k3 ^= tailBytes[10] << 16;
        case 10: k3 ^= tailBytes[ 9] << 8;
        case  9: k3 ^= tailBytes[ 8] << 0;
                 k3 *= c3; k3 = rotl32(k3,17); k3 *= c4; h3 ^= k3;

        case  8: k2 ^= tailBytes[ 7] << 24;
        case  7: k2 ^= tailBytes[ 6] << 16;
        case  6: k2 ^= tailBytes[ 5] << 8;
        case  5: k2 ^= tailBytes[ 4] << 0;
                 k2 *= c2; k2 = rotl32(k2,16); k2 *= c3; h2 ^= k2;

        case  4: k1 ^= tailBytes[ 3] << 24;
        case  3: k1 ^= tailBytes[ 2] << 16;
        case  2: k1 ^= tailBytes[ 1] << 8;
        case  1: k1 ^= tailBytes[ 0] << 0;
                 k1 *= c1; k1 = rotl32(k1,15); k1 *= c2; h1 ^= k1;
        };

        // finalization

        h1 ^= size; h2 ^= size; h3 ^= size; h4 ^= size;

        h1 += h2; h1 += h3; h1 += h4;
        h2 += h1; h3 += h1; h4 += h1;

        h1 = fmix32(h1);
        h2 = fmix32(h2);
        h3 = fmix32(h3);
        h4 = fmix32(h4);

        h1 += h2; h1 += h3; h1 += h4;
        h2 += h1; h3 += h1; h4 += h1;
    }

    //--------------------------------------------------------------------------

} // namespace xtd













































