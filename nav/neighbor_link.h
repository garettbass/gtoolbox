#pragma once
#include <gtoolbox/nav/forward/types.h>
#include <gtoolbox/nav/inline.h>
#include <gtoolbox/math/functions.h>

namespace nav {

    //--------------------------------------------------------------------------

    struct neighbor_link
    {
    public: // types

        using pair = std::pair<direction_index, layer_index>;

        using pairs = std::initializer_list<pair>;

    public: // fields

        union {
            struct {
                uint8_t
                neighbor_exists : 4,
                neighbor_layers : 4;
            };

            struct {
                bool
                north_neighbor_exists : 1,
                south_neighbor_exists : 1,
                east_neighbor_exists  : 1,
                west_neighbor_exists  : 1;

                layer_index
                north_neighbor_layer : 1,
                south_neighbor_layer : 1,
                east_neighbor_layer  : 1,
                west_neighbor_layer  : 1;
            };

            uint8_t bits;
        };

    public: // tests

        static void tests();

    public: // structors

        neighbor_link() : bits(0) {}

        neighbor_link(pairs neighbors)
        {
            for (auto& neighbor : neighbors)
            {
                exists(neighbor.first, true);
                layer(neighbor.first, neighbor.second);
            }
        }

    public: // queries

        template <
            direction_index    NEIGHBOR,
            direction_index... NEIGHBORS>
        nav_inline
        bool all() const
        { return math::all(exists(NEIGHBOR), exists(NEIGHBORS)...); }

        template <
            direction_index    NEIGHBOR,
            direction_index... NEIGHBORS>
        nav_inline
        bool any() const
        { return math::any(exists(NEIGHBOR), exists(NEIGHBORS)...); }

        nav_inline
        bool none() const
        { return not any<NORTH, SOUTH, EAST, WEST>(); }

        template <
            direction_index    NEIGHBOR,
            direction_index... NEIGHBORS>
        nav_inline
        bool only() const
        {
            neighbor_link n;
            math::all(n.exists(NEIGHBOR, true), n.exists(NEIGHBORS, true)...);
            return neighbor_exists == n.neighbor_exists;
        }

        nav_inline
        bool exists(direction_index n) const
        { return (neighbor_exists >> n & true); }

        nav_inline
        bool exists(direction_index n, bool exists)
        {
            neighbor_exists &= ~(1 << n);
            neighbor_exists |= exists << n;
            return exists;
        }

        nav_inline
        layer_index layer(direction_index n) const
        { return layer_index((neighbor_layers >> n) & 1); }

        nav_inline
        void layer(direction_index n, layer_index layer)
        {
            neighbor_layers &= ~(1 << n);
            neighbor_layers |= (layer << n);
        }
    };

    env_static_assert(sizeof(neighbor_link) == sizeof(uint8_t));

    nav_inline
    bool
    operator == (neighbor_link a, neighbor_link b) { return a.bits == b.bits; }

    nav_inline
    bool
    operator != (neighbor_link a, neighbor_link b) { return a.bits != b.bits; }

    //--------------------------------------------------------------------------

} // namespace nav