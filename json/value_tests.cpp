#if TEST
#define CATCH_CONFIG_SFINAE
#include <catch.h>
#include "./value.h"

TEST_CASE("cfg", "[cfg]")
{
    json::value true_value = true;

    REQUIRE((json::boolean*)true_value);
    REQUIRE(true_value == true);

    json::value false_value = false;

    REQUIRE((json::boolean*)false_value);
    REQUIRE(false_value == false);

    json::value integer_value = 1;

    REQUIRE((json::number*)integer_value);
    REQUIRE(integer_value == 1);

    json::value double_value = 1.0;

    REQUIRE((json::number*)double_value);
    REQUIRE(double_value == 1);

    json::value array_value = json::array { 1, 2, 3 };

    REQUIRE((json::array*)array_value);

    json::array& array = array_value;

    REQUIRE(array.size() == 3);
    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 2);
    REQUIRE(array[2] == 3);

    json::value object_value = json::object {
        {"one",   1},
        {"two",   2},
        {"three", 3},
    };

    REQUIRE((json::object*)object_value);

    json::object& object = object_value;

    REQUIRE(object.size()   == 3);
    REQUIRE(object["one"]   == 1);
    REQUIRE(object["two"]   == 2);
    REQUIRE(object["three"] == 3);

    json::value string_value = "hello";

    REQUIRE((json::string*)string_value);
    REQUIRE(string_value == "hello");
}

#endif // TEST
