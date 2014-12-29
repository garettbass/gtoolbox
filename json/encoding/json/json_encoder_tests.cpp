#if TEST
#include <sstream>
#include <catch.h>
#include "./json_encoder.h"

TEST_CASE("json::json_encoder tests", "[cfg][JSON]")
{
    using namespace json;

    std::stringstream s;

    json_encoder encoder(s);

    encoder.object_enter();

        encoder.property_enter("null");
            encoder.encode_null();
        encoder.property_leave();

        encoder.property_enter("false");
            encoder.encode_boolean(false);
        encoder.property_leave();

        encoder.property_enter("true");
            encoder.encode_boolean(true);
        encoder.property_leave();

        encoder.property_enter("one");
            encoder.encode_number(1);
        encoder.property_leave();

        encoder.property_enter("string");
            encoder.encode_string("hello \"world\"\\\b\f\n\r\t");
        encoder.property_leave();

        encoder.property_enter("array");
            encoder.array_enter();
                encoder.encode_null();
                encoder.encode_boolean(false);
                encoder.encode_boolean(true);
                encoder.encode_number(1);
                encoder.encode_string("hello");
            encoder.array_leave();
        encoder.property_leave();

    encoder.object_leave();

    const char expected_output[] =
R"({
    "null": null,
    "false": false,
    "true": true,
    "one": 1,
    "string": "hello \"world\"\\\b\f\n\r\t",
    "array": [
        null,
        false,
        true,
        1,
        "hello"
    ]
})";

    REQUIRE(s.str() == expected_output);

    std::cout << std::endl;
}

#endif // TEST