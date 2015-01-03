#include <gtoolbox/nav/map.h>
#include <gtoolbox/nav/private/map.set_neighbors.h>

namespace nav {

    using cell_heights = map::cell_heights;

    using cell_obstacles = map::cell_obstacles;

    using cell_neighbors = map::cell_neighbors;

    using cell_islands = map::cell_islands;

    using cell_costs = map::cell_costs;

    //--------------------------------------------------------------------------

    template <layer_index NEIGHBOR_LAYER>
    static
    nav_inline
    float
    get_neighbor_height(
        const int                   neighbor_offset,
        const cell_heights*   const heights,
        const cell_obstacles* const obstacles)
    {
        return
            obstacles[neighbor_offset][NEIGHBOR_LAYER].obstructed()
            ? -INFINITY
            : heights[neighbor_offset][NEIGHBOR_LAYER];
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    template <layer_index TILE_LAYER, direction_index NEIGHBOR>
    static
    nav_inline
    void
    set_neighbor(
        const int                   neighbor_offset,
        const float                 max_height_delta,
        const cell_heights*   const heights,
        const cell_obstacles* const obstacles,
              cell_neighbors* const neighbors)
    {
        auto& neighbor = neighbors[0][TILE_LAYER];

        const bool obstructed = obstacles[0][TILE_LAYER].obstructed();

        if (obstructed)
        {
            neighbor.exists(NEIGHBOR, false);

            return;
        }

        const float tile_height = heights[0][TILE_LAYER];

        const bool is_tile_unreachable = std::isinf(tile_height);

        if (is_tile_unreachable)
        {
            neighbor.exists(NEIGHBOR, false);

            return;
        }

        const float terrain_neighbor_height =
            get_neighbor_height<TERRAIN>(
                neighbor_offset,
                heights,
                obstacles);

        const bool is_tile_under_bridge =
            (TILE_LAYER == TERRAIN)
            and
            (heights[0][BRIDGES] > tile_height);

        if (not is_tile_under_bridge)
        {
            const float bridge_neighbor_height =
                get_neighbor_height<BRIDGES>(
                    neighbor_offset,
                    heights,
                    obstacles);

            const bool neighbors_unreachable =
                std::isinf(terrain_neighbor_height)
                and
                std::isinf(bridge_neighbor_height);

            if (neighbors_unreachable)
            {
                neighbor.exists(NEIGHBOR, false);

                return;
            }

            const float bridge_neighbor_height_delta =
                std::abs(bridge_neighbor_height - tile_height);

            const bool is_bridge_neighbor_above_terrain_neighbor =
                bridge_neighbor_height > terrain_neighbor_height;

            const bool is_bridge_neighbor_near_enough =
                bridge_neighbor_height_delta < max_height_delta;

            if (is_bridge_neighbor_above_terrain_neighbor &
                is_bridge_neighbor_near_enough)
            {
                neighbor.exists(NEIGHBOR, true);

                neighbor.layer(NEIGHBOR, BRIDGES);

                return;
            }
        }

        const float terrain_neighbor_height_delta =
            std::abs(terrain_neighbor_height - tile_height);

        const bool is_terrain_neighbor_near_enough =
            terrain_neighbor_height_delta < max_height_delta;

        if (is_terrain_neighbor_near_enough)
        {
            neighbor.exists(NEIGHBOR, true);

            neighbor.layer(NEIGHBOR, TERRAIN);

            return;
        }

        neighbor.exists(NEIGHBOR, false);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    template <direction_index NEIGHBOR>
    static
    nav_inline
    void
    set_neighbor(
        const map&                  map,
        const cell_heights*   const heights,
        const cell_obstacles* const obstacles,
              cell_neighbors* const neighbors)
    {
        const auto neighbor_offset = map.neighbor_offset(NEIGHBOR);

        const auto max_height_delta = map.max_height_delta();

        set_neighbor<TERRAIN, NEIGHBOR>(
            neighbor_offset,
            max_height_delta,
            heights,
            obstacles,
            neighbors);

        set_neighbor<BRIDGES, NEIGHBOR>(
            neighbor_offset,
            max_height_delta,
            heights,
            obstacles,
            neighbors);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    template <direction_index NORTH_OR_SOUTH>
    static
    nav_inline
    void
    set_row_neighbors_from(
        const map&            map,
        const cell_heights*   heights,
        const cell_obstacles* obstacles,
              cell_neighbors* neighbors)
    {
        env_static_assert(
            NORTH_OR_SOUTH == NORTH
            or
            NORTH_OR_SOUTH == SOUTH);

        constexpr auto FROM = NORTH_OR_SOUTH;

        constexpr auto OPPOSITE = (FROM == NORTH) ? SOUTH : NORTH;

        (void)OPPOSITE;

        // west column only has neighbors to the east and south
        set_neighbor<EAST>(map, heights, obstacles, neighbors);
        set_neighbor<FROM>(map, heights, obstacles, neighbors);
        assert(neighbors[0][TERRAIN].exists(WEST) == false);
        assert(neighbors[0][BRIDGES].exists(WEST) == false);
        assert(neighbors[0][TERRAIN].exists(OPPOSITE) == false);
        assert(neighbors[0][BRIDGES].exists(OPPOSITE) == false);

        ++heights;
        ++obstacles;
        ++neighbors;

        const unsigned col_count = map.col_count();
        const unsigned last_col = col_count - 1;

        // internal columns have neighbors to the east, west, and south
        for (unsigned col = 1; col < last_col; ++col)
        {
            set_neighbor<EAST>(map, heights, obstacles, neighbors);
            set_neighbor<WEST>(map, heights, obstacles, neighbors);
            set_neighbor<FROM>(map, heights, obstacles, neighbors);

            ++heights;
            ++obstacles;
            ++neighbors;
        }

        // east column only has neighbors to the west and south
        set_neighbor<WEST>(map, heights, obstacles, neighbors);
        set_neighbor<FROM>(map, heights, obstacles, neighbors);
        assert(neighbors[0][TERRAIN].exists(EAST) == false);
        assert(neighbors[0][BRIDGES].exists(EAST) == false);
        assert(neighbors[0][TERRAIN].exists(OPPOSITE) == false);
        assert(neighbors[0][BRIDGES].exists(OPPOSITE) == false);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    static
    nav_inline
    void
    set_row_neighbors(
        const map&            map,
        const cell_heights*   heights,
        const cell_obstacles* obstacles,
              cell_neighbors* neighbors)
    {
        // west cell only has neighbors to the east, north, and south
        set_neighbor<EAST>(map, heights, obstacles, neighbors);
        set_neighbor<NORTH>(map, heights, obstacles, neighbors);
        set_neighbor<SOUTH>(map, heights, obstacles, neighbors);
        assert(neighbors[0][TERRAIN].exists(WEST) == false);
        assert(neighbors[0][BRIDGES].exists(WEST) == false);

        ++heights;
        ++obstacles;
        ++neighbors;

        const unsigned col_count = map.col_count();
        const unsigned last_col = col_count - 1;

        // internal cells have neighbors in all directions
        for (unsigned col = 1; col < last_col; ++col)
        {
            set_neighbor<EAST>(map, heights, obstacles, neighbors);
            set_neighbor<WEST>(map, heights, obstacles, neighbors);
            set_neighbor<NORTH>(map, heights, obstacles, neighbors);
            set_neighbor<SOUTH>(map, heights, obstacles, neighbors);

            ++heights;
            ++obstacles;
            ++neighbors;
        }

        // east cell only has neighbors to the west, north, and south
        set_neighbor<WEST>(map, heights, obstacles, neighbors);
        set_neighbor<NORTH>(map, heights, obstacles, neighbors);
        set_neighbor<SOUTH>(map, heights, obstacles, neighbors);
        assert(neighbors[0][TERRAIN].exists(EAST) == false);
        assert(neighbors[0][BRIDGES].exists(EAST) == false);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void
    set_neighbors(
        const map&            map,
        const cell_heights*   heights,
        const cell_obstacles* obstacles,
              cell_neighbors* neighbors)
    {
        // north row only has neighbors to the south
        set_row_neighbors_from<SOUTH>(map, heights, obstacles, neighbors);

        const unsigned row_stride = map.row_stride();

        heights   += row_stride;
        obstacles += row_stride;
        neighbors += row_stride;

        // internal rows have neighbors to the north and south
        const unsigned row_count = map.row_count();
        const unsigned last_row  = row_count - 1;
        for (unsigned row = 1; row < last_row; ++row)
        {
            set_row_neighbors(map, heights, obstacles, neighbors);

            heights   += row_stride;
            obstacles += row_stride;
            neighbors += row_stride;
        }

        // south row only has neighbors to the north
        set_row_neighbors_from<NORTH>(map, heights, obstacles, neighbors);
    }

    //--------------------------------------------------------------------------

} // namespace nav