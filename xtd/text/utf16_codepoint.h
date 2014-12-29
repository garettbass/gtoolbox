#pragma once
#include "./unicode.h"

namespace xtd {

    class utf16_codepoint
    {
        char16_t _data[4] = { 0, 0, 0, 0 };

        size_t _size = 0;

    public: // structors

        utf16_codepoint() = default;

    public: // operators

        bool operator () (const char32_t);
        bool operator () (const char32_t*&);
        bool operator () (const char16_t*&);
        bool operator () (const char*&);

        const char16_t& operator [] (size_t i) const { return _data[i]; }

    public: // properties

        const char16_t* data() const { return _data; }

        bool is_valid() const
        {
            return (_size > 0) & is_valid_codepoint(_data);
        }

        size_t size() const { return _size; }

    public: // iterators

        const char16_t* begin() const { return _data; }

        const char16_t* end() const { return _data + _size; }
    };

    //--------------------------------------------------------------------------

    inline bool utf16_codepoint::operator()(const char32_t c)
    {
        const bool is_size_1 = (c <= 0xFFFF);
        const bool is_size_2 = (not is_size_1) & (c <= 0x10FFFF);

        const size_t size =
            (is_size_1 * 1) +
            (is_size_2 * 2);

        const bool is_legible = (size > 0);

        _data[0] =
            (is_size_1 * (c & 0xFFFF)) +
            (is_size_2 * (0xD800 + ((c >> 10) - 0x40)));

        _data[1] =
            (is_size_2 * (0xDC00 + (c & 0x03FF)));

        _size = size;

        return is_legible;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline bool utf16_codepoint::operator()(const char32_t*& p)
    {
        const char32_t c = p[0];

        const bool is_legible = operator()(c);

        p += is_legible;

        return is_legible;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline bool utf16_codepoint::operator()(const char16_t*& p)
    {
        const char16_t c0 = p[0];
        const char16_t c1 = p[1];

        const bool is_c0_head = is_surrogate_head(c0);
        const bool is_c1_tail = is_surrogate_tail(c1);

        const bool is_size_2 = (is_c0_head & is_c1_tail);

        const size_t size = 1 + is_size_2;

        const bool is_legible = (size > 0);

        _data[0] = c0;
        _data[1] = c1 * is_size_2;

        _size = size;

        p += size;

        return is_legible;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline bool utf16_codepoint::operator()(const char*& p)
    {
        char32_t c;

        const size_t utf8_size = to_codepoint(c, p);

        const bool is_legible_utf8  = (utf8_size > 0);
        const bool is_legible_utf32 = operator()(c);
        const bool is_legible       = is_legible_utf8 & is_legible_utf32;

        _size *= is_legible;

        p += is_legible * utf8_size;

        return is_legible;
    }

    //--------------------------------------------------------------------------

} // namespace xtd