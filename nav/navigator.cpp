#include <zna/source/thirdParty/gtoolbox/nav/navigator.h>
#include <zna/source/thirdParty/gtoolbox/nav/private/navigator.find_nearest_tile_in_island.h>
//#include <zna/source/thirdParty/gtoolbox/nav/private/navigator.push_heap.h>
//#include <zna/source/thirdParty/gtoolbox/nav/private/navigator.pop_heap.h>
#include <zna/source/thirdParty/gtoolbox/nav/neighbor_set.h>

namespace nav {

    //--------------------------------------------------------------------------

    navigator::navigator(const nav::map& map)
    : _map(&map)
    {
        assert(map.row_count() <= cell(unreachable()).row);
        assert(map.col_count() <= cell(unreachable()).col);

        const auto cell_size    =
            sizeof(*_cell_map)  +
            sizeof(*_open_head);

        const auto cell_count = _map->cell_count();

        const auto data_size = cell_size * cell_count;

        auto* data = new uint8_t[data_size];

        _cell_map  = decltype(_cell_map)  (data);
        _open_head = decltype(_open_head) (_cell_map + cell_count);

//        printf("navigator(%u, %u): %zu bytes\n", map.row_count(), map.col_count(), data_size);

        clear();
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    navigator::navigator(navigator&& n)
    : navigator((const navigator&)n) // copy 'n'
    {
        new(&n) navigator(); // reset 'n'
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    navigator&
    navigator::operator = (navigator&& n)
    {
        this->~navigator(); // release resources

        return *new(this) navigator(std::move(n)); // move construct
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    navigator::~navigator()
    {
        delete[] _cell_map;
    }

    //--------------------------------------------------------------------------
    
    void
    navigator::clear()
    {
        // this fill clears the visited bit
        const auto cell_count = _map->cell_count();
        {
            tile* head = &_cell_map[0][0];
            tile* tail = head + (cell_count * LAYER_COUNT);
            std::fill(head, tail, unreachable());
        }

        _open_tail = _open_head;

        _start = unreachable();

        _stop = unreachable();
    }

    //--------------------------------------------------------------------------

    nav_inline
    bool
    navigator::done() const
    { return _open_tail == _open_head; }

    //--------------------------------------------------------------------------
    // open_tile comparison operator for std::push_heap/pop_heap()

    using open_tile = navigator::open_tile;

    nav_inline
    static
    bool operator < (const open_tile& a, const open_tile& b)
    {
        return a.f > b.f; // comparison inverted to produce min-heap
    }

    //--------------------------------------------------------------------------

    nav_inline
    void
    navigator::push(
        nav::tile   next,
        nav::tile   curr,
        const float curr_g,
        const float curr_f)
    {
        assert(not visited(curr));

        const auto i = _map->cell_index(curr);

        auto& tile_map_ref = _cell_map[i][curr.layer];

        tile_map_ref = next;

        tile_map_ref.visited = true;

        *_open_tail++ = { curr, curr_g, curr_f };

        std::push_heap(_open_head, _open_tail);
    }

    //--------------------------------------------------------------------------

    nav_inline
    navigator::open_tile
    navigator::pop()
    {
        assert(not done());

        std::pop_heap(_open_head, _open_tail);

        return *--_open_tail;
    }

    //--------------------------------------------------------------------------

    // h() - estimated cost curr to goal
    nav_inline
    float
    h(float curr_cost, tile curr, tile goal)
    {
        float scale = 1.f + curr_cost;

        return scale * distance::euclidean(curr, goal);
    }

    // f() - estimated cost start to goal
    nav_inline
    float
    f(float curr_g, float curr_cost, tile curr, tile goal)
    {
        return curr_g + h(curr_cost, curr, goal);
    }

    // g() - accumulated cost start to curr
    nav_inline
    float
    g(float prev_g, tile prev, tile curr)
    {
        return prev_g + distance::euclidean(prev, curr);
    }

    //--------------------------------------------------------------------------

    nav_inline
    bool
    navigator::a_star(const tile start, const tile stop)
    {
        assert(_map);

        assert(_map->island(start) == _map->island(stop));

        const auto& map = *_map;

        neighbor_set neighbors;

        push(unreachable(), start, 0, f(0, 0, start, stop));

        while (not done())
        {
            const auto curr = pop();

            if (curr == stop)
                return true;

            for (auto next : neighbors(map, curr))
            {
                if (visited(next))
                    continue;

                auto next_cost = map.cost(next);

                auto next_g = g(curr.g, curr, next);

                auto next_f = f(next_g, next_cost, next, stop);

                push(curr, next, next_g, next_f);
            }
        }

        return false;
    }

    //--------------------------------------------------------------------------

    nav_inline
    void
    navigator::reverse()
    {
        assert(next(_start) == unreachable());

        tile prev = unreachable();

        tile curr = _stop;

        while (curr != _start)
        {
            tile temp = curr;

            curr = next(curr);

            set_next(temp, prev);

            prev = temp;
        }

        set_next(_start, prev);

        assert(next(_stop) == unreachable());
    }

    //--------------------------------------------------------------------------

    bool
    navigator::navigate(const tile start, const tile goal)
    {
        assert(_map);

        assert(not _map->has_unresolved_barriers());

        clear();

        _start = start;

        _stop = start;

        const auto start_island = _map->island(_start);

        if (not start_island)
        {
            // 'start' is inside an obstacle
            // TODO: consider options for moving start out of the obstacle
            return false;
        }

        _stop = find_nearest_tile_in_island(*_map, start_island, goal);

        if (_map->island(_start) != _map->island(_stop))
        {
            // failed to find a tile near the start location
            // likely due to -INF boundaries in terrain height
            // this is almost certainly a bug
            return false;
        }

        // find forward path
        if (not a_star(_start, _stop))
        {
            clear();

            return false;
        }

        // reverse path connectivity for forward iteration
        reverse();

        return true;
    }

    //--------------------------------------------------------------------------

} // namespace nav