#pragma once

//------------------------------------------------------------------------------
/** @name CPU recognition macros

    Identifies the CPU architecture targeted by the build toolchain.

    One of the following is defined to the integer constant 1

        env_cpu_arm
        env_cpu_ppc (and one of: env_cpu_ppc_32, env_cpu_ppc_64)
        env_cpu_x86 (and one of: env_cpu_x86_32, env_cpu_x86_64)

    Additionally, the following are defined:

        env_cpu_id - a valid C++ identifier, e.g. arm, ppc_32, x86_64
        env_cpu_name - a string literal name for the CPU architecture
        env_cpu_bits - an integer constant, 32 or 64
        env_cpu_file(file) - include path to CPU-specific file, e.g. "arm/file"
*/

#if defined(ARM)     || \
    defined(__arm)   || \
    defined(__arm__) || \
    defined(_ARM)
    #define env_cpu_arm   (1)
    #define env_cpu_id    arm
    #define env_cpu_name "ARM"
    #define env_cpu_bits  (32)
#elif \
    defined(__powerpc64__) || \
    defined(__ppc64__)
    #define env_cpu_ppc    (1)
    #define env_cpu_ppc_64 (1)
    #define env_cpu_id     ppc_64
    #define env_cpu_name  "PowerPC 64"
    #define env_cpu_bits   (64)
#elif \
    defined(__POWERPC__) || \
    defined(__PPC__)     || \
    defined(__powerpc__) || \
    defined(__ppc__)     || \
    defined(_POWER)      || \
    defined(powerpc)
    #define env_cpu_ppc    (1)
    #define env_cpu_ppc_32 (1)
    #define env_cpu_id     ppc_32
    #define env_cpu_name  "PowerPC"
    #define env_cpu_bits   (32)
#elif \
    defined(__x86_64__) || \
    defined(_M_X64)
    #define env_cpu_x86    (1)
    #define env_cpu_x86_64 (1)
    #define env_cpu_id     x86_64
    #define env_cpu_name  "x86-64"
    #define env_cpu_bits   (64)
#elif \
    defined(__386__)  || \
    defined(__i386__) || \
    defined(__X86__)  || \
    defined(_M_IX86)  || \
    defined(i386)
    #define env_cpu_x86    (1)
    #define env_cpu_x86_32 (1)
    #define env_cpu_id     x86_32
    #define env_cpu_name  "x86"
    #define env_cpu_bits   (32)
#else
    #define env_cpu_id     unsupported_cpu
    #define env_cpu_name  "unsupported CPU"
    #define env_cpu_bits   (0)
#endif

#define env_cpu_file(file) env_quote(./cpu/env_cpu_id/file)

//------------------------------------------------------------------------------