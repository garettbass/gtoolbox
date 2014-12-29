#pragma once
#include <zna/source/thirdParty/gtoolbox/nav/inline.h>

namespace nav {

    //--------------------------------------------------------------------------

    struct obstacles
    {
        union {
            struct {
                bool
                terrain_slope : 1;
                
                uint8_t
                barrier_count : 7;
            };

            uint8_t bits;
        };

        obstacles() : bits(0) {}

        nav_inline
        uint8_t
        count() const
        { return barrier_count + terrain_slope; }

        nav_inline
        bool
        obstructed() const
        { return bits != 0; }
    };

    env_static_assert(sizeof(obstacles) == sizeof(uint8_t));

    //--------------------------------------------------------------------------

} // namespace nav