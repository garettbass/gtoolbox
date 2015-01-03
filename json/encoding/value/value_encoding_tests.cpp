#if TEST
#define CATCH_CONFIG_SFINAE
#include <catch.h>
#include <sstream>
#include <gtoolbox/xtd/echo.h>
#include "../debug/debug_encoder.h"
#include "../json/json_decoder.h"
#include "../json/json_encoder.h"
#include "./value_decoder.h"
#include "./value_encoder.h"

TEST_CASE("json::value_encoder tests", "[cfg][JSON]")
{
    using namespace json;

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
        "array"  : [ null, false, true, 1, 2.0, 3e0, 4.0e0, "hello" ],
        "object" : {
            "false" : false,
            "true"  : true,
            "one"   : 1
        }
    }
    )";

    value v1, v2;

    {
        value_encoder encoder(v1);

        //debug_encoder encoder;

        json_decoder(json_object).decode(encoder);
    }

    REQUIRE(v1.type() == type::object);

    object& o1 = v1;

    REQUIRE(o1["null"].type() == type::null);
    REQUIRE(o1["null"] == nullptr);

    REQUIRE(o1["false"].type() == type::boolean);
    REQUIRE(o1["false"] == false);
    REQUIRE(o1["false"] != true);

    REQUIRE(o1["true"].type() == type::boolean);
    REQUIRE(o1["true"] == true);
    REQUIRE(o1["true"] != false);

    REQUIRE(o1["one"].type() == type::number);
    REQUIRE(o1["one"] == 1);
    REQUIRE(o1["one"] != 2);

    REQUIRE(o1["two"].type() == type::number);
    REQUIRE(o1["two"] == 2);
    REQUIRE(o1["two"] != 3);

    REQUIRE(o1["three"].type() == type::number);
    REQUIRE(o1["three"] == 3);
    REQUIRE(o1["three"] != 4);

    REQUIRE(o1["four"].type() == type::number);
    REQUIRE(o1["four"] == 4);
    REQUIRE(o1["four"] != 5);

    REQUIRE(o1["string"].type() == type::string);
    REQUIRE(o1["string"] == "hello \"world\"\\\b\f\n\r/\t");
    REQUIRE(o1["string"] != "");

    REQUIRE(o1["array"].type() == type::array);

    array& a1 = o1["array"];

    REQUIRE(a1.size() == 8);
    REQUIRE(a1[0] == nullptr);
    REQUIRE(a1[1] == false);
    REQUIRE(a1[2] == true);
    REQUIRE(a1[3] == 1);
    REQUIRE(a1[4] == 2.0);
    REQUIRE(a1[5] == 3e0);
    REQUIRE(a1[6] == 4.0e0);
    REQUIRE(a1[7] == "hello");

    REQUIRE(o1["object"].type()   == type::object);

    object& o1o = o1["object"];

    REQUIRE(o1o.size()   == 3);
    REQUIRE(o1o["false"] == false);
    REQUIRE(o1o["true"]  == true);
    REQUIRE(o1o["one"]   == 1);

    {
        std::stringstream s;

        value_decoder(v1).decode_to<json_encoder>(s);

        json_decoder(s).decode_to<value_encoder>(v2);
    }

    std::cout << v1 << std::endl;

    std::cout << v2 << std::endl;

    REQUIRE(v1 == v2);
}

#endif // TEST
