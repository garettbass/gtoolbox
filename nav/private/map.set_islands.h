#pragma once

namespace nav {

    void
    set_islands(
        const map&                       map,
        const map::cell_obstacles* const map_obstacles,
        const map::cell_neighbors* const map_neighbors,
              map::cell_islands*   const map_islands,
              uint32_t*            const scratch,
              size_t               const scratch_size);

} // namespace nav