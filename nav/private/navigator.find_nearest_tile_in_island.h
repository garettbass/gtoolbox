#pragma once

namespace nav {

    tile
    find_nearest_tile_in_island(
        const nav::map& map,
        const uint32_t  island,
        const nav::tile goal_tile);

} // namespace nav