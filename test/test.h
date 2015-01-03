#pragma once
#include <gtoolbox/env/cpu.h>
#include <gtoolbox/env/os.h>
#include <gtoolbox/test/inline.h>

//------------------------------------------------------------------------------
// test_breakpoint() - a platform-specific breakpoint statement

#if env_cpu_arm
    #define test_breakpoint()                                                  \
        __asm__ __volatile__(                                                  \
            "mov r0, %0\nmov r1, %1\nmov r12, #37\nswi 128\n"                  \
            : : "r" (test::getpid()), "r" (2) : "r12", "r0", "r1", "cc")

#elif env_cpu_x86
    #define test_breakpoint() __asm__ __volatile__("int3")

#else
    #define test_breakpoint() ((void)0)
#endif

//------------------------------------------------------------------------------

#define test_scope __PRETTY_FUNCTION__, __FILE__, __LINE__

//------------------------------------------------------------------------------
// test_assert(expr) - check 'expr' in all builds, abort on failure

#define test_assert(...)                                                       \
    do if (not bool(__VA_ARGS__)) {                                            \
        test::assert_failed(test_scope, #__VA_ARGS__);                         \
        if (test::debugger_attached()) test_breakpoint();                      \
        test::abort();                                                         \
    } while (test::eval(0))

//------------------------------------------------------------------------------
// test_check(expr) - check 'expr' in all builds, continue after failure

#define test_check(...)                                                        \
    do if (not bool(__VA_ARGS__)) {                                            \
        test::check_failed(test_scope, #__VA_ARGS__);                          \
        if (test::debugger_attached()) test_breakpoint();                      \
        test::flush();                                                         \
    } while (test::eval(0))

//------------------------------------------------------------------------------

namespace test {

    void
    abort();

    template <typename T>
    test_inline
    const T&
    eval(const T& expr)
    { return expr; }

    void
    flush();

    int
    getpid();

    bool
    debugger_attached();

    void
    assert_failed(
        const char* func,
        const char* file,
        unsigned    line,
        const char* expr);

    void
    check_failed(
        const char* func,
        const char* file,
        unsigned    line,
        const char* expr);

} // namespace test