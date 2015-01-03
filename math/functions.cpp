#include <cassert>
#include <gtoolbox/math/functions.h>
#include <gtoolbox/test/test.h>

namespace math {

    void functions::tests()
    {
        test_assert(math::min<uint8_t>()  == 0);
        test_assert(math::min<uint16_t>() == 0);
        test_assert(math::min<uint32_t>() == 0);
        test_assert(math::min<uint64_t>() == 0);

        test_assert(math::min<int8_t>()   == -128);
        test_assert(math::min<int16_t>()  == -32768);
        test_assert(math::min<int32_t>()  == -2147483648);
        test_assert(math::min<int64_t>()  == INT64_C(0x8000000000000000));

        test_assert(math::max<uint8_t>()  == 255);
        test_assert(math::max<uint16_t>() == 65535);
        test_assert(math::max<uint32_t>() == 4294967295);
        test_assert(math::max<uint64_t>() == UINT64_C(0xFFFFFFFFFFFFFFFF));

        test_assert(math::max<int8_t>()   == 127);
        test_assert(math::max<int16_t>()  == 32767);
        test_assert(math::max<int32_t>()  == 2147483647);
        test_assert(math::max<int64_t>()  == INT64_C(0x7FFFFFFFFFFFFFFF));

        test_assert(math::iabs(-1) == 1);
        test_assert(math::iabs(-2) == 2);
        test_assert(math::iabs(-2147483648) == 2147483648u);

        test_assert(math::ceilpow2(3) == 4);
        test_assert(math::ceilpow2(513) == 1024);
        test_assert(math::ceilpow2(1023) == 1024);

        test_assert(math::log2(math::ceilpow2(3)) == 2);
        test_assert(math::log2(4) == 2);
        test_assert(math::log2(1024) == 10);

        test_assert(math::min(1, 2) == 1);
        test_assert(math::max(1, 2) == 2);

        test_assert(math::min(1, 2, 3) == 1);
        test_assert(math::min(3, 2, 1) == 1);
        test_assert(math::max(1, 2, 3) == 3);
        test_assert(math::max(3, 2, 1) == 3);

        test_assert(math::min(1, 2, 3, 4) == 1);
        test_assert(math::min(4, 3, 2, 1) == 1);
        test_assert(math::max(1, 2, 3, 4) == 4);
        test_assert(math::max(4, 3, 2, 1) == 4);

        test_assert(math::sum(1, 2) == 3);
        test_assert(math::sum(1, 2, 3) == 6);

        test_assert(math::avg(1, 2) == 1);
        test_assert(math::avg<float>(1, 2) == 1.5f);

        test_assert(math::avg(1, 2, 3) == 2);
        test_assert(math::avg<float>(1, 2, 3) == 2.f);
    }

} // namespace math