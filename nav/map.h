#pragma once
#include <array>
#include <limits>
#include <zna/source/thirdParty/gtoolbox/nav/height_map.h>
#include <zna/source/thirdParty/gtoolbox/nav/neighbor_link.h>
#include <zna/source/thirdParty/gtoolbox/nav/obstacles.h>
#include <zna/source/thirdParty/gtoolbox/nav/point.h>

namespace nav {

    //--------------------------------------------------------------------------

    class map : public map_index
    {
        using base = map_index;

    public: // types

        using cell_heights = std::array<float, LAYER_COUNT>;

        using cell_obstacles = std::array<obstacles, LAYER_COUNT>;

        using cell_neighbors = std::array<neighbor_link, LAYER_COUNT>;

        using cell_islands = std::array<uint32_t, LAYER_COUNT>;

        using cell_costs = std::array<uint8_t, LAYER_COUNT>;

        struct tile_info {
            unsigned      row;
            unsigned      col;
            layer_index   layer;
            float         height;
            uint32_t      island;
            obstacles     obstacles;
            neighbor_link neighbors;
            uint8_t       cost;
        };

        struct cell_info {
            unsigned       row;
            unsigned       col;
            cell_heights   heights;
            cell_islands   islands;
            cell_obstacles obstacles;
            cell_neighbors neighbor_link;
            cell_costs     costs;

            tile_info
            operator [] (layer_index layer) const
            {
                return {
                    row, col, layer,
                    heights  [layer],
                    islands  [layer],
                    obstacles[layer],
                    neighbor_link[layer],
                    costs    [layer]
                };
            }
        };

    public: // constants

        enum : unsigned { DEFAULT_MAX_SLOPE_DEGREES = 30 };

    private: // fields

        cell_heights*   _heights   = nullptr;
        cell_islands*   _islands   = nullptr;
        cell_obstacles* _obstacles = nullptr;
        cell_neighbors* _neighbors = nullptr;
        cell_costs*     _costs     = nullptr;
        uint32_t*       _scratch   = nullptr;

        float _max_height_delta = 0;

        bool _has_unresolved_barriers = false;

        uint32_t _state = 0;

    public: // tests

        static void tests();

    private: // structors

        map(const map&) = default;

    public: // structors

        map() = default;

        map(unsigned rows, unsigned cols);

        map(const height_map&, unsigned = DEFAULT_MAX_SLOPE_DEGREES);

        map(map&& m);

        map& operator = (map&& g);

       ~map();

    public: // operators

        cell_info
        operator () (unsigned row, unsigned col) const
        {
            const auto i = cell_index(row, col);
            return {
                row, col,
                _heights  [i],
                _islands  [i],
                _obstacles[i],
                _neighbors[i],
                _costs    [i]
            };
        }

        tile_info
        operator () (unsigned row, unsigned col, layer_index layer) const
        {
            const auto i = cell_index(row, col);
            return {
                row, col, layer,
                _heights  [i][layer],
                _islands  [i][layer],
                _obstacles[i][layer],
                _neighbors[i][layer],
                _costs    [i][layer]
            };
        }

    public: // properties

        const cell_heights*   heights()   const { return _heights;   }
        const cell_obstacles* obstacles() const { return _obstacles; }
        const cell_neighbors* neighbors() const { return _neighbors; }
        const cell_islands*   islands()   const { return _islands;   }
        const cell_costs*     costs()     const { return _costs;     }

        float max_height_delta() const { return _max_height_delta; }

    public: // queries

        nav::cell
        cell(nav::point p) const
        {
            const auto max_row = _row_count - 1;
            const auto max_col = _col_count - 1;

            const auto row = math::min<unsigned>(std::floor(p.row), max_row);
            const auto col = math::min<unsigned>(std::floor(p.col), max_col);

            return { row, col };
        }

        uint8_t
        cost(nav::tile t) const
        { return _costs[cell_index(t)][t.layer]; }

        float
        height(nav::tile t) const
        { return _heights[cell_index(t)][t.layer]; }

        uint32_t
        island(nav::tile t) const
        { return _islands[cell_index(t)][t.layer]; }

        nav::neighbor_link
        neighbors(nav::tile t) const
        { return _neighbors[cell_index(t)][t.layer]; }

        uint8_t
        obstacle_count(nav::tile t) const
        { return _obstacles[cell_index(t)][t.layer].count(); }

        nav::obstacles
        obstacles(nav::tile t) const
        { return _obstacles[cell_index(t)][t.layer]; }

        bool
        obstructed(nav::tile t) const
        { return _obstacles[cell_index(t)][t.layer].obstructed(); }

        nav::point
        point(nav::tile t) const
        { return { float(t.row) + 0.5f, float(t.col) + 0.5f, height(t) }; }

        uint32_t
        state() const
        { return _state; }

        nav::tile
        tile(nav::point p) const
        {
            const auto max_row = _row_count - 1;
            const auto max_col = _col_count - 1;

            const auto row = math::min<unsigned>(std::floor(p.row), max_row);
            const auto col = math::min<unsigned>(std::floor(p.col), max_col);

            const auto& heights = _heights[cell_index(row, col)];

            const auto dh_terrain = std::abs(p.height - heights[TERRAIN]);
            const auto dh_bridges = std::abs(p.height - heights[BRIDGES]);

            return { row, col, (dh_terrain < dh_bridges) ? TERRAIN : BRIDGES };
        }

    public: // barrier management

        void add_barrier(nav::tile t)
        { modify_barriers(t, +1); }

        void add_barrier(nav::point min, nav::point max)
        { modify_barriers(min, max, +1); }

        void remove_barrier(nav::tile t)
        { modify_barriers(t, -1); }

        void remove_barrier(nav::point min, nav::point max)
        { modify_barriers(min, max, -1); }

        void modify_barriers(nav::tile, int8_t delta);

        void modify_barriers(nav::point min, nav::point max, int8_t delta);

        void resolve_barriers();

        bool has_unresolved_barriers() const
        { return _has_unresolved_barriers; }

    public: // cost management

        void
        add_cost(
            nav::point center,
            unsigned   radius,
            unsigned   height,
            uint8_t    delta = 1)
        { modify_costs(center, radius, height, +int8_t(delta)); }

        void
        remove_cost(
            nav::point center,
            unsigned   radius,
            unsigned   height,
            uint8_t    delta = 1)
        { modify_costs(center, radius, height, -int8_t(delta)); }

        void
        modify_costs(
            nav::point center,
            unsigned   radius,
            unsigned   height,
            int8_t     delta);
    };

    //--------------------------------------------------------------------------

} // namespace nav