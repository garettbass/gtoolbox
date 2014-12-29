#pragma once
#include <zna/source/thirdParty/gtoolbox/nav/inline.h>
#include <zna/source/thirdParty/gtoolbox/nav/cell.h>
#include <zna/source/thirdParty/gtoolbox/math/functions.h>

namespace nav {

    //--------------------------------------------------------------------------

    class map_index
    {
    protected: // fields

        const unsigned _col_count = 0;

        const unsigned _row_count = 0;

        const unsigned _row_stride = 0;

        const unsigned _row_shift = 0;

    protected: // structors

        map_index() = default;

        map_index(const map_index&) = default;

        map_index(unsigned row_count, unsigned col_count)
        : _col_count  (col_count ? col_count : 1)
        , _row_count  (row_count ? row_count : 1)
        , _row_stride (math::ceilpow2(_col_count))
        , _row_shift  (math::log2(_row_stride))
        {}

    public: // properties

        inline unsigned col_count() const { return _col_count; }

        inline unsigned row_count() const { return _row_count; }

        inline unsigned row_stride() const { return _row_stride; }

        inline unsigned cell_count() const { return _row_count * _row_stride; }

        inline unsigned tile_count() const { return cell_count() * LAYER_COUNT; }

    public: // methods

        nav_inline
        unsigned
        cell_index(unsigned row, unsigned col) const
        { assert(col < _col_count); return row_index(row) + col; }

        nav_inline
        unsigned
        cell_index(const nav::cell& c) const
        { return cell_index(c.row, c.col); }

        nav_inline
        unsigned
        cell_index(const nav::tile& t) const
        { return cell_index(t.row, t.col); }

        nav_inline
        unsigned
        row_index(unsigned row) const
        { assert(row < _row_count); return row << _row_shift; }

        nav_inline
        int
        neighbor_offset(direction_index n) const
        {
            //   n
            // w + e
            //   s
            const int north_offset = -int(n == NORTH) * _row_stride;
            const int south_offset = +int(n == SOUTH) * _row_stride;
            const int east_offset  = +int(n == EAST);
            const int west_offset  = -int(n == WEST);

            return
                north_offset +
                south_offset +
                east_offset  +
                west_offset;
        }

    };

    //--------------------------------------------------------------------------

} // namespace nav