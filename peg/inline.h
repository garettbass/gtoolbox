#pragma once

#define PEG_FORCEINLINE 1 // may take a while to link

#if PEG_FORCEINLINE

    #include <gtoolbox/env/compiler.h>

    #define peg_inline env_forceinline

#else

    #define peg_inline inline

#endif