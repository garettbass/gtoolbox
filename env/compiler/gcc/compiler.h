#pragma once
#include <iso646.h>

// keywords and pseudo-keywords -----------------------------------------------

#if !defined(env_compiler_clang) && (env_compiler_gcc < 47)
    #define final
    #define override
#endif
#define env_aligned(size)   __attribute__((aligned(size)))
#define env_alignof(type)   __alignof__(type)
#define env_thread          __thread

// calling conventions --------------------------------------------------------

#define env_cdecl       __attribute__((cdecl))
#define env_stdcall     __attribute__((stdcall))
#define env_fastcall    __attribute__((fastcall))
#define env_dllimport   __attribute__((dllimport))
#define env_dllexport   __attribute__((dllexport)) \
                        __attribute__((__visibility__("default")))

// utility macros -------------------------------------------------------------

#define env_deprecated(msg)    __attribute__((deprecated))
#define env_poison(identifier) _Pragma(env_quote(GCC poison identifier))
#define env_forceinline        __attribute__((always_inline)) inline
#define env_noinline           __attribute__((noinline))
#define env_noreturn           __attribute__((noreturn))
#define env_unreachable        __builtin_unreachable()

// static_assert(expr, msg) fallback ------------------------------------------

#if !defined(env_compiler_clang) && (env_compiler_gcc < 45)
    #define static_assert(expr, msg) \
        enum { env_concat(static_assert_, __COUNTER__) = 1 / (!!(expr)) }
#endif

#define env_static_assert(...) static_assert((__VA_ARGS__), #__VA_ARGS__)

// warnings & errors ----------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
