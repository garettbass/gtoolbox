#if TEST
#include <catch.h>
#include "./unicode.h"

TEST_CASE("UTF-16 head surrogates are recognized", "[unicode]")
{
    using namespace xtd;

    REQUIRE(is_surrogate_head(0xD800));
    REQUIRE(is_surrogate_head(0xDBFF));

    REQUIRE(not is_surrogate_head(0x0000));
    REQUIRE(not is_surrogate_head(0xD7FF));
    REQUIRE(not is_surrogate_head(0xE000));
    REQUIRE(not is_surrogate_head(0xFFFF));
}

//------------------------------------------------------------------------------

TEST_CASE("UTF-16 tail surrogates are recognized", "[unicode][UTF-16]")
{
    using namespace xtd;

    REQUIRE(is_surrogate_tail(0xDC00));
    REQUIRE(is_surrogate_tail(0xDFFF));

    REQUIRE(not is_surrogate_tail(0x0000));
    REQUIRE(not is_surrogate_tail(0xD7FF));
    REQUIRE(not is_surrogate_tail(0xE000));
    REQUIRE(not is_surrogate_tail(0xFFFF));
}

//------------------------------------------------------------------------------

TEST_CASE("UTF-32 codepoints are recognized", "[unicode][UTF-32]")
{
    using namespace xtd;

//    REQUIRE(is_valid_utf32(0x0000));
//    REQUIRE(is_valid_utf32(0xD7FF));
//    REQUIRE(is_valid_utf32(0xE000));
//    REQUIRE(is_valid_utf32(0x10FFFF));
//
//    REQUIRE(not is_valid_utf32(0xD800));
//    REQUIRE(not is_valid_utf32(0xDFFF));
//    REQUIRE(not is_valid_utf32(0xFFFFFFFF));
}

//------------------------------------------------------------------------------

TEST_CASE("UTF-16 surrogate pairs are transcoded to UTF-32", "[unicode]")
{
    using namespace xtd;

//    REQUIRE(to_codepoint(0xD800, 0xDC00) == 0x10000);
//    REQUIRE(to_codepoint(0xDBFF, 0xDFFF) == 0x10FFFF);
//
//    REQUIRE(to_codepoint(0xD7FF, 0xDC00) == 0xFFFFFFFF);
//    REQUIRE(to_codepoint(0xD800, 0xDBFF) == 0xFFFFFFFF);
}

#endif // TEST