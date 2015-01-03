#pragma once
#include <gtoolbox/env/compiler.h>

#define NAV_FORCEINLINE 1

#if NAV_FORCEINLINE

    #define nav_inline env_forceinline

#else

    #define nav_inline inline

#endif