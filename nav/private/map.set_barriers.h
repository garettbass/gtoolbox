#pragma once

namespace nav {

    void
    set_barriers(
        const map&                 map,
        const nav::point           min,
        const nav::point           max,
        const int                  delta,
        const map::cell_heights*   heights,
              map::cell_obstacles* obstacles);

} // namespace nav