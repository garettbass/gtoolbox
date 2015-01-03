#pragma once
#include <cassert>
#include <gtoolbox/nav/forward/types.h>
#include <gtoolbox/nav/inline.h>

namespace nav {

    //--------------------------------------------------------------------------

    struct tile
    {
        union {
            struct {
                uint32_t
                row   : 15,
                col   : 15;

                layer_index
                layer : 1;

                bool
                visited : 1;
            };
            uint32_t loc : 31;
            uint32_t bits;
        };

    public: // tests

        static void tests();

    public: // structors

        tile() : bits(0) {}

        nav_inline
        tile(unsigned row, unsigned col, layer_index layer)
        : row(row), col(col), layer(layer), visited(false)
        {
            assert(row <= tile(unreachable()).row);
            assert(col <= tile(unreachable()).col);
        }

        nav_inline
        tile(nav::unreachable)
        : row(32767), col(32767), layer(TERRAIN), visited(false)
        {}

    public: // operators

        nav_inline
        operator const nav::cell& () const { return *(nav::cell*)this; }
    };

    env_static_assert(sizeof(tile) == sizeof(uint32_t));

    nav_inline bool operator == (tile a, tile b) { return a.loc == b.loc; }
    nav_inline bool operator != (tile a, tile b) { return a.loc != b.loc; }

    //--------------------------------------------------------------------------

} // namespace nav