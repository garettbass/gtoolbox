#pragma once
#include <cstddef>
#include <cstdint>

namespace xtd {

    size_t codepoint_size(const char16_t*);
    size_t codepoint_size(const char*);

    bool is_surrogate_head(const char16_t);
    bool is_surrogate_tail(const char16_t);

    bool is_valid_codepoint(const char32_t);
    bool is_valid_codepoint(const char16_t*);
    bool is_valid_codepoint(const char*);

    size_t to_codepoint(char32_t&, const char16_t*);
    size_t to_codepoint(char32_t&, const char*);

    //--------------------------------------------------------------------------

    inline bool is_surrogate_head(const char16_t c)
    {
        return ((c & 0xFC00) == 0xD800);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline bool is_surrogate_tail(const char16_t c)
    {
        return ((c & 0xFC00) == 0xDC00);
    }

    //--------------------------------------------------------------------------

    inline bool is_valid_codepoint(const char32_t u)
    {
        return (u <= 0x10FFFF) & ((u < 0xD800) | (u > 0xDFFF));
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline bool is_valid_codepoint(const char16_t* p)
    {
        char32_t c;

        const size_t size = to_codepoint(c, p);

        const bool _is_legible_utf16 = (size > 0);

        const bool _is_valid_utf32 = is_valid_codepoint(c);

        return _is_legible_utf16 & _is_valid_utf32;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline bool is_valid_codepoint(const char* p)
    {
        char32_t c;

        const size_t size = to_codepoint(c, p);

        const bool is_legible_utf8 = (size > 0);

        const bool is_valid_utf32 = is_valid_codepoint(c);

        return is_legible_utf8 & is_valid_utf32;
    }

    //--------------------------------------------------------------------------

    inline size_t to_codepoint(char32_t& u, const char16_t* p)
    {
        char16_t c0 = p[0];
        char16_t c1 = p[1];

        const bool is_c0_head = is_surrogate_head(c0);
        const bool is_c1_tail = is_surrogate_tail(c1);

        const bool is_size_2 = (is_c0_head & is_c1_tail);
        const bool is_size_1 = not is_size_2;

        const size_t size =
            (is_size_1 * 1) +
            (is_size_2 * 2);

        const char32_t uuuuuyyyyyy = ((c0 & 0x03FF) + 0x40);
        const char32_t  xxxxxxxxxx = ((c1 & 0x03FF));

        const char32_t u1 = (is_size_1 * c0) & 0xFFFF;
        const char32_t u2 = (is_size_2 * ((uuuuuyyyyyy << 10) + xxxxxxxxxx));

        u = u1 + u2;

        return size;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline size_t to_codepoint(char32_t& u, const char* p)
    {
        const char c0 = p[0];
        const char c1 = p[1];
        const char c2 = p[2];
        const char c3 = p[3];

        const bool is_size_1 = ((c0 & 0x80) == 0x00); // 0xxx xxxx
        const bool is_size_2 = ((c0 & 0xE0) == 0xC0); // 110x xxxx
        const bool is_size_3 = ((c0 & 0xF0) == 0xE0); // 1110 xxxx
        const bool is_size_4 = ((c0 & 0xF8) == 0xF0); // 1111 0xxx

        const size_t size =
            (is_size_1 * 1) +
            (is_size_2 * 2) +
            (is_size_3 * 3) +
            (is_size_4 * 4);

        const char32_t u1 =
            (is_size_1 * c0);

        const char32_t u2 =
            (is_size_2 * (
                (uint32_t(c0 & 0x1F) << 6) +
                (uint32_t(c1 & 0x3F))));

        const char32_t u3 =
            (is_size_3 * (
                (uint32_t(c0 & 0x0F) << 12) +
                (uint32_t(c1 & 0x3F) <<  6) +
                (uint32_t(c2 & 0x3F))));

        const char32_t u4 =
            (is_size_4 * (
                (uint32_t(c0 & 0x07) << 18) +
                (uint32_t(c1 & 0x3F) << 12) +
                (uint32_t(c2 & 0x3F) <<  6) +
                (uint32_t(c3 & 0x3F))));

        u = u1 + u2 + u3 + u4;

        return size;
    }

    //--------------------------------------------------------------------------

} // namespace xtd