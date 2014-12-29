#pragma once
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <iostream>
#include "./substring.h"

namespace xtd {

    inline void echo(const char* expr, bool b)
    { std::cout << expr << " == " << (b ? "true\n" : "false\n"); }

    inline void echo(const char* expr, const char* string)
    { std::cout << expr << " == \"" << (string ? string : "<null>") << "\"\n"; }

    inline void echo(const char* expr, const std::string& string)
    { std::cout << expr << " == \"" << string << "\"\n"; }

    inline void echo(const char* expr, const substring& string)
    { std::cout << expr << " == \"" << string << "\"\n"; }

    template <typename T>
    inline void echo(const char* expr, const T* pointer)
    { std::cout << expr << " == " << (void*)pointer << "\n"; }

    template <typename T>
    inline void echo(const char* expr, const T& object)
    { std::cout << expr << " == " << object << "\n"; }

}

#define xtd_echo(expr) ::xtd::echo(#expr, (expr))
