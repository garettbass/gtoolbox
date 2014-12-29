#pragma once

namespace nav {

    void
    set_costs(
        const map&               map,
        const nav::point         center,
        const unsigned           radius,
        const unsigned           height,
        const int                delta,
        const map::cell_heights* heights,
              map::cell_costs*   costs);

} // namespace nav