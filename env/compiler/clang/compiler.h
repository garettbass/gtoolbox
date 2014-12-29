#pragma once
#include "../gcc/compiler.h"

#if __OBJC__

    #define env_objc 1

    #define env_objc_arc __has_feature(objc_arc)

    #define env_objc_class(name) @class name;

    #define env_objc_release(nsobject) [&](){                  \
        void* object = (__bridge_retained void*)nsobject;      \
        nsobject = nil;                                        \
        const CFIndex count = CFGetRetainCount(object);        \
        for (CFIndex i = 0; i < count; ++i) CFRelease(object); \
    }()

#else

    #define env_objc 0

    #define env_objc_arc 0

    #define env_objc_class(name) typedef void name;

#endif // __OBJC__