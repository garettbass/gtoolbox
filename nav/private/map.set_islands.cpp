#include <gtoolbox/nav/map.h>
#include <gtoolbox/nav/private/map.set_islands.h>

namespace nav {

    using cell_heights = map::cell_heights;

    using cell_obstacles = map::cell_obstacles;

    using cell_neighbors = map::cell_neighbors;

    using cell_islands = map::cell_islands;

    using cell_costs = map::cell_costs;

    //--------------------------------------------------------------------------

    template <layer_index CELL_LAYER, direction_index CELL_NEIGHBOR>
    static
    nav_inline
    uint32_t
    get_island_from_neighbor(
        const map&                  map,
        const cell_neighbors* const cell_neighbors,
        const cell_islands*   const cell_islands,
        const uint32_t*       const merge_islands)
    {
        const auto& cell_neighbor = cell_neighbors[0][CELL_LAYER];

        if (not cell_neighbor.exists(CELL_NEIGHBOR))
            return math::max<uint32_t>();

        const auto neighbor_offset = map.neighbor_offset(CELL_NEIGHBOR);

        const auto neighbor_layer = cell_neighbor.layer(CELL_NEIGHBOR);

        return merge_islands[cell_islands[neighbor_offset][neighbor_layer]];
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    template <layer_index CELL_LAYER>
    static
    nav_inline
    void
    set_first_island(
        const cell_obstacles* const obstacles,
              cell_islands*   const islands,
              uint32_t*       const merge_islands,
              uint32_t&             last_island)
    {
        auto& island = islands[0][CELL_LAYER];

        const bool obstructed = obstacles[0][CELL_LAYER].obstructed();

        if (obstructed)
        {
            island = 0;

            return;
        }

        island = ++last_island;

        assert(island);

        merge_islands[island] = island;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    template <
        layer_index        CELL_LAYER,
        direction_index    NEIGHBOR,
        direction_index... NEIGHBORS>
    static
    nav_inline
    void
    set_island_from_neighbors(
        const map&                  map,
        const cell_obstacles* const obstacles,
        const cell_neighbors* const neighbors,
              cell_islands*   const islands,
              uint32_t*       const merge_islands,
              uint32_t&             last_island)
    {
        auto& island = islands[0][CELL_LAYER];

        const bool obstructed = obstacles[0][CELL_LAYER].obstructed();

        if (obstructed)
        {
            island = 0;

            return;
        }

        auto& tile_neighbors = neighbors[0][CELL_LAYER];

        const bool has_neighbors = tile_neighbors.any<NEIGHBOR, NEIGHBORS...>();

        if (not has_neighbors)
        {
            island = ++last_island;

            assert(island);

            merge_islands[island] = island;

            return;
        }

        enum { NEIGHBOR_COUNT = 1 + sizeof...(NEIGHBORS) };

        const uint32_t neighbor_islands[NEIGHBOR_COUNT] = {
            get_island_from_neighbor<CELL_LAYER, NEIGHBOR>(
                map,
                neighbors,
                islands,
                merge_islands),
            get_island_from_neighbor<CELL_LAYER, NEIGHBORS>(
                map,
                neighbors,
                islands,
                merge_islands)...
        };

        // inherit minimum island from neighbors
        island = math::min(neighbor_islands);

        // propagate minimum island to neighbors
        for (auto neighbor_island : neighbor_islands)
        {
            const bool is_valid_island = ~neighbor_island;

            if (is_valid_island & neighbor_island != island)
            {
                assert(neighbor_island);

                merge_islands[neighbor_island] =
                    math::min(merge_islands[neighbor_island], island);
            }
        }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    static
    nav_inline
    void
    set_first_row_islands(
        const map&                  map,
        const cell_obstacles* const row_obstacles,
        const cell_neighbors* const row_neighbors,
              cell_islands*   const row_islands,
              uint32_t*       const merge_islands,
              uint32_t&             last_island)
    {
        auto* cell_obstacles = row_obstacles;
        auto* cell_neighbors = row_neighbors;
        auto* cell_islands   = row_islands;

        // explicitly set the initial island in each layer
        set_first_island<TERRAIN>(
            cell_obstacles,
            cell_islands,
            merge_islands,
            last_island);
        set_first_island<BRIDGES>(
            cell_obstacles,
            cell_islands,
            merge_islands,
            last_island);

        ++cell_obstacles;
        ++cell_neighbors;
        ++cell_islands;

        // inherit subsequent islands from west neighbor
        const unsigned col_count = map.col_count();
        for (unsigned col = 1; col < col_count; ++col)
        {
            set_island_from_neighbors<TERRAIN, WEST>(
                map,
                cell_obstacles,
                cell_neighbors,
                cell_islands,
                merge_islands,
                last_island);
            set_island_from_neighbors<BRIDGES, WEST>(
                map,
                cell_obstacles,
                cell_neighbors,
                cell_islands,
                merge_islands,
                last_island);

            ++cell_obstacles;
            ++cell_neighbors;
            ++cell_islands;
        }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    static
    nav_inline
    void
    set_row_islands(
        const map&                  map,
        const cell_obstacles* const row_obstacles,
        const cell_neighbors* const row_neighbors,
              cell_islands*   const row_islands,
              uint32_t*             merge_islands,
              uint32_t&             last_island)
    {
        auto* cell_obstacles = row_obstacles;
        auto* cell_neighbors = row_neighbors;
        auto* cell_islands   = row_islands;

        // inherit first island from north neighbor
        set_island_from_neighbors<TERRAIN, NORTH>(
            map,
            cell_obstacles,
            cell_neighbors,
            cell_islands,
            merge_islands,
            last_island);
        set_island_from_neighbors<BRIDGES, NORTH>(
            map,
            cell_obstacles,
            cell_neighbors,
            cell_islands,
            merge_islands,
            last_island);

        ++cell_obstacles;
        ++cell_neighbors;
        ++cell_islands;

        const unsigned col_count = map.col_count();

        // inherit subsequent islands from east or north neighbor
        for (unsigned col = 1; col < col_count; ++col)
        {
            set_island_from_neighbors<TERRAIN, NORTH, WEST>(
                map,
                cell_obstacles,
                cell_neighbors,
                cell_islands,
                merge_islands,
                last_island);
            set_island_from_neighbors<BRIDGES, NORTH, WEST>(
                map,
                cell_obstacles,
                cell_neighbors,
                cell_islands,
                merge_islands,
                last_island);

            ++cell_obstacles;
            ++cell_neighbors;
            ++cell_islands;
        }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void
    set_islands(
        const map&                  map,
        const cell_obstacles* const map_obstacles,
        const cell_neighbors* const map_neighbors,
              cell_islands*   const map_islands,
              uint32_t*       const scratch,
              size_t          const scratch_size)
    {
        assert(scratch_size == map.cell_count());

        // merge_islands[0] is not used
        uint32_t* merge_islands = scratch - 1;

        uint32_t last_island = 0;

        auto* row_obstacles = map_obstacles;
        auto* row_neighbors = map_neighbors;
        auto* row_islands   = map_islands;

        set_first_row_islands(
            map,
            row_obstacles,
            row_neighbors,
            row_islands,
            merge_islands,
            last_island);

        const unsigned row_stride = map.row_stride();

        const unsigned row_count = map.row_count();

        // remaining rows inherit islands from northern neighbor_link
        for (unsigned row = 1; row < row_count; ++row)
        {
            row_obstacles += row_stride;
            row_neighbors += row_stride;
            row_islands   += row_stride;

            set_row_islands(
                map,
                row_obstacles,
                row_neighbors,
                row_islands,
                merge_islands,
                last_island);
        }

        //return;

        // merge redundant islands...

        const unsigned col_count = map.col_count();

        // check for merge_islands buffer overflow
        assert(last_island <= scratch_size);

        for (uint32_t i = 1; i <= last_island; ++i)
            merge_islands[i] = merge_islands[merge_islands[i]];

        const int last_col = col_count - 1;

        // currently pointing to first cell in last row
        // move to last cell in last row
        row_islands += last_col;

        const int last_row = row_count - 1;

        for (int row = last_row; row >= 0; --row)
        {
            auto* cell_islands = row_islands;

            for (int col = last_col; col >= 0; --col)
            {
                if (auto& terrain_island = cell_islands[0][TERRAIN])
                    terrain_island = merge_islands[terrain_island];

                if (auto& bridges_island = cell_islands[0][BRIDGES])
                    bridges_island = merge_islands[bridges_island];

                --cell_islands;
            }

            row_islands -= row_stride;
        }
    }

    //--------------------------------------------------------------------------

} // namespace nav