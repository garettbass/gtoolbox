#pragma once
#include "cpu.h"

//------------------------------------------------------------------------------
/** @name Endianness macros

    Identifies the CPU endianness targeted by the build toolchain.

    One of the following defines the endianness to an integer constant
    representing the byte order:

        env_endian_little (e.g. x86)
        env_endian_big    (e.g. PPC, network byte order)

    Additionally, the following are defined:

        env_endian - as either env_endian_little, or env_endian_big
        env_endian_id - a valid C++ identifier, e.g. little_endian, big_endian
        env_endian_name - a string literal name for the CPU endianness
*/

#if (env_cpu_x86) || (env_cpu_arm && !__BIG_ENDIAN__)
    #define env_endian_little (0x01020304u)
    #define env_endian_big    (0)
    #define env_endian        env_endian_little
    #define env_endian_id     little_endian
    #define env_endian_name  "little endian"
    static_assert(0x31323334 == '1234', "byte order mismatch");
#elif (env_cpu_ppc || env_cpu_ppc64)
    #define env_endian_little (0)
    #define env_endian_big    (0x04030201u)
    #define env_endian        env_endian_big
    #define env_endian_id     big_endian
    #define env_endian_name  "big endian"
    static_assert(0x34333231 == '1234', "byte order mismatch");
#else
    #error "unsupported endianness"
#endif

//------------------------------------------------------------------------------