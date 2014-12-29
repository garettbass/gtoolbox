#include <zna/source/thirdParty/gtoolbox/nav/map.h>
#include <zna/source/thirdParty/gtoolbox/nav/point.h>
#include <zna/source/thirdParty/gtoolbox/nav/private/map.set_heights_and_obstacles.h>

namespace nav {

    using cell_heights = map::cell_heights;

    using cell_obstacles = map::cell_obstacles;

    using cell_neighbors = map::cell_neighbors;

    using cell_islands = map::cell_islands;

    using cell_costs = map::cell_costs;

    //--------------------------------------------------------------------------

    template <layer_index CELL_LAYER>
    static
    nav_inline
    void
    set_cell_height_and_obstruction(
        cell_heights*   heights,
        cell_obstacles* obstacles,
        const float     height_nw,
        const float     height_ne,
        const float     height_sw,
        const float     height_se,
        const float     max_height_delta)
    {
        const float h_min = math::min(height_nw, height_ne, height_sw, height_se);
        const float h_max = math::max(height_nw, height_ne, height_sw, height_se);

        const float h_delta = h_max - h_min;

        const float h = (h_min + h_max) / 2.f;

        heights[0][CELL_LAYER] = h;

        const bool obstructed = std::isinf(h) | (h_delta >= max_height_delta);

        if (obstructed) obstacles[0][CELL_LAYER].terrain_slope = true;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void
    set_heights_and_obstacles(
        const map&        map,
        const height_map& height_map,
        cell_heights*     map_heights,
        cell_obstacles*   map_obstacles)
    {
        const float max_height_delta = map.max_height_delta();

        // cell height is average of 4 corner heights:
        // nw   ne
        //    +
        // sw   se

        // cell is obstructed if height delta exceeds max_height_delta

        const unsigned map_col_count  = map.col_count();
        const unsigned map_row_count  = map.row_count();
        const unsigned map_row_stride = map.row_stride();

        auto* cell_ht_row = map_heights;
        auto* cell_ob_row = map_obstacles;

        const unsigned hm_row_stride = height_map.row_stride();

        const float* terrain_row = height_map.layer(TERRAIN);
        const float* bridges_row = height_map.layer(BRIDGES);

        for (unsigned row = 0; row < map_row_count; ++row)
        {
            auto* cell_ht_col = cell_ht_row;
            auto* cell_ob_col = cell_ob_row;

            const float* terrain_north = terrain_row;
            const float* terrain_south = terrain_row + hm_row_stride;
            const float* bridges_north = bridges_row;
            const float* bridges_south = bridges_row + hm_row_stride;

            // first point
            float terrain_nw = *terrain_north++;
            float terrain_sw = *terrain_south++;
            float bridges_nw = *bridges_north++;
            float bridges_sw = *bridges_south++;

            for (unsigned col = 0; col < map_col_count; ++col)
            {
                { // next point, terrain
                    float terrain_ne = *terrain_north;
                    float terrain_se = *terrain_south;

                    set_cell_height_and_obstruction<TERRAIN>(
                        cell_ht_col, cell_ob_col,
                        terrain_nw, terrain_ne,
                        terrain_sw, terrain_se,
                        max_height_delta);

                    terrain_nw = terrain_ne;
                    terrain_sw = terrain_se;
                }

                { // next point, bridges
                    float bridges_ne = *bridges_north;
                    float bridges_se = *bridges_south;

                    set_cell_height_and_obstruction<BRIDGES>(
                        cell_ht_col, cell_ob_col,
                        bridges_nw, bridges_ne,
                        bridges_sw, bridges_se,
                        max_height_delta);

                    bridges_nw = bridges_ne;
                    bridges_sw = bridges_se;
                }

                // next column
                ++cell_ht_col;
                ++cell_ob_col;
                ++terrain_north;
                ++terrain_south;
                ++bridges_north;
                ++bridges_south;
            }

            // next row
            cell_ht_row += map_row_stride;
            cell_ob_row += map_row_stride;

            terrain_row += hm_row_stride;
            bridges_row += hm_row_stride;
        }
    }

    //--------------------------------------------------------------------------

} // namespace nav