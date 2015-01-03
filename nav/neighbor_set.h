#pragma once
#include <gtoolbox/nav/forward/types.h>

namespace nav {

    //--------------------------------------------------------------------------

    class neighbor_set
    {
        nav::tile  _tiles[8];

        nav::tile* _end = _tiles;

    public: // structors

        neighbor_set() = default;

        nav_inline
        neighbor_set(const nav::map& map, const nav::tile t)
        { operator()(map, t); }

    public: // operators

        nav_inline
        neighbor_set&
        operator() (const nav::map& map, const nav::tile t)
        {
            _end = _tiles;

            const auto t_neighbors = map.neighbors(t);

            bool nw_exists = false; layer_index nw_layer = TERRAIN;
            bool sw_exists = false; layer_index sw_layer = TERRAIN;
            bool ne_exists = false; layer_index ne_layer = TERRAIN;
            bool se_exists = false; layer_index se_layer = TERRAIN;

            if (t_neighbors.exists(WEST))
            {
                const tile w(t.row, t.col - 1, t_neighbors.layer(WEST));

                *_end++ = w;

                const auto w_neighbors = map.neighbors(w);

                nw_exists = w_neighbors.exists(NORTH);
                nw_layer  = w_neighbors.layer(NORTH);
                sw_exists = w_neighbors.exists(SOUTH);
                sw_layer  = w_neighbors.layer(SOUTH);
            }

            if (t_neighbors.exists(EAST))
            {
                const tile e(t.row, t.col + 1, t_neighbors.layer(EAST));

                *_end++ = e;

                const auto e_neighbors = map.neighbors(e);

                ne_exists = e_neighbors.exists(NORTH);
                ne_layer  = e_neighbors.layer(NORTH);
                se_exists = e_neighbors.exists(SOUTH);
                se_layer  = e_neighbors.layer(SOUTH);
            }

            if (t_neighbors.exists(NORTH))
            {
                const tile n(t.row - 1, t.col, t_neighbors.layer(NORTH));

                if (nw_exists & (nw_layer == n.layer))
                    *_end++ = tile(t.row - 1, t.col - 1, nw_layer);

                *_end++ = n;

                if (ne_exists & (ne_layer == n.layer))
                    *_end++ = tile(t.row - 1, t.col + 1, ne_layer);
            }

            if (t_neighbors.exists(SOUTH))
            {
                const tile s(t.row + 1, t.col, t_neighbors.layer(SOUTH));

                if (sw_exists & (sw_layer == s.layer))
                    *_end++ = tile(t.row + 1, t.col - 1, sw_layer);

                *_end++ = s;

                if (se_exists & (se_layer == s.layer))
                    *_end++ = tile(t.row + 1, t.col + 1, se_layer);
            }

            return *this;
        }


    public: // iterators

        const nav::tile* begin() const { return _tiles; }

        const nav::tile* end() const { return _end; }
    };

    //--------------------------------------------------------------------------

} // namespace nav