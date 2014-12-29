#pragma once
#include "utility.h"

//------------------------------------------------------------------------------
/** @name Compiler recognition macros

    Identifies the compiler used by the build toolchain.

    One or more of the following defines the compiler version as an integer:

        env_compiler_clang
        env_compiler_gcc
        env_compiler_msvc

    Additionally, the following are defined:

        env_compiler_id - a valid C++ identifier, e.g. gcc, msvc
        env_compiler_name - a string literal name for the compiler
        env_compiler_file(file) - expands to, e.g. "msvc/file"

    For cross-compiler compatibility, the following macros are defined via
    #include env_compiler_file(_config.h):

        final
        override

        static_assert(expr, msg)
        
        env_static_assert(expr)

        env_aligned(size)
        env_alignof(type)

        env_thread

        env_cdecl
        env_stdcall
        env_fastcall
        env_dllexport
        env_dllimport

        env_deprecated(msg)
        env_poison(identifier)
        env_forceinline
        env_noinline
        env_noreturn
        env_unreachable

        env_objc
        env_objc_arc
        env_objc_class(name)

*/

#if defined(__GNUC__) && defined(__clang__)

    // Clang
    #define env_compiler_id clang
    #define env_compiler_clang (__clang_major__ * 10 + __clang_minor__)
    #define env_compiler_name "Clang " __clang_version__

#elif defined(__GNUC__) && !defined(__clang__)

    // GCC
    #define env_compiler_gcc (__GNUC__ * 10 + __GNUC_MINOR__)
    #define env_compiler_id gcc
    #define env_compiler_name "GCC " __VERSION__

#elif defined(_MSC_VER)

    // MSVC
    #define env_compiler_id msvc
    #if (_MSC_VER >= 1800)
        #define env_compiler_msvc 12
        #define env_compiler_name "Microsoft Visual C++ 12 (2013)"
    #elif (_MSC_VER >= 1700)
        #define env_compiler_msvc 11
        #define env_compiler_name "Microsoft Visual C++ 11 (2012)"
    #elif (_MSC_VER >= 1600)
        #define env_compiler_msvc 10
        #define env_compiler_name "Microsoft Visual C++ 10 (2010)"
    #elif (_MSC_VER == 1500)
        #define env_compiler_msvc 9
        #define env_compiler_name "Microsoft Visual C++ 9 (2008)"
    #elif (_MSC_VER == 1400)
        #define env_compiler_msvc 8
        #define env_compiler_name "Microsoft Visual C++ 8 (2005)"
    #endif

#else

    #define env_compiler_id    unsupported_compiler
    #define env_compiler_name "unsupported compiler"
    
#endif

#define env_compiler_file(file) env_quote(./compiler/env_compiler_id/file)

#include env_compiler_file(compiler.h)

//------------------------------------------------------------------------------