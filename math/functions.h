#pragma once
#include <cmath>
#include <iostream>
#include <zna/source/thirdParty/gtoolbox/env/compiler.h>
#include <zna/source/thirdParty/gtoolbox/math/forward/float.h>
#include <zna/source/thirdParty/gtoolbox/math/forward/int.h>
#include <zna/source/thirdParty/gtoolbox/xtd/traits.h>

namespace math {

    struct functions { static void tests(); };

    template <typename Int>
    inline
    constexpr
    unsigned
    bitsof()
    { return sizeof(Int) * 8; }

    template <typename Int>
    inline
    constexpr
    unsigned
    bitsof(const Int)
    { return bitsof<Int>(); }

    template <typename Int>
    inline
    xtd::make_unsigned<Int>
    iabs(const Int i)
    {
        env_static_assert(xtd::is_integral<Int>());

        using unsigned_int = xtd::make_unsigned<Int>;

        const unsigned_int mask = i >> (bitsof<Int>() - 1);

        return (unsigned_int(i) + mask) ^ mask;
    }

    inline uint8   abs(uint8   u) { return u; }
    inline uint16  abs(uint16  u) { return u; }
    inline uint32  abs(uint32  u) { return u; }
    inline uint64  abs(uint64  u) { return u; }
    inline uint8   abs(int8    i) { return math::iabs(i); }
    inline uint16  abs(int16   i) { return math::iabs(i); }
    inline uint32  abs(int32   i) { return math::iabs(i); }
    inline uint64  abs(int64   i) { return math::iabs(i); }
    inline float32 abs(float32 f) { return std::abs(f); }
    inline float64 abs(float64 f) { return std::abs(f); }

    template <typename Int>
    inline
    bool
    is_pow2(const Int n)
    { return !(n & (n - 1)); }

    template <typename Int>
    inline
    Int
    ceilpow2(const Int n, const Int pow2)
    {
        assert(is_pow2(pow2));
        const Int x = pow2 - 1;
        return ((n + x) & ~x);
    }

    template <typename Int>
    inline
    Int
    ceilpow2(Int n)
    {
        n -= 1;
        n |= n >>  1;
        n |= n >>  2;
        n |= n >>  4;
        n |= n >>  8;
        n |= n >> 16;
        n += 1;
        return n;
    }

    template <typename Int>
    inline
    Int
    floorpow2(Int n)
    {
        n |= n >>  1;
        n |= n >>  2;
        n |= n >>  4;
        n |= n >>  8;
        n |= n >> 16;
        return n - (n >> 1);
    }

    inline
    uint32
    log2(uint32 u)
    {
        uint32 r;
        uint32 s;
        r = (u > 0xFFFF) << 4; u >>= r;
        s = (u > 0xFF  ) << 3; u >>= s; r |= s;
        s = (u > 0xF   ) << 2; u >>= s; r |= s;
        s = (u > 0x3   ) << 1; u >>= s; r |= s; r |= (u >> 1);
        return r;
    }

    // min ---------------------------------------------------------------------

    template <typename T> constexpr T min();

    template <> uint8  constexpr min< uint8>() { return 0; }
    template <> uint16 constexpr min<uint16>() { return 0; }
    template <> uint32 constexpr min<uint32>() { return 0; }
    template <> uint64 constexpr min<uint64>() { return 0; }
    template <> int8   constexpr min<  int8>() { return  int8( uint8(1) <<  7); }
    template <> int16  constexpr min< int16>() { return int16(uint16(1) << 15); }
    template <> int32  constexpr min< int32>() { return int32(uint32(1) << 31); }
    template <> int64  constexpr min< int64>() { return int64(uint64(1) << 63); }

    template <typename T>
    env_forceinline
    constexpr const T&
    min(const T& a)
    { return a; }

    template <typename T>
    env_forceinline
    constexpr const T&
    min(const T& a, const T& b)
    { return (a < b) ? a : b; }

// Failed in iOS Release - min(3, 2, 1) returned 0
//    template <typename T, typename... U>
//    env_forceinline
//    constexpr const T&
//    min(const T& a, const T& b, const U&... c)
//    { return min(a, math::min(b, T(c)...)); }

    template <typename T>
    env_forceinline
    constexpr const T&
    min(const T& a, const T& b, const T& c)
    { return min(min(a, b), c); }

    template <typename T>
    env_forceinline
    constexpr const T&
    min(const T& a, const T& b, const T& c, const T& d)
    { return min(min(a, b), min(c, d)); }

    template <typename T, size_t SIZE>
    const T&
    min(const T (&array)[SIZE])
    {
        env_static_assert(SIZE > 0);

        auto* head = array;
        auto* tail = head + SIZE;

        auto* min = head;

        while (++head != tail) min = (*head < *min) ? head : min;

        return *min;
    }

    // max ---------------------------------------------------------------------

    template <typename T> constexpr T max();

    template <> uint8  constexpr max< uint8>() { return ~min< uint8>(); }
    template <> uint16 constexpr max<uint16>() { return ~min<uint16>(); }
    template <> uint32 constexpr max<uint32>() { return ~min<uint32>(); }
    template <> uint64 constexpr max<uint64>() { return ~min<uint64>(); }
    template <> int8   constexpr max<  int8>() { return ~min<  int8>(); }
    template <> int16  constexpr max< int16>() { return ~min< int16>(); }
    template <> int32  constexpr max< int32>() { return ~min< int32>(); }
    template <> int64  constexpr max< int64>() { return ~min< int64>(); }

    template <typename T>
    env_forceinline
    constexpr const T&
    max(const T& a)
    { return a; }

    template <typename T>
    env_forceinline
    constexpr const T&
    max(const T& a, const T& b)
    { return (a > b) ? a : b; }

// Failed in iOS Release
//    template <typename T, typename... U>
//    env_forceinline
//    constexpr const T&
//    max(const T& a, const T& b, const U&... c)
//    { return max(a, math::max(b, T(c)...)); }

    template <typename T>
    env_forceinline
    constexpr const T&
    max(const T& a, const T& b, const T& c)
    { return max(max(a, b), c); }

    template <typename T>
    env_forceinline
    constexpr const T&
    max(const T& a, const T& b, const T& c, const T& d)
    { return max(max(a, b), max(c, d)); }

    template <typename T, size_t SIZE>
    const T&
    max(const T (&array)[SIZE])
    {
        env_static_assert(SIZE > 0);

        auto* head = array;
        auto* tail = head + SIZE;

        auto* max = head;

        while (++head != tail) max = (*head > *max) ? head : max;

        return *max;
    }

    // clamp -------------------------------------------------------------------

    template <typename T>
    env_forceinline
    const T&
    clamp(const T& value, const T& lo, const T& hi)
    { return min(max(value, lo), hi); }

    // sum ---------------------------------------------------------------------

    template <typename T>
    env_forceinline
    constexpr const T
    sum(const T& a)
    { return a; }

    template <typename T>
    env_forceinline
    constexpr const T
    sum(const T& a, const T& b)
    { return a + b; }

    template <typename T, typename... TT>
    env_forceinline
    constexpr const T
    sum(const T& a, const T& b, const TT&... c)
    { return sum(a, math::sum(b, T(c)...)); }

    // avg ---------------------------------------------------------------------

    template <typename T>
    env_forceinline
    constexpr const T
    avg(const T& a)
    { return a; }

    template <typename T>
    env_forceinline
    constexpr const T
    avg(const T& a, const T& b)
    { return sum(a, b) / T(2); }

    template <typename T, typename... TT>
    env_forceinline
    constexpr const T
    avg(const T& a, const T& b, const TT&... c)
    { return sum(a, b, T(c)...) / T(2 + sizeof...(TT)); }

    // all ---------------------------------------------------------------------

    template <typename Arg>
    env_forceinline
    constexpr
    bool
    all(const Arg& arg)
    {
        return bool(arg);
    }

    template <typename Arg, typename... Args>
    env_forceinline
    constexpr
    bool
    all(const Arg& arg, const Args&... args)
    {
        return bool(arg) and all(args...);
    }

    // any ---------------------------------------------------------------------

    template <typename Arg>
    env_forceinline
    constexpr
    bool
    any(const Arg& arg)
    {
        return bool(arg);
    }

    template <typename Arg, typename... Args>
    env_forceinline
    constexpr
    bool
    any(const Arg& arg, const Args&... args)
    {
        return bool(arg) or any(args...);
    }

/*
    //--------------------------------------------------------------------------
    // import cmath declarations into xtd
    // as listed on: http://www.cplusplus.com/reference/cmath/

    // trigonometric functions
    using std::cos;
    using std::sin;
    using std::tan;
    using std::acos;
    using std::asin;
    using std::atan;
    using std::atan2;

    // hyperbolic functions
    using std::cosh;
    using std::sinh;
    using std::tanh;
    // using std::acosh;
    // using std::asinh;
    // using std::atanh;

    // exponential and logarithmic functions
    using std::exp;
    using std::frexp;
    using std::ldexp;
    using std::log;
    using std::log10;
    using std::modf;
    // using std::exp2;
    // using std::expm1;
    // using std::ilogb;
    // using std::log1p;
    // using std::log2;
    // using std::logb;
    // using std::scalbn;
    // using std::scalbln;

    // power functions
    using std::pow;
    using std::sqrt;
    // using std::cbrt;
    // using std::hypot;

    // error and gamma functions
    // using std::erf;
    // using std::erfc;
    // using std::tgamma;
    // using std::lgamma;

    // rounding and remainder functions
    using std::ceil;
    using std::floor;
    using std::fmod;
    // using std::trunc;
    // using std::round;
    // using std::lround;
    // using std::llround;
    // using std::rint;
    // using std::lrint;
    // using std::llrint;
    // using std::nearbyint;
    // using std::remainder;
    // using std::remquo;

    // floating-point manipulation functions
    // using std::copysign;
    // using std::nan;
    // using std::nanf;
    // using std::nanl;
    // using std::nextafter;
    // using std::nexttoward;

    // minimum, maximum, difference functions
    // using std::fdim;
    // using std::fmax;
    // using std::fmin;

    // other functions
    using std::fabs;
    using std::abs;
    // using std::fma;

    // classification functions
    using std::fpclassify;
    using std::isfinite;
    using std::isinf;
    using std::isnan;
    using std::isnormal;
    using std::signbit;

    // comparison functions
    using std::isgreater;
    using std::isgreaterequal;
    using std::isless;
    using std::islessequal;
    using std::islessgreater;
    using std::isunordered;

    // types
    // using std::double_t;
    // using std::float_t;
    
*/

} // namespace math