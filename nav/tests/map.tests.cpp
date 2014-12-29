#include <zna/source/thirdParty/gtoolbox/nav/map.h>
#include <zna/source/thirdParty/gtoolbox/test/test.h>

namespace nav {

    template <typename T>
    using args = std::initializer_list<T>;

    template <typename T>
    using args2D = args<args<T>>;

    template <typename T>
    using args3D = args<args<args<T>>>;

//    template <layer_index LAYER, typename T>
//    static
//    void
//    test_layer(T (map::*fn)(tile) const, const map& m, args2D<T> state)
//    {
//        const unsigned row_count = m.row_count();
//        const unsigned col_count = m.col_count();
//
//        test_assert(state.size() == row_count);
//
//        for (unsigned row = 0; row < row_count; ++row)
//        {
//            auto& state_row = state.begin()[row];
//
//            test_assert(state_row.size() == col_count);
//
//            for (unsigned col = 0; col < col_count; ++col)
//            {
//                const nav::tile tile { row, col, LAYER };
//
//                const T expected_state = state_row.begin()[col];
//
//                const T actual_state = (m.*fn)(tile);
//
//                test_assert(actual_state == expected_state);
//            }
//        }
//    }

    template <layer_index LAYER, typename T, typename F>
    static
    void
    test_layer(F fn, const map& m, args2D<T> state)
    {
        const unsigned row_count = m.row_count();
        const unsigned col_count = m.col_count();

        test_assert(state.size() == row_count);

        for (unsigned row = 0; row < row_count; ++row)
        {
            auto& state_row = state.begin()[row];

            test_assert(state_row.size() == col_count);

            for (unsigned col = 0; col < col_count; ++col)
            {
                const nav::tile tile { row, col, LAYER };

                const T expected_state = state_row.begin()[col];

                const T actual_state = fn(m, tile);

                test_assert(actual_state == expected_state);
            }
        }
    }

    template <typename T, typename F>
    static
    void
    test_map(F fn, const map& map, args3D<T> state)
    {
        test_assert(state.size() == LAYER_COUNT);

        test_layer<TERRAIN>(fn, map, state.begin()[TERRAIN]);
        test_layer<BRIDGES>(fn, map, state.begin()[BRIDGES]);
    }

    template <typename T>
    static
    void
    test_map(T (map::*method)(tile) const, const map& m, args3D<T> state)
    {
        auto fn = [&](const nav::map& m, tile t){ return (m.*method)(t); };

        test_map(fn, m, state);
    }

    void map::tests()
    {
        constexpr const float N = -INFINITY;

        height_map hm {
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

        map m(hm);

        test_assert(m.row_count() == 3);
        test_assert(m.col_count() == 5);

        // TERRAIN BRIDGES
        // +-----+ +-----+
        // |1#2#1| |#####| numbers: islands
        // |1#2#1| |#111#|
        // |1#2#1| |#####| hashes: obstacles
        // +-----+ +-----+

        test_map(&map::island, m, {
            { // terrain
                { 1, 0, 2, 0, 1 },
                { 1, 0, 2, 0, 1 },
                { 1, 0, 2, 0, 1 },
            },
            { // bridges
                { 0, 0, 0, 0, 0 },
                { 0, 1, 1, 1, 0 },
                { 0, 0, 0, 0, 0 },
            },
        });

        test_map(&map::obstacle_count, m, {
            { // terrain
                { 0, 1, 0, 1, 0 },
                { 0, 1, 0, 1, 0 },
                { 0, 1, 0, 1, 0 },
            },
            { // bridges
                { 1, 1, 1, 1, 1 },
                { 1, 0, 0, 0, 1 },
                { 1, 1, 1, 1, 1 },
            },
        });

        // test terrain neighbors

        test_assert((m.neighbors({0, 0, TERRAIN}).only<SOUTH>()));
        test_assert((m.neighbors({1, 0, TERRAIN}).only<NORTH, EAST, SOUTH>()));
        test_assert((m.neighbors({2, 0, TERRAIN}).only<NORTH>()));

        test_assert((m.neighbors({0, 1, TERRAIN}).none()));
        test_assert((m.neighbors({1, 1, TERRAIN}).none()));
        test_assert((m.neighbors({2, 1, TERRAIN}).none()));

        test_assert((m.neighbors({0, 2, TERRAIN}).only<SOUTH>()));
        test_assert((m.neighbors({1, 2, TERRAIN}).only<NORTH, SOUTH>()));
        test_assert((m.neighbors({2, 2, TERRAIN}).only<NORTH>()));

        test_assert((m.neighbors({0, 3, TERRAIN}).none()));
        test_assert((m.neighbors({1, 3, TERRAIN}).none()));
        test_assert((m.neighbors({2, 3, TERRAIN}).none()));

        test_assert((m.neighbors({0, 4, TERRAIN}).only<SOUTH>()));
        test_assert((m.neighbors({1, 4, TERRAIN}).only<NORTH, WEST, SOUTH>()));
        test_assert((m.neighbors({2, 4, TERRAIN}).only<NORTH>()));

        // test terrain neighbor layers

        test_assert((m.neighbors({0, 0, TERRAIN}).layer(SOUTH) == TERRAIN));
        test_assert((m.neighbors({1, 0, TERRAIN}).layer(NORTH) == TERRAIN));
        test_assert((m.neighbors({1, 0, TERRAIN}).layer(EAST)  == BRIDGES));
        test_assert((m.neighbors({1, 0, TERRAIN}).layer(SOUTH) == TERRAIN));
        test_assert((m.neighbors({2, 0, TERRAIN}).layer(NORTH) == TERRAIN));

        test_assert((m.neighbors({0, 2, TERRAIN}).layer(SOUTH) == TERRAIN));
        test_assert((m.neighbors({1, 2, TERRAIN}).layer(NORTH) == TERRAIN));
        test_assert((m.neighbors({1, 2, TERRAIN}).layer(SOUTH) == TERRAIN));
        test_assert((m.neighbors({2, 2, TERRAIN}).layer(NORTH) == TERRAIN));

        test_assert((m.neighbors({0, 4, TERRAIN}).layer(SOUTH) == TERRAIN));
        test_assert((m.neighbors({1, 4, TERRAIN}).layer(NORTH) == TERRAIN));
        test_assert((m.neighbors({1, 4, TERRAIN}).layer(WEST)  == BRIDGES));
        test_assert((m.neighbors({1, 4, TERRAIN}).layer(SOUTH) == TERRAIN));
        test_assert((m.neighbors({2, 4, TERRAIN}).layer(NORTH) == TERRAIN));

        // test bridge neighbors

        test_assert((m.neighbors({0, 0, BRIDGES}).none()));
        test_assert((m.neighbors({1, 0, BRIDGES}).none()));
        test_assert((m.neighbors({2, 0, BRIDGES}).none()));

        test_assert((m.neighbors({0, 1, BRIDGES}).none()));
        test_assert((m.neighbors({1, 1, BRIDGES}).only<WEST, EAST>()));
        test_assert((m.neighbors({2, 1, BRIDGES}).none()));

        test_assert((m.neighbors({0, 2, BRIDGES}).none()));
        test_assert((m.neighbors({1, 2, BRIDGES}).only<WEST, EAST>()));
        test_assert((m.neighbors({2, 2, BRIDGES}).none()));

        test_assert((m.neighbors({0, 3, BRIDGES}).none()));
        test_assert((m.neighbors({1, 3, BRIDGES}).only<WEST, EAST>()));
        test_assert((m.neighbors({2, 3, BRIDGES}).none()));

        test_assert((m.neighbors({0, 4, BRIDGES}).none()));
        test_assert((m.neighbors({1, 4, BRIDGES}).none()));
        test_assert((m.neighbors({2, 4, BRIDGES}).none()));

        // test bridge neighbor layers

        test_assert((m.neighbors({1, 1, BRIDGES}).layer(WEST) == TERRAIN));
        test_assert((m.neighbors({1, 1, BRIDGES}).layer(EAST) == BRIDGES));

        test_assert((m.neighbors({1, 2, BRIDGES}).layer(WEST) == BRIDGES));
        test_assert((m.neighbors({1, 2, BRIDGES}).layer(EAST) == BRIDGES));

        test_assert((m.neighbors({1, 3, BRIDGES}).layer(WEST) == BRIDGES));
        test_assert((m.neighbors({1, 3, BRIDGES}).layer(EAST) == TERRAIN));

        // test barriers

        { // NW barrier --------------------------------------------------------

            auto map_barrier_count = [](const nav::map& m, nav::tile t) {
                return m.obstacles(t).barrier_count;
            };

            const uint8_t _ = 0;

            test_map<uint8_t>(map_barrier_count, m, {
                { // terrain
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                },
                { // bridges
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                },
            });

            const nav::point barrier[2] { { 0, 0, 0 }, { 2, 2, 2 } };

            m.add_barrier(barrier[0], barrier[1]);

            test_assert(m.has_unresolved_barriers());

            test_map<uint8_t>(map_barrier_count, m, {
                { // terrain
                    { 1, 1, _, _, _ },
                    { 1, 1, _, _, _ },
                    { _, _, _, _, _ },
                },
                { // bridges
                    { _, _, _, _, _ },
                    { _, 1, _, _, _ },
                    { _, _, _, _, _ },
                },
            });

            m.add_barrier(barrier[0], barrier[1]);

            test_assert(m.has_unresolved_barriers());

            test_map<uint8_t>(map_barrier_count, m, {
                { // terrain
                    { 2, 2, _, _, _ },
                    { 2, 2, _, _, _ },
                    { _, _, _, _, _ },
                },
                { // bridges
                    { _, _, _, _, _ },
                    { _, 2, _, _, _ },
                    { _, _, _, _, _ },
                },
            });

            m.remove_barrier(barrier[0], barrier[1]);

            test_assert(m.has_unresolved_barriers());

            test_map<uint8_t>(map_barrier_count, m, {
                { // terrain
                    { 1, 1, _, _, _ },
                    { 1, 1, _, _, _ },
                    { _, _, _, _, _ },
                },
                { // bridges
                    { _, _, _, _, _ },
                    { _, 1, _, _, _ },
                    { _, _, _, _, _ },
                },
            });

            m.remove_barrier(barrier[0], barrier[1]);

            test_assert(m.has_unresolved_barriers());

            test_map<uint8_t>(map_barrier_count, m, {
                { // terrain
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                },
                { // bridges
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                },
            });

            m.resolve_barriers();

            test_assert(not m.has_unresolved_barriers());
        }

        { // SE barrier --------------------------------------------------------

            auto map_barrier_count = [](const nav::map& m, nav::tile t) {
                return m.obstacles(t).barrier_count;
            };

            const uint8_t _ = 0;

            test_map<uint8_t>(map_barrier_count, m, {
                { // terrain
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                },
                { // bridges
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                },
            });

            const nav::point barrier[2] { { 1, 3, 0 }, { 3, 5, 2 } };

            m.add_barrier(barrier[0], barrier[1]);

            test_assert(m.has_unresolved_barriers());

            test_map<uint8_t>(map_barrier_count, m, {
                { // terrain
                    { _, _, _, _, _ },
                    { _, _, _, 1, 1 },
                    { _, _, _, 1, 1 },
                },
                { // bridges
                    { _, _, _, _, _ },
                    { _, _, _, 1, _ },
                    { _, _, _, _, _ },
                },
            });

            m.remove_barrier(barrier[0], barrier[1]);

            test_assert(m.has_unresolved_barriers());

            test_map<uint8_t>(map_barrier_count, m, {
                { // terrain
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                },
                { // bridges
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                },
            });

            m.resolve_barriers();

            test_assert(not m.has_unresolved_barriers());
        }

        // test costs

        { // W cost ------------------------------------------------------------

            const uint8_t _ = 0;

            test_map<uint8_t>(&map::cost, m, {
                { // terrain
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                },
                { // bridges
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                },
            });

            const nav::point cost_center { 1.5f, 1.5f, 0 };

            const unsigned cost_radius = 1;

            const unsigned cost_height = 2;

            m.add_cost(cost_center, cost_radius, cost_height, 2);

            test_map<uint8_t>(&map::cost, m, {
                { // terrain
                    { _, 2, _, _, _ },
                    { 2, 2, 2, _, _ },
                    { _, 2, _, _, _ },
                },
                { // bridges
                    { _, _, _, _, _ },
                    { _, 2, 2, _, _ },
                    { _, _, _, _, _ },
                },
            });

            m.remove_cost(cost_center, cost_radius, cost_height, 2);

            test_map<uint8_t>(&map::cost, m, {
                { // terrain
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                },
                { // bridges
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                    { _, _, _, _, _ },
                },
            });
        }

        // TODO: test barriers/costs that overrun map boundaries
        // e.g. negative rows, or cols >= map.col_count()

    }

} // namespace nav