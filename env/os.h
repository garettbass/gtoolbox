#pragma once
#include "./utility.h"

//------------------------------------------------------------------------------
/** @name OS recognition macros

    Identifies the OS targeted by the build toolchain.

    One of the following is defined to the integer constant 1

        env_os_windows
        env_os_ios
        env_os_osx
        env_os_LINUX

    Additionally, the following are defined:

        env_os_id - a valid C++ identifier, e.g. Windows, IOS, OSX, Linux
        env_os_name - a string literal name for the OS

        env_os_file(file) - include path to OS-specific file, e.g. "windows/file"

        env_os_type(name) - namespace-qualify an OS-specific type
*/

#if defined(_WIN32)

    // Windows
    #define env_os_windows (1)
    #define env_os_id      windows
    #define env_os_name   "Windows"
    #define env_os_windows_required \
        static_assert(env_os_windows, "Windows required");

#elif defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)

    // iOS
    #define env_os_ios   (1)
    #define env_os_id    ios
    #define env_os_name "iOS"
    #define env_os_ios_required \
        static_assert(env_os_ios, "iOS required");

#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)

    // OS X
    #define env_os_osx   (1)
    #define env_os_id    osx
    #define env_os_name "OS X"
    #define env_os_osx_required \
        static_assert(env_os_osx, "OS X required");

#elif defined(linux) || defined(__linux) || defined(__linux__)

    // Linux
    #define env_os_linux (1)
    #define env_os_id    linux
    #define env_os_name "Linux"
    #define env_os_linux_required \
        static_assert(env_os_osx, "Linux required");

#else

    #define env_os_id    unsupported_os
    #define env_os_name "unsupported OS"
    
#endif

#define env_os_file(file) env_quote(./os/env_os_id/file)

#define env_os_type(name) \
    env_os_id::name

//------------------------------------------------------------------------------