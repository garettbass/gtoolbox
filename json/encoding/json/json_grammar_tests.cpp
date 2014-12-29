#if TEST
#include <catch.h>
#include <zna/source/thirdParty/gtoolbox/peg/utility.h>
#include <zna/source/thirdParty/gtoolbox/xtd/filesystem/path.h>
#include "./json_grammar.h"

TEST_CASE("json::json_grammar tests", "[cfg][JSON]")
{
    using namespace json;
    using namespace peg;

    using json = json_grammar;

    REQUIRE(is_match<json::string>(R"("")"));
    REQUIRE(is_match<json::string>(R"("h")"));
    REQUIRE(is_match<json::string>(R"("hello")"));
    REQUIRE(is_match<json::string>(R"("hello \"world\"")"));
    //REQUIRE((has_error<json::string, json::error_incomplete_string>(R"("hello)")));
    REQUIRE(not is_match<json::string>("123"));
    
    REQUIRE(is_match<json::number>("123"));
    REQUIRE(is_match<json::number>("123.0"));
    REQUIRE(is_match<json::number>("123e+0"));
    REQUIRE(is_match<json::number>("-123"));
    REQUIRE(is_match<json::number>("-123.0"));
    REQUIRE(is_match<json::number>("-123e+0"));
    REQUIRE(not is_match<json::number>(R"("hello")"));
    
    REQUIRE(is_match<json::value>("false"));
    REQUIRE(is_match<json::value>("true"));
    REQUIRE(is_match<json::value>("null"));
    REQUIRE(is_match<json::value>("123"));
    REQUIRE(is_match<json::value>("123.0"));
    REQUIRE(is_match<json::value>("123e+0"));
    REQUIRE(is_match<json::value>("-123"));
    REQUIRE(is_match<json::value>("-123.0"));
    REQUIRE(is_match<json::value>("-123e+0"));
    REQUIRE(is_match<json::value>(R"("hello")"));
    
    REQUIRE(is_match<json::value>(R"([])"));
    REQUIRE(is_match<json::value>(R"(["hello","world"])"));
    REQUIRE(is_match<json::value>(R"([ "hello" , "world" ])"));
    REQUIRE(is_match<json::value>(R"([ false, true, null, 1, "hello" ])"));
    
//    REQUIRE((has_error<json::array, json::error_trailing_comma>("[,]")));
//    REQUIRE((has_error<json::array, json::error_trailing_comma>(R"([
//        false, true, null, 1, "hello",
//    ])")));
    
    REQUIRE(is_match<json::value>(R"({})"));
    REQUIRE(is_match<json::value>(R"({"hello":"world"})"));
    REQUIRE(is_match<json::value>(R"({ "hello" : "world" })"));

    const char json_array[] = R"(
    [ false, true, null, 1, 2.0, 3e0, 4.0e0, "hello" ]
    )";

    REQUIRE(is_match<json::document>(json_array));

    const char json_object[] = R"(
    {
        "null"   : null,
        "false"  : false,
        "true"   : true,
        "one"    : 1,
        "two"    : 2.0,
        "three"  : 3e0,
        "four"   : 4.0e0,
        "string" : "hello \"world\"\\\b\f\n\r\/\t",
        "array"  : [ false, true, null, 1, 2.0, 3e0, 4.0e0, "hello" ],
        "object" : { "false" : false, "true" : true, "one" : 1 }
    }
    )";

    REQUIRE(is_match<json::document>(json_object));
}

#endif // TEST