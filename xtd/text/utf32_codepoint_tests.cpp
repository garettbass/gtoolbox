#if TEST
#include <catch.h>
#include "utf32_codepoint.h"

TEST_CASE("xtd::utf32_codepoint tests", "[unicode]")
{
    using namespace xtd;

    utf32_codepoint utf32;

    REQUIRE(utf32.size() == 0);
    REQUIRE(utf32[0] == 0);
    REQUIRE(not utf32.is_valid());

    SECTION("valid UTF-32 codepoints are recognized")
    {
        REQUIRE(utf32(char32_t(0)));
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0);
        REQUIRE(utf32[1] == 0);
        REQUIRE(utf32.is_valid());

        REQUIRE(utf32(char32_t(0x6C34)));
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0x6C34);
        REQUIRE(utf32[1] == 0);
        REQUIRE(utf32.is_valid());

        REQUIRE(utf32(char32_t(0x10000)));
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0x10000);
        REQUIRE(utf32[1] == 0);
        REQUIRE(utf32.is_valid());

        REQUIRE(utf32(char32_t(0x10FFFF)));
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0x10FFFF);
        REQUIRE(utf32[1] == 0);
        REQUIRE(utf32.is_valid());
    }

    SECTION("invalid UTF-32 codepoints are tolerated")
    {
        REQUIRE(utf32(char32_t(0xDC00)));
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xDC00);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());

        REQUIRE(utf32(char32_t(0xDFFF)));
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xDFFF);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());

        REQUIRE(utf32(char32_t(0xD800)));
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xD800);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());

        REQUIRE(utf32(char32_t(0xDBFF)));
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xDBFF);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());

        REQUIRE(utf32(char32_t(0x110000)));
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0x110000);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());

        REQUIRE(utf32(char32_t(0xFFFFFFFF)));
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xFFFFFFFF);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());
    }

    SECTION("valid UTF-16 sequences are recognized")
    {
        const char16_t s[] =
            u"\x0000"
            u"\x6C34"
            u"\xD800\xDC00"
            u"\xDBFF\xDFFF";

        const char16_t* p = s;

        REQUIRE(utf32(p));
        REQUIRE(p == &s[1]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0);
        REQUIRE(utf32[1] == 0);
        REQUIRE(utf32.is_valid());

        REQUIRE(utf32(p));
        REQUIRE(p == &s[2]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0x6C34);
        REQUIRE(utf32[1] == 0);
        REQUIRE(utf32.is_valid());

        REQUIRE(utf32(p));
        REQUIRE(p == &s[4]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0x10000);
        REQUIRE(utf32[1] == 0);
        REQUIRE(utf32.is_valid());

        REQUIRE(utf32(p));
        REQUIRE(p == &s[6]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0x10FFFF);
        REQUIRE(utf32[1] == 0);
        REQUIRE(utf32.is_valid());
    }

    SECTION("invalid UTF-16 sequences are tolerated")
    {
        const char16_t s[] =
            u"\xDC00"
            u"\xDFFF"
            u"\xD800"
            u"\xDBFF";

        const char16_t* p = s;

        REQUIRE(utf32(p));
        REQUIRE(p == &s[1]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xDC00);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());

        REQUIRE(utf32(p));
        REQUIRE(p == &s[2]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xDFFF);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());

        REQUIRE(utf32(p));
        REQUIRE(p == &s[3]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xD800);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());

        REQUIRE(utf32(p));
        REQUIRE(p == &s[4]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xDBFF);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());
    }

    SECTION("valid UTF-8 sequences are recognized")
    {
        const char s[] =
            "\x00"
            "\xE6\xB0\xB4"      // 0x6C34
            "\xF0\x90\x80\x80"  // 0x10000
            "\xF4\x8F\xBF\xBF"; // 0x10FFFF

        const char* p = s;

        REQUIRE(utf32(p));
        REQUIRE(p == &s[1]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0);
        REQUIRE(utf32[1] == 0);
        REQUIRE(utf32.is_valid());

        REQUIRE(utf32(p));
        REQUIRE(p == &s[4]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0x6C34);
        REQUIRE(utf32[1] == 0);
        REQUIRE(utf32.is_valid());

        REQUIRE(utf32(p));
        REQUIRE(p == &s[8]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0x10000);
        REQUIRE(utf32[1] == 0);
        REQUIRE(utf32.is_valid());

        REQUIRE(utf32(p));
        REQUIRE(p == &s[12]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0x10FFFF);
        REQUIRE(utf32[1] == 0);
        REQUIRE(utf32.is_valid());
    }

    SECTION("invalid UTF-8 sequences are tolerated")
    {
        const char s[] =
            "\xED\xB0\x80"  // 0xDC00
            "\xED\xBF\xBF"  // 0xDFFF
            "\xED\xA0\x80"  // 0xD800
            "\xED\xAF\xBF"; // 0xDBFF

        const char* p = s;

        REQUIRE(utf32(p));
        REQUIRE(p == &s[3]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xDC00);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());

        REQUIRE(utf32(p));
        REQUIRE(p == &s[6]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xDFFF);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());

        REQUIRE(utf32(p));
        REQUIRE(p == &s[9]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xD800);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());

        REQUIRE(utf32(p));
        REQUIRE(p == &s[12]);
        REQUIRE(utf32.size() == 1);
        REQUIRE(utf32[0] == 0xDBFF);
        REQUIRE(utf32[1] == 0);
        REQUIRE(not utf32.is_valid());
    }
}

#endif // TEST