#include <cassert>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <gtoolbox/env/os.h>
#include <gtoolbox/nav/navigator.h>
#include <gtoolbox/nav/tests.h>
#include <gtoolbox/test/test.h>
#include <gtoolbox/xtd/utility/scope_timer.h>

namespace nav {

    static
    void
    print_upper_outline(std::ostream& s, const map_index& m)
    {
        const unsigned cols = m.col_count();
        s << "\u259B";
        for (int col = 0; col < cols; ++col) s << "\u2580";
        s << "\u259C";
    }

    static
    void
    print_left_outline(std::ostream& s)
    {
        s << "\u258C";
    }

    static
    void
    print_right_outline(std::ostream& s)
    {
        s << "\u2590";
    }

    static
    void
    print_lower_outline(std::ostream& s, const map_index& m)
    {
        const unsigned cols = m.col_count();
        s << "\u2599";
        for (int col = 0; col < cols; ++col) s << "\u2584";
        s << "\u259F";
    }

    static
    void
    print_height(std::ostream& s, const float height)
    {
        if (std::isinf(height))
        {
            s << "\u2588";

            return;
        }

        enum : int { DIGITS = 10, LETTERS = 26 };

        const int i = int(std::round(height)) % (DIGITS + LETTERS + LETTERS);

        char c =
            (i < DIGITS)
            ? '0' + i
            : (i < DIGITS + LETTERS)
              ? 'a' + (i - DIGITS)
              : 'A' + (i - DIGITS - LETTERS);

        s << c;
    }

    void
    print_heights(const height_map& h, std::FILE* out)
    {
        std::stringstream s;

        const unsigned rows = h.row_count();
        const unsigned cols = h.col_count();

        print_upper_outline(s, h); print_upper_outline(s, h); s << '\n';

        for (unsigned row = 0; row < rows; ++row)
        {
            print_left_outline(s);

            for (unsigned col = 0; col < cols; ++col)
            {
                print_height(s, h.height(TERRAIN, row, col));
            }

            print_right_outline(s);
            print_left_outline(s);

            for (unsigned col = 0; col < cols; ++col)
            {
                print_height(s, h.height(BRIDGES, row, col));
            }

            print_right_outline(s); s << '\n';
        }

        print_lower_outline(s, h); print_lower_outline(s, h);

        fputs(s.str().c_str(), out);
    }

    static
    void
    print_island(std::ostream& s, const map& m, const nav::tile t)
    {
        auto island = m.island(t);

        if (m.obstructed(t))
        {
            assert(island == 0);

            s << "\u2588";

            return;
        }

        enum : int { DIGITS = 10, LETTERS = 26 };

        const int i = island % (DIGITS + LETTERS + LETTERS);

        char c =
            (i < DIGITS)
            ? '0' + i
            : (i < DIGITS + LETTERS)
              ? 'a' + (i - DIGITS)
              : 'A' + (i - DIGITS - LETTERS);

        s << c;
    }

    void
    print_islands(const map& m, std::FILE* out)
    {
        std::stringstream s;

        const unsigned rows = m.row_count();
        const unsigned cols = m.col_count();

        print_upper_outline(s, m); print_upper_outline(s, m); s << '\n';

        for (unsigned row = 0; row < rows; ++row)
        {
            print_left_outline(s);

            for (unsigned col = 0; col < cols; ++col)
            {
                print_island(s, m, {row, col, TERRAIN});
            }

            print_right_outline(s);
            print_left_outline(s);

            for (unsigned col = 0; col < cols; ++col)
            {
                print_island(s, m, {row, col, BRIDGES});
            }

            print_right_outline(s); s << '\n';
        }

        print_lower_outline(s, m); print_lower_outline(s, m);

        fputs(s.str().c_str(), out);
    }

    template <typename Container, typename T>
    bool contains(const Container& c, const T& t)
    {
        for (auto& v : c) if (v == t) return true;

        return false;
    }

    static
    void
    print_path_tile(
        std::ostream&            s,
        const map&               m,
        const std::vector<tile>& path,
        const tile               goal,
        const tile               t)
    {
        auto* head = &*path.begin();
        auto* tail = &*path.end();
        auto* back = tail - 1;

        auto* itr = std::find(head, tail, t);

        if (m.obstructed(t))
        {
            assert(itr == tail);

            if (*itr == goal) return (void)(s << 'x');

            s << "\u2588";

            return;
        }

        if (itr == head) return (void)(s << 'a');

        if (itr == back) return (void)(s << 'b');

        if (t == goal) return (void)(s << 'x');

        if (itr != tail) return (void)(s << "\u2022");

        if (m.cost(t)) return (void)(s << "\u2591");

        s << ' ';
    }

    static
    void
    print_path(
        const map&               m,
        const std::vector<tile>& path,
        const tile               goal)
    {
        std::stringstream s;

        const unsigned rows = m.row_count();
        const unsigned cols = m.col_count();

        print_upper_outline(s, m); print_upper_outline(s, m); s << '\n';

        for (unsigned row = 0; row < rows; ++row)
        {
            print_left_outline(s);

            for (unsigned col = 0; col < cols; ++col)
            {
                const tile t {row, col, TERRAIN};
                print_path_tile(s, m, path, goal, t);
            }

            print_right_outline(s);
            print_left_outline(s);

            for (unsigned col = 0; col < cols; ++col)
            {
                const tile t {row, col, BRIDGES};
                print_path_tile(s, m, path, goal, t);
            }

            print_right_outline(s); s << '\n';
        }

        print_lower_outline(s, m); print_lower_outline(s, m);

        log(s.str().c_str());
    }

    static
    void
    print_path(const map& m, const std::vector<tile>& path)
    {
        print_path(m, path, path.back());
    }

    static
    void
    print_path(const map& m, const tile a, const tile b)
    {
        navigator n(m);

        n.navigate(a, b);

        std::vector<tile> path; path << n.path();

        print_path(m, path);
    }

#define PROFILE 0

    static
    void sleep(size_t ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    static
    void test_unreachable_corner(const unsigned ROWS, const unsigned COLS, const bool profile)
    {
        printf("%s(%u, %u)\n", __func__, ROWS, COLS);

        height_map hm(ROWS + 1, COLS + 1);

        for (unsigned row = 0; row < hm.row_count(); ++row)
        for (unsigned col = 0; col < hm.col_count(); ++col)
        {
            const auto i = hm.cell_index(row, col);

            hm.layer(TERRAIN)[i] = 0.f;
            hm.layer(BRIDGES)[i] = 4.f;
        }

        // surround (D-1,D-1) with a wall
        //hm.layer(TERRAIN)[hm.cell_index(ROWS-2, COLS-2)] = 2.f;
        hm.layer(TERRAIN)[hm.cell_index(ROWS-2, COLS-1)] = 2.f;
        hm.layer(TERRAIN)[hm.cell_index(ROWS-1, COLS-2)] = 2.f;

        cell NW { 0, 0 };
        cell SE { ROWS - 1, COLS - 1 };

        map m(hm);

        navigator n(m);

        if (profile) sleep(100);

        TIMED_SCOPE(profile ? "find path 32x" : "find path")
        {
            for (int i = 0; i <= 32 * profile; ++i)
                n.navigate(NW[TERRAIN], SE[TERRAIN]);
        }

        std::vector<tile> path; path << n.path();

        if (COLS <= 64)
        {
            print_islands(m);

            print_path(m, path, SE[TERRAIN]);
        }
    }

    static
    void test_horizontal_maze(const unsigned ROWS, const unsigned COLS, const bool profile)
    {
        printf("%s(%u, %u)\n", __func__, ROWS, COLS);

        height_map h(ROWS + 1, COLS + 1);

        for (unsigned row = 0; row < h.row_count(); ++row)
        for (unsigned col = 0; col < h.col_count(); ++col)
        {
            const auto i = h.cell_index(row, col);

            const bool lower_passage = (col > COLS - 2) and (row % 6 == 2);
            const bool upper_passage = (col < 2)        and (row % 6 == 5);
            const bool passage       = lower_passage or upper_passage;
            const bool wall          = (row % 3 == 2) and not passage;

            h.layer(TERRAIN)[i] = float(wall);
            h.layer(BRIDGES)[i] = 4.f;
        }

        cell NW { 0, 0 };
        cell SE { ROWS - 1, COLS - 1 };

        map m(h);

        navigator n(m);

        if (profile) sleep(100);

        TIMED_SCOPE(profile ? "find path 32x" : "find path")
        {
            for (int i = 0; i <= 32 * profile; ++i)
                n.navigate(NW[TERRAIN], SE[TERRAIN]);
        }

        std::vector<tile> path; path << n.path();

        if (COLS <= 64)
        {
            print_islands(m);

            print_path(m, path, SE[TERRAIN]);
        }
    }

    static
    void test_vertical_maze(const unsigned ROWS, const unsigned COLS, const bool profile)
    {
        printf("%s(%u, %u)\n", __func__, ROWS, COLS);

        height_map h(ROWS + 1, COLS + 1);

        for (unsigned row = 0; row < h.row_count(); ++row)
        for (unsigned col = 0; col < h.col_count(); ++col)
        {
            const auto i = h.cell_index(row, col);

            const bool lower_passage = (row > ROWS - 2) and (col % 6 == 2);
            const bool upper_passage = (row < 2)        and (col % 6 == 5);
            const bool passage       = lower_passage or upper_passage;
            const bool wall          = (col % 3 == 2) and not passage;

            h.layer(TERRAIN)[i] = float(wall);
            h.layer(BRIDGES)[i] = 4.f;
        }

        cell NW { 0, 0 };
        cell SE { ROWS - 1, COLS - 1 };

        map m(h);

        navigator n(m);

        if (profile) sleep(100);

        TIMED_SCOPE(profile ? "find path 32x" : "find path")
        {
            for (int i = 0; i <= 32 * profile; ++i)
                n.navigate(NW[TERRAIN], SE[TERRAIN]);
        }

        std::vector<tile> path; path << n.path();

        if (COLS <= 64)
        {
            print_islands(m);

            print_path(m, path, SE[TERRAIN]);
        }
    }

    static
    void test_fortress(const unsigned ROWS, const unsigned COLS, bool profile)
    {
        printf("%s(%u, %u)\n", __func__, ROWS, COLS);

        height_map h(ROWS + 1, COLS + 1);

        const int R = 2;

        for (unsigned row = 0; row < h.row_count(); ++row)
        for (unsigned col = 0; col < h.col_count(); ++col)
        {
            const auto i = h.cell_index(row, col);

            const auto rx = math::abs(int(col) - int(COLS / 2));
            const auto ry = math::abs(int(row) - int(ROWS / 2));

            const bool wall_x = (rx == R) and (ry < R);
            const bool wall_y = (ry == R) and (rx < R);
            const bool wall   = wall_x or wall_y;

            h.layer(TERRAIN)[i] = float(wall);
            h.layer(BRIDGES)[i] = 4.f;
        }

        map m(h);

        if (COLS <= 64) print_islands(m);

        navigator n(m);

        {
            cell NW { 0, 0 };
            cell SE { ROWS - 1, COLS - 1 };

            if (profile) sleep(100);

            TIMED_SCOPE(profile ? "find path to far corner 32x" : "find path to far corner")
            {
                for (int i = 0; i <= 32 * profile; ++i)
                    n.navigate(NW[TERRAIN], SE[TERRAIN]);
            }

            std::vector<tile> path; path << n.path();

            if (COLS <= 64) print_path(m, path, SE[TERRAIN]);
        }

        {
            cell NW { 0, 0 };
            cell SE { ROWS / 2 - 1, COLS / 2 - 1 };

            if (profile) sleep(100);

            TIMED_SCOPE(profile ? "find path to near center 32x" : "find path to near center")
            {
                for (int i = 0; i <= 32 * profile; ++i)
                    n.navigate(NW[TERRAIN], SE[TERRAIN]);
            }

            std::vector<tile> path; path << n.path();

            if (COLS <= 64) print_path(m, path, SE[TERRAIN]);
        }

        {
            cell NW { 0, 0 };
            cell SE { ROWS / 2, COLS / 2 };

            if (profile) sleep(100);

            TIMED_SCOPE(profile ? "find path to far center 32x" : "find path to far center")
            {
                for (int i = 0; i <= 32 * profile; ++i)
                    n.navigate(NW[TERRAIN], SE[TERRAIN]);
            }

            std::vector<tile> path; path << n.path();

            if (COLS <= 64) print_path(m, path, SE[TERRAIN]);
        }
    }

    static
    void test_city(const unsigned ROWS, const unsigned COLS, bool profile)
    {
        printf("%s(%u, %u)\n", __func__, ROWS, COLS);

        height_map h(ROWS + 1, COLS + 1);

        for (unsigned row = 0; row < h.row_count(); ++row)
        for (unsigned col = 0; col < h.col_count(); ++col)
        {
            const auto i = h.cell_index(row, col);

            h.layer(TERRAIN)[i] = 0.f;
            h.layer(BRIDGES)[i] = 4.f;
        }

        map m(h);

        for (unsigned row = 0; row < m.row_count(); ++row)
        for (unsigned col = 0; col < m.col_count(); ++col)
        {
            const bool street = !(row % 3) or !(col % 3);

            if (not street) m.add_barrier({ row, col, TERRAIN });
        }

        m.resolve_barriers();

        if (COLS <= 64) print_islands(m);

        navigator n(m);

        {
            cell NW { 0, 0 };
            cell SE { ROWS - 1, COLS - 1 };

            if (profile) sleep(100);

            TIMED_SCOPE(profile ? "find path to far corner 32x" : "find path to far corner")
            {
                for (int i = 0; i <= 32 * profile; ++i)
                    n.navigate(NW[TERRAIN], SE[TERRAIN]);
            }

            std::vector<tile> path; path << n.path();

            if (COLS <= 64) print_path(m, path, SE[TERRAIN]);
        }

        {
            cell NW { 0, 0 };
            cell SE { ROWS / 2 - 1, COLS / 2 - 1 };

            if (profile) sleep(100);

            TIMED_SCOPE(profile ? "find path to near center 32x" : "find path to near center")
            {
                for (int i = 0; i <= 32 * profile; ++i)
                    n.navigate(NW[TERRAIN], SE[TERRAIN]);
            }

            std::vector<tile> path; path << n.path();

            if (COLS <= 64) print_path(m, path, SE[TERRAIN]);
        }

        {
            cell NW { 0, 0 };
            cell SE { ROWS / 2, COLS / 2 };

            if (profile) sleep(100);

            TIMED_SCOPE(profile ? "find path to far center 32x" : "find path to far center")
            {
                for (int i = 0; i <= 32 * profile; ++i)
                    n.navigate(NW[TERRAIN], SE[TERRAIN]);
            }

            std::vector<tile> path; path << n.path();

            if (COLS <= 64) print_path(m, path, SE[TERRAIN]);
        }
    }

    static
    void test_city_castle(const unsigned ROWS, const unsigned COLS, bool profile)
    {
        printf("%s(%u, %u)\n", __func__, ROWS, COLS);

        height_map h(ROWS + 1, COLS + 1);

        for (unsigned row = 0; row < h.row_count(); ++row)
        for (unsigned col = 0; col < h.col_count(); ++col)
        {
            const auto i = h.cell_index(row, col);

            h.layer(TERRAIN)[i] = 0.f;
            h.layer(BRIDGES)[i] = 4.f;
        }

        map m(h);

        const unsigned north_wall_row = ROWS / 3;
        const unsigned south_wall_row = north_wall_row * 2;

        const unsigned west_wall_col = COLS / 3;
        const unsigned east_wall_col = west_wall_col * 2;

        const unsigned west_gate_col = COLS / 2 - 2;
        const unsigned east_gate_col = west_gate_col + 2;

        for (unsigned row = 0; row < m.row_count(); ++row)
        for (unsigned col = 0; col < m.col_count(); ++col)
        {
            const bool gate_row(row == south_wall_row);
            const bool gate_col(col >= west_gate_col & col <= east_gate_col);
            const bool gate = gate_row & gate_col;

            if (gate) continue;

            const bool yard_row(row - 1 >= north_wall_row & row + 1 <= south_wall_row);
            const bool yard_col(col - 1 >=  west_wall_col & col + 1 <=  east_wall_col);
            const bool yard = yard_row & yard_col;

            if (yard) continue;

            const bool wall_row(row >= north_wall_row & row <= south_wall_row);
            const bool wall_col(col >=  west_wall_col & col <=  east_wall_col);
            const bool wall = wall_row & wall_col;

            if (wall) { m.add_barrier({ row, col, TERRAIN }); continue; }

            const bool street = !(row % 3) or !(col % 3);

            if (not street) m.add_barrier({ row, col, TERRAIN });
        }

        m.resolve_barriers();

        if (COLS <= 64) print_islands(m);

        navigator n(m);

        {
            cell NW { 0, 0 };
            cell SE { ROWS - 1, COLS - 1 };

            if (profile) sleep(100);

            TIMED_SCOPE(profile ? "find path to far corner 32x" : "find path to far corner")
            {
                for (int i = 0; i <= 32 * profile; ++i)
                    n.navigate(NW[TERRAIN], SE[TERRAIN]);
            }

            std::vector<tile> path; path << n.path();

            if (COLS <= 64) print_path(m, path, SE[TERRAIN]);
        }

        {
            cell NW { 0, 0 };
            cell SE { ROWS / 2 - 1, COLS / 2 - 1 };

            if (profile) sleep(100);

            TIMED_SCOPE(profile ? "find path to near center 32x" : "find path to near center")
            {
                for (int i = 0; i <= 32 * profile; ++i)
                    n.navigate(NW[TERRAIN], SE[TERRAIN]);
            }

            std::vector<tile> path; path << n.path();

            if (COLS <= 64) print_path(m, path, SE[TERRAIN]);
        }

        {
            cell NW { 0, 0 };
            cell SE { ROWS / 2, COLS / 2 };

            if (profile) sleep(100);

            TIMED_SCOPE(profile ? "find path to far center 32x" : "find path to far center")
            {
                for (int i = 0; i <= 32 * profile; ++i)
                    n.navigate(NW[TERRAIN], SE[TERRAIN]);
            }

            std::vector<tile> path; path << n.path();

            if (COLS <= 64) print_path(m, path, SE[TERRAIN]);
        }
    }

    static
    void test_fire(const unsigned ROWS, const unsigned COLS, bool profile)
    {
        printf("%s(%u, %u)\n", __func__, ROWS, COLS);

        height_map h(ROWS + 1, COLS + 1);

        for (unsigned row = 0; row < h.row_count(); ++row)
        for (unsigned col = 0; col < h.col_count(); ++col)
        {
            const auto i = h.cell_index(row, col);

            h.layer(TERRAIN)[i] = 0.f;
            h.layer(BRIDGES)[i] = 4.f;
        }

        map m(h);

        const point fire_center { ROWS / 2.f, COLS / 2.f, 0.f };

        const unsigned fire_radius = math::min(ROWS, COLS) / 3;

        const unsigned fire_height = 2.f;

        m.add_cost(fire_center, fire_radius, fire_height, 2);

        if (COLS <= 64) print_islands(m);

        navigator n(m);

        {
            cell NW { 0, 0 };
            cell SE { ROWS - 1, COLS - 1 };

            if (profile) sleep(100);

            TIMED_SCOPE(profile ? "find path to far corner 32x" : "find path to far corner")
            {
                for (int i = 0; i <= 32 * profile; ++i)
                    n.navigate(NW[TERRAIN], SE[TERRAIN]);
            }

            std::vector<tile> path; path << n.path();

            if (COLS <= 64) print_path(m, path, SE[TERRAIN]);
        }

        {
            cell NW { 0, 0 };
            cell SE { ROWS / 2 - 1, COLS / 2 - 1 };

            if (profile) sleep(100);

            TIMED_SCOPE(profile ? "find path to near center 32x" : "find path to near center")
            {
                for (int i = 0; i <= 32 * profile; ++i)
                    n.navigate(NW[TERRAIN], SE[TERRAIN]);
            }

            std::vector<tile> path; path << n.path();

            if (COLS <= 64) print_path(m, path, SE[TERRAIN]);
        }

        {
            cell NW { 0, 0 };
            cell SE { ROWS / 2, COLS / 2 };

            if (profile) sleep(100);

            TIMED_SCOPE(profile ? "find path to far center 32x" : "find path to far center")
            {
                for (int i = 0; i <= 32 * profile; ++i)
                    n.navigate(NW[TERRAIN], SE[TERRAIN]);
            }

            std::vector<tile> path; path << n.path();

            if (COLS <= 64) print_path(m, path, SE[TERRAIN]);
        }
    }

    static
    void test_nav(const unsigned ROWS, const unsigned COLS, bool profile = false)
    {
        if (profile) sleep(500);

        puts("===============================================================");
        test_unreachable_corner(ROWS, COLS, profile);

        if (profile) sleep(500);

        puts("---------------------------------------------------------------");
        test_horizontal_maze(ROWS, COLS, profile);

        if (profile) sleep(500);

        puts("---------------------------------------------------------------");
        test_vertical_maze(ROWS, COLS, profile);

        if (profile) sleep(500);

        puts("---------------------------------------------------------------");
        test_fortress(ROWS, COLS, profile);

        if (profile) sleep(500);

        puts("---------------------------------------------------------------");
        test_city(ROWS, COLS, profile);

        if (profile) sleep(500);

        puts("---------------------------------------------------------------");
        test_city_castle(ROWS, COLS, profile);

        if (profile) sleep(500);

        puts("---------------------------------------------------------------");
        test_fire(ROWS, COLS, profile);
    }

    void tests()
    {
        math::functions::tests();
        nav::map::tests();
        nav::navigator::tests();
        nav::neighbor_link::tests();
        nav::tile::tests();

        { // check whether a map can be created from an empty height map
            height_map h(3, 3);

            print_heights(h);

            map m(h);

            print_islands(m);
        }

        { // try simple map with bridge over river
            constexpr const float N = -INFINITY;

            height_map h {
                { // terrain                           river
                    { 1, 1, 0, 0, 1, 1 }, // bank   +-+\___/+-+
                    { 1, 1, 0, 0, 1, 1 }, // bank   +-+\___/+-+
                    { 1, 1, 0, 0, 1, 1 }, // bank   +-+\___/+-+
                    { 1, 1, 0, 0, 1, 1 }, // bank   +-+\___/+-+
                },
                { // bridges
                    { N, N, N, N, N, N },
                    { N, 1, 1, 1, 1, N }, // bridge   +-+-+-+
                    { N, 1, 1, 1, 1, N }, // bridge   +-+-+-+
                    { N, N, N, N, N, N },
                }
            };

            print_heights(h);

            map m(h);

            print_islands(m);

            print_path(m, {0, 0, TERRAIN}, {2, 4, TERRAIN});

            print_path(m, {0, 0, TERRAIN}, {2, 3, TERRAIN});

            print_path(m, {0, 0, TERRAIN}, {0, 2, TERRAIN});
        }

        { // try simple map with walled corner
            constexpr const float N = -INFINITY;

            height_map h {
                { // terrain
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 2, 2 },
                    { 0, 0, 0, 2, 0, 0 },
                    { 0, 0, 0, 2, 0, 0 },
                },
                { // bridges
                    { N, N, N, N, N, N },
                    { N, N, N, N, N, N },
                    { N, N, N, N, N, N },
                    { N, N, N, N, N, N },
                    { N, N, N, N, N, N },
                    { N, N, N, N, N, N },
                }
            };

            print_heights(h);

            map m(h);

            print_islands(m);

            print_path(m, {0, 0, TERRAIN}, {4, 4, TERRAIN});
        }

        { // try rasterized heightfield

            height_map h(10, 10);

            point v0 {2.5, 4.5, 0}, v1 {6.5, 6.5, 1}, v2 {6.5, 2.5, 2};

            h.rasterize(TERRAIN, v0, v1, v2); // clockwise

            print_heights(h);
        }

        { // try rasterized heightfield (counter clockwise)

            height_map h(10, 10);

            point v0 {2.5, 4.5, 0}, v1 {6.5, 6.5, 1}, v2 {6.5, 2.5, 2};

            h.rasterize(TERRAIN, v0, v2, v1); // counter clockwise

            print_heights(h);
        }

        test_nav( 16,  32);
        test_nav(128, 128);
        test_nav(256, 256);

    #if env_os_osx
        test_nav(1024, 1024, true);
    #endif
    }

    void
    log(const char* c_str)
    {
        fprintf(stderr, "\n%s\n", c_str);
    }

} // namespace nav