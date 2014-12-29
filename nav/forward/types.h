#pragma once
#include <cstdint>

namespace nav {

    //--------------------------------------------------------------------------

    enum direction_index : uint8_t { NORTH, SOUTH, EAST, WEST };

    enum { DIRECTION_COUNT = 4 };

    //--------------------------------------------------------------------------

    enum layer_index : uint8_t { TERRAIN, BRIDGES };

    enum { LAYER_COUNT = 2 };

    //--------------------------------------------------------------------------

    struct cell;

    struct cell_info;

    struct neighbor_link;

    struct neighbor_set;

    struct point;

    struct tile;

    struct tile_info;

    //--------------------------------------------------------------------------

    class height_map;

    class map;

    class navigator;

    class path;

    class unreachable final {};

    //--------------------------------------------------------------------------

} // namespace nav