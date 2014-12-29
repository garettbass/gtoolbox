#include <zna/source/thirdParty/gtoolbox/nav/map.h>
#include <zna/source/thirdParty/gtoolbox/nav/point.h>
#include <zna/source/thirdParty/gtoolbox/nav/private/navigator.find_nearest_tile_in_island.h>

namespace nav {

    using cell_heights = map::cell_heights;

    using cell_obstacles = map::cell_obstacles;

    using cell_neighbors = map::cell_neighbors;

    using cell_islands = map::cell_islands;

    using cell_costs = map::cell_costs;

    //--------------------------------------------------------------------------

    static
    nav_inline
    float
    distance_sq(
        const nav::point& a,
        const float       b_row,
        const float       b_col,
        const float       b_height)
    {
        const float dx = a.row    - b_row;
        const float dy = a.col    - b_col;
        const float dz = a.height - b_height;

        const float dx_sq = dx * dx;
        const float dy_sq = dy * dy;
        const float dz_sq = dz * dz;

        return dx_sq + dy_sq + dz_sq;
    }

    //--------------------------------------------------------------------------

    template <layer_index CELL_LAYER>
    static
    nav_inline
    void
    find_nearest_tile_in_island_cell(
        const uint32_t      island,
        const nav::point&   goal_point,
        const unsigned      row,
        const float         frow,
        const unsigned      col,
        const float         fcol,
        const cell_heights* cell_heights,
        const cell_islands* cell_islands,
              float&        nearest_distance_sq,
              nav::tile&    nearest_tile)
    {
        const uint32_t cell_island = cell_islands[0][CELL_LAYER];

        if (cell_island != island)
            return;

        const float cell_height = cell_heights[0][CELL_LAYER];

        const float cell_distance_sq =
            distance_sq(goal_point, frow, fcol, cell_height);

        if (nearest_distance_sq > cell_distance_sq)
        {
            nearest_distance_sq = cell_distance_sq;
            nearest_tile.row    = row;
            nearest_tile.col    = col;
            nearest_tile.layer  = CELL_LAYER;
        }
    }

    //--------------------------------------------------------------------------

    static
    nav_inline
    void
    find_nearest_tile_in_island_row(
        const uint32_t      island,
        const nav::point&   goal_point,
        const unsigned      row,
        const float         frow,
        const cell_heights* row_heights,
        const cell_islands* row_islands,
        const unsigned      col_count,
              float&        nearest_distance_sq,
              nav::tile&    nearest_tile)
    {
        auto* cell_heights = row_heights;
        auto* cell_islands = row_islands;

        for (unsigned col = 0; col < col_count; ++col)
        {
            const float fcol = col;

            find_nearest_tile_in_island_cell<TERRAIN>(
                island,
                goal_point,
                row, frow,
                col, fcol,
                cell_heights,
                cell_islands,
                nearest_distance_sq,
                nearest_tile);
            find_nearest_tile_in_island_cell<BRIDGES>(
                island,
                goal_point,
                row, frow,
                col, fcol,
                cell_heights,
                cell_islands,
                nearest_distance_sq,
                nearest_tile);

            ++cell_heights;
            ++cell_islands;
        }
    }

    //--------------------------------------------------------------------------

    tile
    find_nearest_tile_in_island(
        const nav::map& map,
        const uint32_t  island,
        const nav::tile goal_tile)
    {
        assert(island);

        const auto goal_island = map.island(goal_tile);

        if (goal_island == island)
            return goal_tile;

        const point goal_point {
            float(goal_tile.row),
            float(goal_tile.col),
            map.height(goal_tile)
        };

        float nearest_distance_sq = INFINITY;

        nav::tile nearest_tile;

        auto* row_heights = map.heights();
        auto* row_islands = map.islands();

        const auto col_count  = map.col_count();
        const auto row_count  = map.row_count();
        const auto row_stride = map.row_stride();

        for (unsigned row = 0; row < row_count; ++row)
        {
            const float frow = row;

            find_nearest_tile_in_island_row(
                island,
                goal_point,
                row, frow,
                row_heights,
                row_islands,
                col_count,
                nearest_distance_sq,
                nearest_tile);

            row_heights += row_stride;
            row_islands += row_stride;
        }
        
        return nearest_tile;
    }

    //--------------------------------------------------------------------------

} // namespace nav