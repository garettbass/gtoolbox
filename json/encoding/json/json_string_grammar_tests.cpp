#if TEST
#include <catch.h>
#include <zna/source/thirdParty/gtoolbox/peg/utility.h>
#include "./json_string_grammar.h"

//------------------------------------------------------------------------------

TEST_CASE("json::json_string_grammar tests", "[cfg][JSON]")
{
    using namespace json;
    using namespace peg;

    SECTION("empty string is recognized")
    {
        auto expr = parse<json_string_grammar::string>("\"\"");

        REQUIRE(expr.offset() == 0);
        REQUIRE(expr.size()   == 2);
    }

    SECTION("simple string is recognized")
    {
        auto expr = parse<json_string_grammar::string>("\"hello\"");

        REQUIRE(expr.offset() == 0);
        REQUIRE(expr.size()   == 7);
    }

    SECTION("unterminated string produces error")
    {
        auto expr =
            find_first<json_string_grammar::error_invalid_string_character>
            ::in<json_string_grammar::string>("\"hello");

        REQUIRE(expr.offset() == 6);
        REQUIRE(expr.size()   == 0);
    }
}

//------------------------------------------------------------------------------

#endif // TEST
