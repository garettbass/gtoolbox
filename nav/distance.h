#pragma once
#include <gtoolbox/math/functions.h>
#include <gtoolbox/nav/inline.h>
#include <gtoolbox/nav/tile.h>

namespace nav {

    struct distance
    {
        // http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html#heuristics-for-grid-maps

        nav_inline
        static
        float
        diagonal(const nav::tile a, const nav::tile b)
        {
            const float drow   = float(a.row)   - float(b.row);
            const float dcol   = float(a.col)   - float(b.col);
            const float dlayer = float(a.layer) - float(b.layer);

            return math::max(drow, dcol, dlayer);
        }

        nav_inline
        static
        float
        euclidean_sq(const nav::tile a, const nav::tile b)
        {
            const float drow   = float(a.row)   - float(b.row);
            const float dcol   = float(a.col)   - float(b.col);
            const float dlayer = float(a.layer) - float(b.layer);

            const float drow_sq   = drow    * drow;
            const float dcol_sq   = dcol    * dcol;
            const float dlayer_sq = dlayer * dlayer;

            return drow_sq + dcol_sq + dlayer_sq;
        }

        nav_inline
        static
        float
        euclidean(const nav::tile a, const nav::tile b)
        {
            return std::sqrt(euclidean_sq(a, b));
        }

        nav_inline
        static
        float
        manhattan(const nav::tile a, const nav::tile b)
        {
            const float drow   = float(a.row)   - float(b.row);
            const float dcol   = float(a.col)   - float(b.col);
            const float dlayer = float(a.layer) - float(b.layer);

            return drow + dcol + dlayer;
        }

    };

} // namespace nav