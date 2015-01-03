#pragma once
#include <cassert>
#include <gtoolbox/nav/tile.h>

namespace nav {

    //--------------------------------------------------------------------------

    struct cell
    {
        union {
            struct {
                uint32_t
                row    : 15,
                col    : 15,
                unused :  2;
            };
            uint32_t loc    : 30;
            uint32_t bits;
        };

    public: // tests

        static void tests() { tile::tests(); }

    public: // structors

        cell() : bits(0) {}

        nav_inline
        cell(unsigned row, unsigned col)
        : row(row), col(col), unused(0)
        {
            assert(row <= cell(unreachable()).row);
            assert(col <= cell(unreachable()).col);
        }

        nav_inline
        cell(unreachable)
        : row(32767), col(32767), unused(0)
        {}

    public: // operators

        nav_inline
        nav::tile
        operator [] (layer_index layer) const
        { return { row, col, layer }; }
    };

    env_static_assert(sizeof(cell) == sizeof(uint32_t));
    env_static_assert(sizeof(cell) == sizeof(tile));

    nav_inline bool operator == (cell a, cell b) { return a.loc == b.loc; }
    nav_inline bool operator != (cell a, cell b) { return a.loc != b.loc; }

    //--------------------------------------------------------------------------

} // namespace nav