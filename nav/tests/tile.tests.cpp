#include <gtoolbox/nav/cell.h>
#include <gtoolbox/test/test.h>

namespace nav {

    void tile::tests()
    {
        tile t;

        test_assert(t.row     == 0);
        test_assert(t.col     == 0);
        test_assert(t.layer   == TERRAIN);
        test_assert(t.visited == false);

        t.row   = 32767;
        t.col   = 16383;
        t.layer = TERRAIN;

        test_assert(t.row     == 32767);
        test_assert(t.col     == 16383);
        test_assert(t.layer   == TERRAIN);
        test_assert(t.visited == false);

        t.row   = 16383;
        t.col   = 32767;
        t.layer = BRIDGES;

        test_assert(t.row     == 16383);
        test_assert(t.col     == 32767);
        test_assert(t.layer   == BRIDGES);
        test_assert(t.visited == false);

        t.visited = true;

        test_assert(t.row     == 16383);
        test_assert(t.col     == 32767);
        test_assert(t.layer   == BRIDGES);
        test_assert(t.visited == true);

        tile s { 16383, 32767, BRIDGES };

        test_assert(t == s); // equality ignores 'visited'
        test_assert(s.visited == false);

        const cell& c = t;

        test_assert(c == s);
    }

} // namespace nav