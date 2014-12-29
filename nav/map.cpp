#include <algorithm>
#include <memory>
#include <zna/source/thirdParty/gtoolbox/math/constants.h>
#include <zna/source/thirdParty/gtoolbox/nav/map.h>
#include <zna/source/thirdParty/gtoolbox/nav/private/map.set_heights_and_obstacles.h>
#include <zna/source/thirdParty/gtoolbox/nav/private/map.set_islands.h>
#include <zna/source/thirdParty/gtoolbox/nav/private/map.set_neighbors.h>
#include <zna/source/thirdParty/gtoolbox/nav/private/map.set_barriers.h>
#include <zna/source/thirdParty/gtoolbox/nav/private/map.set_costs.h>
#include <zna/source/thirdParty/gtoolbox/xtd/utility/scope_timer.h>

namespace nav {

    /*
        height_map initialization:
        
            render terrain into layer 0
            render bridges into layer 1
        
        map initialization:

            height -> obstacles -> neighbors -> islands
        
        path finding:
        
            1. find start island and goal island
            
            2. if (start_island != goal_island)
                  goal = point in start_island nearest to goal
            
            3. use A* to path from start to goal

        TODO: obstacles are rasterized into each layer, based on height
    */

    //--------------------------------------------------------------------------

    map::map(unsigned rows, unsigned cols)
    : map_index(rows, cols)
    {
        assert(rows <= nav::cell(unreachable()).row);
        assert(cols <= nav::cell(unreachable()).col);

        const auto cell_size    =
            sizeof(*_heights)   +
            sizeof(*_islands)   +
            sizeof(*_obstacles) +
            sizeof(*_neighbors) +
            sizeof(*_costs)     +
            sizeof(*_scratch);

        const auto cell_count = base::cell_count();

        const auto data_size = cell_size * cell_count;

//        printf("map(%u, %u): %zu bytes\n", rows, cols, data_size);

        auto* head = new uint8_t[data_size];
        auto* tail = head + data_size;

        std::fill(head, tail, 0);

        _heights   = decltype(_heights)   (head);
        _islands   = decltype(_islands)   (_heights   + cell_count);
        _obstacles = decltype(_obstacles) (_islands   + cell_count);
        _neighbors = decltype(_neighbors) (_obstacles + cell_count);
        _costs     = decltype(_costs)     (_neighbors + cell_count);
        _scratch   = decltype(_scratch)   (_costs     + cell_count);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    map::map(const height_map& height_map, const unsigned max_slope_degrees)
    : map(
        height_map.row_count() - 1,
        height_map.col_count() - 1)
    {
        assert(row_count() > 1);
        assert(col_count() > 1);
        assert(max_slope_degrees < 60);

        const float max_slope_radians
            = float(max_slope_degrees)
            * math::RADIANS_PER_DEGREE;

        _max_height_delta = std::tan(max_slope_radians);

        if (not row_count() or not col_count())
            return;

        set_heights_and_obstacles(
            *this,
            height_map,
            _heights,
            _obstacles);

        set_neighbors(
            *this,
            _heights,
            _obstacles,
            _neighbors);

        set_islands(
            *this,
            _obstacles,
            _neighbors,
            _islands,
            _scratch,
            cell_count());
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    map::map(map&& m)
    : map((const map&)m) // copy 'm'
    {
        new(&m) map(); // reset 'm'
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    map& map::operator = (map&& g)
    {
        this->~map(); // release resources

        return *new(this) map(std::move(g)); // move construct
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    map::~map()
    {
        delete[] _heights;
    }

    //--------------------------------------------------------------------------

    void
    map::modify_barriers(nav::tile tile, int8_t delta)
    {
        _has_unresolved_barriers = true;

        auto& obstacle = _obstacles[cell_index(tile)][tile.layer];

        assert(
            delta > 0
            ? (obstacle.barrier_count <= math::max<int8_t>() - delta)
            : delta < 0
            ? (obstacle.barrier_count >= delta)
            : delta == 0);

        obstacle.barrier_count += delta;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void
    map::modify_barriers(nav::point min, nav::point max, int8_t delta)
    {
        _has_unresolved_barriers = true;

        set_barriers(
            *this,
            min,
            max,
            delta,
            _heights,
            _obstacles);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void
    map::resolve_barriers()
    {
        if (_has_unresolved_barriers)
        {
            set_neighbors(
                *this,
                _heights,
                _obstacles,
                _neighbors);

            set_islands(
                *this,
                _obstacles,
                _neighbors,
                _islands,
                _scratch,
                cell_count());

            _has_unresolved_barriers = false;

            _state += 1;
        }
    }

    //--------------------------------------------------------------------------

    void
    map::modify_costs(
        nav::point center,
        unsigned   radius,
        unsigned   height,
        int8_t     delta)
    {
        set_costs(
            *this,
            center,
            radius,
            height,
            delta,
            _heights,
            _costs);

        _state += 1;
    }

    //--------------------------------------------------------------------------

} // namespace nav