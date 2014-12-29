#if TEST
#include <catch.h>
#include "./utf16_codepoint.h"

TEST_CASE("xtd::utf16_codepoint tests", "[unicode]")
{
    using namespace xtd;

    utf16_codepoint utf16;

    REQUIRE(utf16.size() == 0);
    REQUIRE(utf16[0] == 0);
    REQUIRE(not utf16.is_valid());

    SECTION("valid UTF-32 codepoints are recognized")
    {
        REQUIRE(utf16(char32_t(0)));
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0);
        REQUIRE(utf16[1] == 0);
        REQUIRE(utf16.is_valid());

        REQUIRE(utf16(char32_t(0x6C34)));
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0x6C34);
        REQUIRE(utf16[1] == 0);
        REQUIRE(utf16.is_valid());

        REQUIRE(utf16(char32_t(0x10000)));
        REQUIRE(utf16.size() == 2);
        REQUIRE(utf16[0] == 0xD800);
        REQUIRE(utf16[1] == 0xDC00);
        REQUIRE(utf16[2] == 0);
        REQUIRE(utf16.is_valid());

        REQUIRE(utf16(char32_t(0x10FFFF)));
        REQUIRE(utf16.size() == 2);
        REQUIRE(utf16[0] == 0xDBFF);
        REQUIRE(utf16[1] == 0xDFFF);
        REQUIRE(utf16[2] == 0);
        REQUIRE(utf16.is_valid());
    }

    SECTION("invalid UTF-32 codepoints are recognized")
    {
        REQUIRE(utf16(char32_t(0xDC00)));
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xDC00);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(utf16(char32_t(0xDFFF)));
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xDFFF);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(utf16(char32_t(0xD800)));
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xD800);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(utf16(char32_t(0xDBFF)));
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xDBFF);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(not utf16(char32_t(0x110000)));
        REQUIRE(utf16.size() == 0);
        REQUIRE(utf16[0] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(not utf16(char32_t(0xFFFFFFFF)));
        REQUIRE(utf16.size() == 0);
        REQUIRE(utf16[0] == 0);
        REQUIRE(not utf16.is_valid());
    }

    SECTION("valid UTF-32 sequences are recognized")
    {
        const char32_t s[] =
            U"\x0000"
            U"\x6C34"
            U"\x10000"
            U"\x10FFFF";

        const char32_t* p = s;

        REQUIRE(utf16(p));
        REQUIRE(p == &s[1]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0);
        REQUIRE(utf16[1] == 0);
        REQUIRE(utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[2]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0x6C34);
        REQUIRE(utf16[1] == 0);
        REQUIRE(utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[3]);
        REQUIRE(utf16.size() == 2);
        REQUIRE(utf16[0] == 0xD800);
        REQUIRE(utf16[1] == 0xDC00);
        REQUIRE(utf16[2] == 0);
        REQUIRE(utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[4]);
        REQUIRE(utf16.size() == 2);
        REQUIRE(utf16[0] == 0xDBFF);
        REQUIRE(utf16[1] == 0xDFFF);
        REQUIRE(utf16[2] == 0);
        REQUIRE(utf16.is_valid());
    }

    SECTION("invalid UTF-32 sequences are recognized")
    {
        const char32_t s[] =
            U"\xDC00"
            U"\xDFFF"
            U"\xD800"
            U"\xDBFF"
            U"\x110000"
            U"\xFFFFFFFF";

        const char32_t* p = s;

        REQUIRE(utf16(p));
        REQUIRE(p == &s[1]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xDC00);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[2]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xDFFF);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[3]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xD800);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[4]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xDBFF);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        // 0x110000

        REQUIRE(not utf16(p));
        REQUIRE(p == &s[4]);
        REQUIRE(utf16.size() == 0);
        REQUIRE(utf16[0] == 0);
        REQUIRE(not utf16.is_valid());

        ++p; // 0xFFFFFFFF

        REQUIRE(not utf16(p));
        REQUIRE(p == &s[5]);
        REQUIRE(utf16.size() == 0);
        REQUIRE(utf16[0] == 0);
        REQUIRE(not utf16.is_valid());
    }

    SECTION("valid UTF-16 sequences are recognized")
    {
        const char16_t s[] =
            u"\x0000"
            u"\x6C34"
            u"\xD800\xDC00"
            u"\xDBFF\xDFFF";

        const char16_t* p = s;

        REQUIRE(utf16(p));
        REQUIRE(p == &s[1]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0);
        REQUIRE(utf16[1] == 0);
        REQUIRE(utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[2]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0x6C34);
        REQUIRE(utf16[1] == 0);
        REQUIRE(utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[4]);
        REQUIRE(utf16.size() == 2);
        REQUIRE(utf16[0] == 0xD800);
        REQUIRE(utf16[1] == 0xDC00);
        REQUIRE(utf16[2] == 0);
        REQUIRE(utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[6]);
        REQUIRE(utf16.size() == 2);
        REQUIRE(utf16[0] == 0xDBFF);
        REQUIRE(utf16[1] == 0xDFFF);
        REQUIRE(utf16[2] == 0);
        REQUIRE(utf16.is_valid());
    }

    SECTION("invalid UTF-16 sequences are recognized")
    {
        const char16_t s[] =
            u"\xDC00"
            u"\xDFFF"
            u"\xD800"
            u"\xDBFF";

        const char16_t* p = s;

        REQUIRE(utf16(p));
        REQUIRE(p == &s[1]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xDC00);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[2]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xDFFF);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[3]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xD800);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[4]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xDBFF);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());
    }

    SECTION("valid UTF-8 sequences are recognized")
    {
        const char s[] =
            "\x00"
            "\xE6\xB0\xB4"
            "\xF0\x90\x80\x80"
            "\xF4\x8F\xBF\xBF";

        const char* p = s;

        REQUIRE(utf16(p));
        REQUIRE(p == &s[1]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0);
        REQUIRE(utf16[1] == 0);
        REQUIRE(utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[4]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0x6C34);
        REQUIRE(utf16[1] == 0);
        REQUIRE(utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[8]);
        REQUIRE(utf16.size() == 2);
        REQUIRE(utf16[0] == 0xD800);
        REQUIRE(utf16[1] == 0xDC00);
        REQUIRE(utf16[2] == 0);
        REQUIRE(utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[12]);
        REQUIRE(utf16.size() == 2);
        REQUIRE(utf16[0] == 0xDBFF);
        REQUIRE(utf16[1] == 0xDFFF);
        REQUIRE(utf16[2] == 0);
        REQUIRE(utf16.is_valid());
    }

    SECTION("invalid UTF-8 sequences are recognized")
    {
        const char s[] =
            "\xED\xB0\x80"  // 0xDC00
            "\xED\xBF\xBF"  // 0xDFFF
            "\xED\xA0\x80"  // 0xD800
            "\xED\xAF\xBF"; // 0xDBFF

        const char* p = s;

        REQUIRE(utf16(p));
        REQUIRE(p == &s[3]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xDC00);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[6]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xDFFF);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[9]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xD800);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());

        REQUIRE(utf16(p));
        REQUIRE(p == &s[12]);
        REQUIRE(utf16.size() == 1);
        REQUIRE(utf16[0] == 0xDBFF);
        REQUIRE(utf16[1] == 0);
        REQUIRE(not utf16.is_valid());
    }
}

#endif // TEST