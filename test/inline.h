#pragma once
#include <gtoolbox/env/compiler.h>

#define TEST_FORCEINLINE 1

#if TEST_FORCEINLINE

    #define test_inline env_forceinline

#else

    #define test_inline inline

#endif