#pragma once

namespace nav {

    void
    set_neighbors(
        const map&                 map,
        const map::cell_heights*   heights,
        const map::cell_obstacles* obstacles,
              map::cell_neighbors* neighbors);

} // namespace nav