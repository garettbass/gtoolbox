#if TEST
#include <catch.h>
#include "./utf8_codepoint.h"

TEST_CASE("xtd::utf8_codepoint tests", "[unicode]")
{
    using namespace xtd;

    utf8_codepoint utf8;

    REQUIRE(utf8.size() == 0);
    REQUIRE(utf8[0] == 0);
    REQUIRE(not utf8.is_valid());

    SECTION("valid UTF-32 codepoints are recognized")
    {
        REQUIRE(utf8(char32_t(0)));
        REQUIRE(utf8.size() == 1);
        REQUIRE(utf8[0] == 0);
        REQUIRE(utf8[1] == 0);
        REQUIRE(utf8.is_valid());

        REQUIRE(utf8(char32_t(0x6C34))); // 0xE6 0xB0 0xB4
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xE6));
        REQUIRE(utf8[1] == char(0xB0));
        REQUIRE(utf8[2] == char(0xB4));
        REQUIRE(utf8[3] == 0);
        REQUIRE(utf8.is_valid());

        REQUIRE(utf8(char32_t(0x10000))); // 0xF0 0x90 0x80 0x80
        REQUIRE(utf8.size() == 4);
        REQUIRE(utf8[0] == char(0xF0));
        REQUIRE(utf8[1] == char(0x90));
        REQUIRE(utf8[2] == char(0x80));
        REQUIRE(utf8[3] == char(0x80));
        REQUIRE(utf8[4] == 0);
        REQUIRE(utf8.is_valid());

        REQUIRE(utf8(char32_t(0x10FFFF))); // 0xF4 0x8F 0xBF 0xBF
        REQUIRE(utf8.size() == 4);
        REQUIRE(utf8[0] == char(0xF4));
        REQUIRE(utf8[1] == char(0x8F));
        REQUIRE(utf8[2] == char(0xBF));
        REQUIRE(utf8[3] == char(0xBF));
        REQUIRE(utf8[4] == 0);
        REQUIRE(utf8.is_valid());
    }

    SECTION("invalid UTF-32 codepoints are recognized")
    {
        REQUIRE(utf8(char32_t(0xDC00))); // 0xED 0xB0 0x80
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xED));
        REQUIRE(utf8[1] == char(0xB0));
        REQUIRE(utf8[2] == char(0x80));
        REQUIRE(utf8[3] == 0);
        REQUIRE(not utf8.is_valid());

        REQUIRE(utf8(char32_t(0xDFFF))); // 0xED 0xBF 0xBF
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xED));
        REQUIRE(utf8[1] == char(0xBF));
        REQUIRE(utf8[2] == char(0xBF));
        REQUIRE(utf8[3] == 0);
        REQUIRE(not utf8.is_valid());

        REQUIRE(utf8(char32_t(0xD800))); // 0xED 0xA0 0x80
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xED));
        REQUIRE(utf8[1] == char(0xA0));
        REQUIRE(utf8[2] == char(0x80));
        REQUIRE(utf8[3] == 0);
        REQUIRE(not utf8.is_valid());

        REQUIRE(utf8(char32_t(0xDBFF))); // 0xED 0xAF 0xBF
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xED));
        REQUIRE(utf8[1] == char(0xAF));
        REQUIRE(utf8[2] == char(0xBF));
        REQUIRE(utf8[3] == 0);
        REQUIRE(not utf8.is_valid());

        REQUIRE(not utf8(char32_t(0x110000)));
        REQUIRE(utf8.size() == 0);
        REQUIRE(utf8[0] == 0);
        REQUIRE(not utf8.is_valid());

        REQUIRE(not utf8(char32_t(0xFFFFFFFF)));
        REQUIRE(utf8.size() == 0);
        REQUIRE(utf8[0] == 0);
        REQUIRE(not utf8.is_valid());
    }

    SECTION("valid UTF-16 sequences are recognized")
    {
        const char16_t s[] =
            u"\x0000"
            u"\x6C34"
            u"\xD800\xDC00"
            u"\xDBFF\xDFFF";

        const char16_t* p = s;

        REQUIRE(utf8(p));
        REQUIRE(p == &s[1]);
        REQUIRE(utf8.size() == 1);
        REQUIRE(utf8[0] == 0);
        REQUIRE(utf8[1] == 0);
        REQUIRE(utf8.is_valid());

        REQUIRE(utf8(p));
        REQUIRE(p == &s[2]);
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xE6));
        REQUIRE(utf8[1] == char(0xB0));
        REQUIRE(utf8[2] == char(0xB4));
        REQUIRE(utf8[3] == 0);
        REQUIRE(utf8.is_valid());

        REQUIRE(utf8(p));
        REQUIRE(p == &s[4]);
        REQUIRE(utf8.size() == 4);
        REQUIRE(utf8[0] == char(0xF0));
        REQUIRE(utf8[1] == char(0x90));
        REQUIRE(utf8[2] == char(0x80));
        REQUIRE(utf8[3] == char(0x80));
        REQUIRE(utf8[4] == 0);
        REQUIRE(utf8.is_valid());

        REQUIRE(utf8(p));
        REQUIRE(p == &s[6]);
        REQUIRE(utf8.size() == 4);
        REQUIRE(utf8[0] == char(0xF4));
        REQUIRE(utf8[1] == char(0x8F));
        REQUIRE(utf8[2] == char(0xBF));
        REQUIRE(utf8[3] == char(0xBF));
        REQUIRE(utf8[4] == 0);
        REQUIRE(utf8.is_valid());
    }

    SECTION("invalid UTF-16 sequences are recognized")
    {
        const char16_t s[] =
            u"\xDC00"  // 0xED 0xB0 0x80
            u"\xDFFF"  // 0xED 0xBF 0xBF
            u"\xD800"  // 0xED 0xA0 0x80
            u"\xDBFF"; // 0xED 0xAF 0xBF

        const char16_t* p = s;

        REQUIRE(utf8(p));
        REQUIRE(p == &s[1]);
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xED));
        REQUIRE(utf8[1] == char(0xB0));
        REQUIRE(utf8[2] == char(0x80));
        REQUIRE(utf8[3] == 0);
        REQUIRE(not utf8.is_valid());

        REQUIRE(utf8(p));
        REQUIRE(p == &s[2]);
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xED));
        REQUIRE(utf8[1] == char(0xBF));
        REQUIRE(utf8[2] == char(0xBF));
        REQUIRE(utf8[3] == 0);
        REQUIRE(not utf8.is_valid());

        REQUIRE(utf8(p));
        REQUIRE(p == &s[3]);
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xED));
        REQUIRE(utf8[1] == char(0xA0));
        REQUIRE(utf8[2] == char(0x80));
        REQUIRE(utf8[3] == 0);
        REQUIRE(not utf8.is_valid());

        REQUIRE(utf8(p));
        REQUIRE(p == &s[4]);
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xED));
        REQUIRE(utf8[1] == char(0xAF));
        REQUIRE(utf8[2] == char(0xBF));
        REQUIRE(utf8[3] == 0);
        REQUIRE(not utf8.is_valid());
    }

    SECTION("valid UTF-8 sequences are recognized")
    {
        const char s[] =
            "\x00"
            "\xE6\xB0\xB4"
            "\xF0\x90\x80\x80"
            "\xF4\x8F\xBF\xBF";

        const char* p = s;

        REQUIRE(utf8(p));
        REQUIRE(p == &s[1]);
        REQUIRE(utf8.size() == 1);
        REQUIRE(utf8[0] == 0);
        REQUIRE(utf8[1] == 0);
        REQUIRE(utf8.is_valid());

        REQUIRE(utf8(p));
        REQUIRE(p == &s[4]);
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xE6));
        REQUIRE(utf8[1] == char(0xB0));
        REQUIRE(utf8[2] == char(0xB4));
        REQUIRE(utf8[3] == 0);
        REQUIRE(utf8.is_valid());

        REQUIRE(utf8(p));
        REQUIRE(p == &s[8]);
        REQUIRE(utf8.size() == 4);
        REQUIRE(utf8[0] == char(0xF0));
        REQUIRE(utf8[1] == char(0x90));
        REQUIRE(utf8[2] == char(0x80));
        REQUIRE(utf8[3] == char(0x80));
        REQUIRE(utf8[4] == 0);
        REQUIRE(utf8.is_valid());

        REQUIRE(utf8(p));
        REQUIRE(p == &s[12]);
        REQUIRE(utf8.size() == 4);
        REQUIRE(utf8[0] == char(0xF4));
        REQUIRE(utf8[1] == char(0x8F));
        REQUIRE(utf8[2] == char(0xBF));
        REQUIRE(utf8[3] == char(0xBF));
        REQUIRE(utf8[4] == 0);
        REQUIRE(utf8.is_valid());
    }

    SECTION("invalid UTF-8 sequences are recognized")
    {
        const char s[] =
            "\xED\xB0\x80"
            "\xED\xBF\xBF"
            "\xED\xA0\x80"
            "\xED\xAF\xBF";

        const char* p = s;

        REQUIRE(utf8(p));
        REQUIRE(p == &s[3]);
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xED));
        REQUIRE(utf8[1] == char(0xB0));
        REQUIRE(utf8[2] == char(0x80));
        REQUIRE(utf8[3] == 0);
        REQUIRE(not utf8.is_valid());

        REQUIRE(utf8(p));
        REQUIRE(p == &s[6]);
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xED));
        REQUIRE(utf8[1] == char(0xBF));
        REQUIRE(utf8[2] == char(0xBF));
        REQUIRE(utf8[3] == 0);
        REQUIRE(not utf8.is_valid());

        REQUIRE(utf8(p));
        REQUIRE(p == &s[9]);
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xED));
        REQUIRE(utf8[1] == char(0xA0));
        REQUIRE(utf8[2] == char(0x80));
        REQUIRE(utf8[3] == 0);
        REQUIRE(not utf8.is_valid());

        REQUIRE(utf8(p));
        REQUIRE(p == &s[12]);
        REQUIRE(utf8.size() == 3);
        REQUIRE(utf8[0] == char(0xED));
        REQUIRE(utf8[1] == char(0xAF));
        REQUIRE(utf8[2] == char(0xBF));
        REQUIRE(utf8[3] == 0);
        REQUIRE(not utf8.is_valid());
    }
}

#endif // TEST