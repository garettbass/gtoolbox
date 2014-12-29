#pragma once
#include <iso646.h>

// keywords and pseudo-keywords -----------------------------------------------

#define _ALLOW_KEYWORD_MACROS
#if env_compiler_msvc < 10
    #define final
    #define override
#elif env_compiler_msvc == 10
    #define final sealed
#endif
#define env_alignof(type) __alignof(type)
#define env_aligned(size) __declspec(align(size))
#define env_thread        __declspec(thread)

// calling conventions --------------------------------------------------------

#define env_cdecl       __cdecl
#define env_stdcall     __stdcall
#define env_fastcall    __fastcall
#define env_dllimport   __declspec(dllimport)
#define env_dllexport   __declspec(dllexport)

// utility macros -------------------------------------------------------------

#define env_deprecated(msg)    __declspec(deprecated("DEPRECATED: " msg))
#define env_poison(identifier) __pragma(deprecated(identifier))
#define env_forceinline        __forceinline
#define env_noinline           __declspec(noinline)
#define env_noreturn           __declspec(noreturn)
#define env_unreachable        __assume(0)

// static_assert(expr, msg) fallback ------------------------------------------

#if (env_compiler_msvc < 10)
    #define static_assert(expr, msg) \
        enum { env_concat(static_assert_, __COUNTER__) = 1 / (!!(expr)) }
#endif

#define env_static_assert(...) \
        static_assert((__VA_ARGS__), "static assertion failed: " #__VA_ARGS__)

// warnings & errors ----------------------------------------------------------

#pragma warning(default: 4062) // enum not handled by switch, no default
#pragma warning(error:   4099) // class/struct declaration
#pragma warning(disable: 4200) // zero-sized array in struct/union
#pragma warning(disable: 4201) // nameless struct/union
#pragma warning(default: 4263) // member function hides virtual function
#pragma warning(default: 4265) // virtual functions, no virtual destructor
#pragma warning(error:   4324) // structure was padded due to alignment

///////////////////////////////////////////////////////////////////////////////
