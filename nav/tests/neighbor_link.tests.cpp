#include <cassert>
#include <zna/source/thirdParty/gtoolbox/nav/neighbor_link.h>
#include <zna/source/thirdParty/gtoolbox/test/test.h>

namespace nav {
    void neighbor_link_test(neighbor_link& n, direction_index d);

    void neighbor_link_test(neighbor_link& n, direction_index d)
    {
        n.exists(d, true);
        test_assert(n.exists(NORTH)         == (d == NORTH));
        test_assert(n.exists(SOUTH)         == (d == SOUTH));
        test_assert(n.exists(EAST)          == (d == EAST));
        test_assert(n.exists(WEST)          == (d == WEST));
        test_assert(n.north_neighbor_exists == (d == NORTH));
        test_assert(n.south_neighbor_exists == (d == SOUTH));
        test_assert(n.east_neighbor_exists  == (d == EAST));
        test_assert(n.west_neighbor_exists  == (d == WEST));

        n.exists(d, false);
        test_assert(n.exists(NORTH)         == false);
        test_assert(n.exists(SOUTH)         == false);
        test_assert(n.exists(EAST)          == false);
        test_assert(n.exists(WEST)          == false);
        test_assert(n.north_neighbor_exists == false);
        test_assert(n.south_neighbor_exists == false);
        test_assert(n.east_neighbor_exists  == false);
        test_assert(n.west_neighbor_exists  == false);

        n.layer(d, TERRAIN);
        test_assert(n.layer(d)             == TERRAIN);
        test_assert(n.north_neighbor_layer == TERRAIN or (d != NORTH));
        test_assert(n.south_neighbor_layer == TERRAIN or (d != SOUTH));
        test_assert(n.east_neighbor_layer  == TERRAIN or (d != EAST));
        test_assert(n.west_neighbor_layer  == TERRAIN or (d != WEST));

        n.layer(d, BRIDGES);
        test_assert(n.layer(d)             == BRIDGES);
        test_assert(n.north_neighbor_layer == BRIDGES or (d != NORTH));
        test_assert(n.south_neighbor_layer == BRIDGES or (d != SOUTH));
        test_assert(n.east_neighbor_layer  == BRIDGES or (d != EAST));
        test_assert(n.west_neighbor_layer  == BRIDGES or (d != WEST));

        test_assert(n.exists(NORTH)         == false);
        test_assert(n.exists(SOUTH)         == false);
        test_assert(n.exists(EAST)          == false);
        test_assert(n.exists(WEST)          == false);
        test_assert(n.north_neighbor_exists == false);
        test_assert(n.south_neighbor_exists == false);
        test_assert(n.east_neighbor_exists  == false);
        test_assert(n.west_neighbor_exists  == false);
    }

    void neighbor_link::tests()
    {
        neighbor_link n;

        neighbor_link_test(n, NORTH);
        neighbor_link_test(n, SOUTH);
        neighbor_link_test(n, EAST);
        neighbor_link_test(n, WEST);
    }

} // namespace nav