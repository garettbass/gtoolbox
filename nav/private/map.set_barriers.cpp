#include <zna/source/thirdParty/gtoolbox/nav/map.h>
#include <zna/source/thirdParty/gtoolbox/nav/private/map.set_barriers.h>

namespace nav {

    using cell_heights = map::cell_heights;

    using cell_obstacles = map::cell_obstacles;

    using cell_neighbors = map::cell_neighbors;

    using cell_islands = map::cell_islands;

    using cell_costs = map::cell_costs;

    template <layer_index CELL_LAYER>
    static
    nav_inline
    void
    set_cell_barriers(
        const float           min_height,
        const float           max_height,
        const int             delta,
        const cell_heights*   heights,
              cell_obstacles* obstacles)
    {
        const float height = heights[0][CELL_LAYER];

        if (height < min_height | height > max_height)
            return;

        auto& obstacle = obstacles[0][CELL_LAYER];

        assert(
            delta > 0
            ? (obstacle.barrier_count <= math::max<int8_t>() - delta)
            : delta < 0
            ? (obstacle.barrier_count >= delta)
            : delta == 0);

        obstacle.barrier_count += delta;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    static
    nav_inline
    void
    set_row_barriers(
        const unsigned        min_col,
        const unsigned        end_col,
        const float           min_height,
        const float           max_height,
        const int             delta,
        const cell_heights*   heights,
              cell_obstacles* obstacles)
    {
        for (unsigned col = min_col; col < end_col; ++col)
        {
            set_cell_barriers<TERRAIN>(
                min_height,
                max_height,
                delta,
                heights,
                obstacles);
            set_cell_barriers<BRIDGES>(
                min_height,
                max_height,
                delta,
                heights,
                obstacles);

            ++heights;
            ++obstacles;
        }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void
    set_barriers(
        const map&                 map,
        const nav::point           min_point,
        const nav::point           max_point,
        const int                  delta,
        const map::cell_heights*   heights,
              map::cell_obstacles* obstacles)
    {
        assert(min_point.row <= max_point.row);
        assert(min_point.col <= max_point.col);
        assert(delta < math::max<int8_t>());

        const auto min_cell = map.cell(min_point);
        const auto end_cell = map.cell(max_point);

        const auto min_height = min_point.height;
        const auto max_height = max_point.height;

        const auto row_stride = map.row_stride();

        const auto i = map.cell_index(min_cell);

        heights   = &heights[i];
        obstacles = &obstacles[i];

        for (unsigned row = min_cell.row; row < end_cell.row; ++row)
        {
            set_row_barriers(
                min_cell.col,
                end_cell.col,
                min_height,
                max_height,
                delta,
                heights,
                obstacles);

            heights   += row_stride;
            obstacles += row_stride;
        }
    }

    //--------------------------------------------------------------------------

} // namespace nav