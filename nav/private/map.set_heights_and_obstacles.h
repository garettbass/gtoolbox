#pragma once

namespace nav {

    void
    set_heights_and_obstacles(
        const map&           map,
        const height_map&    height_map,
        map::cell_heights*   map_heights,
        map::cell_obstacles* map_obstacles);

} // namespace nav