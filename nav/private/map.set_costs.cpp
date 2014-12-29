#include <zna/source/thirdParty/gtoolbox/math/functions.h>
#include <zna/source/thirdParty/gtoolbox/nav/map.h>
#include <zna/source/thirdParty/gtoolbox/nav/private/map.set_costs.h>

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
    set_cell_costs(
        const float         min_height,
        const float         max_height,
        const int           delta,
        const cell_heights* heights,
              cell_costs*   costs)
    {
        const float height = heights[0][CELL_LAYER];

        if (height < min_height | height > max_height)
            return;

        auto& cost = costs[0][CELL_LAYER];

        assert(
            delta > 0
            ? (cost <= math::max<int8_t>() - delta)
            : delta < 0
            ? (cost >= delta)
            : delta == 0);

        cost += delta;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    static
    nav_inline
    void
    set_row_costs(
        const float         row_offset_sq,
        const unsigned      min_col,
        const unsigned      max_col,
        const float         cost_center_col,
        const unsigned      cost_radius_sq,
        const float         min_height,
        const float         max_height,
        const int           delta,
        const cell_heights* heights,
              cell_costs*   costs)
    {
        const float col_center = min_col + 0.5f;

        float col_offset = col_center - cost_center_col;

        for (unsigned col = min_col; col <= max_col; ++col)
        {
            const float col_offset_sq = col_offset * col_offset;

            const float cell_offset_sq = row_offset_sq + col_offset_sq;

            const bool inside_cost_radius = cell_offset_sq <= cost_radius_sq;

            if (inside_cost_radius)
            {
                set_cell_costs<TERRAIN>(
                    min_height,
                    max_height,
                    delta,
                    heights,
                    costs);
                set_cell_costs<BRIDGES>(
                    min_height,
                    max_height,
                    delta,
                    heights,
                    costs);
            }

            ++col_offset;

            ++heights;
            ++costs;
        }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void
    set_costs(
        const map&          map,
        const nav::point    cost_center,
        const unsigned      cost_radius,
        const unsigned      cost_height,
        const int           cost_delta,
        const cell_heights* heights,
              cell_costs*   costs)
    {
        assert(cost_delta < math::max<int8_t>());

        const float last_row = map.row_count() - 1;
        const float last_col = map.col_count() - 1;

        const nav::point min_point {
            math::clamp(cost_center.row - cost_radius, 0.f, last_row),
            math::clamp(cost_center.col - cost_radius, 0.f, last_col),
            cost_center.height
        };

        const nav::point max_point {
            math::clamp(cost_center.row + cost_radius, 0.f, last_row),
            math::clamp(cost_center.col + cost_radius, 0.f, last_col),
            cost_center.height + cost_height,
        };

        const auto min_cell = map.cell(min_point);
        const auto max_cell = map.cell(max_point);

        assert(min_cell.row <= max_cell.row);
        assert(min_cell.col <= max_cell.col);

        const auto min_height = min_point.height;
        const auto max_height = max_point.height;

        assert(min_height < max_height);

        const auto row_stride = map.row_stride();

        const float row_center = float(min_cell.row) + 0.5f;

        float row_offset = row_center - cost_center.row;

        const float cost_radius_sq = cost_radius * cost_radius;

        const auto i = map.cell_index(min_cell);

        heights = &heights[i];
        costs   = &costs[i];

        for (unsigned row = min_cell.row; row <= max_cell.row; ++row)
        {
            const float row_offset_sq = row_offset * row_offset;

            set_row_costs(
                row_offset_sq,
                min_cell.col,
                max_cell.col,
                cost_center.col,
                cost_radius_sq,
                min_height,
                max_height,
                cost_delta,
                heights,
                costs);

            row_offset += 1.f;

            heights += row_stride;
            costs   += row_stride;
        }
    }

    //--------------------------------------------------------------------------

} // namespace nav