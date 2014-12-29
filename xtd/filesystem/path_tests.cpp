#if TEST
#include <catch.h>
#include "./path.h"

//------------------------------------------------------------------------------

TEST_CASE("xtd::path tests", "[xtd][filesystem]")
{
    using namespace xtd;

    SECTION("root path exists")
    {
        REQUIRE(path("/").exists());
        REQUIRE(path("/").is_directory());
        REQUIRE(path("/") == path("/"));
    }

    SECTION("root path not equal to first subpath in root")
    {
        path a = path("/");
        path b = *a.begin();

        REQUIRE(a != b);
    }

    SECTION("separate enumerators return same results")
    {
        path a = path("/");

        auto itr_1 = a.begin();
        auto itr_2 = a.begin();

        auto end = a.end();

        while (itr_1 != end)
        {
            REQUIRE(itr_1 == itr_2);

            auto parent_1 = itr_1->parent();
            auto parent_2 = itr_2->parent();

            REQUIRE(parent_1 == a);
            REQUIRE(parent_2 == a);
            REQUIRE(parent_1 == parent_2);

            ++itr_1;
            ++itr_2;
        }
    }

    SECTION("test substring properties")
    {
    }
}

//------------------------------------------------------------------------------

#endif // TEST