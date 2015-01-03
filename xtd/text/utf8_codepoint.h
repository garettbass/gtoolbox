#pragma once
#include <gtoolbox/xtd/text/utf32_codepoint.h>

namespace xtd {

    class utf8_codepoint
    {
        char _data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

        size_t _size = 0;

    public: // structors

        utf8_codepoint() = default;

    public: // operators

        bool operator () (const char32_t);
        bool operator () (const char32_t*&);
        bool operator () (const char16_t*&);
        bool operator () (const char*&);

        const char& operator [] (size_t i) const { return _data[i]; }

        template <typename Stream>
        auto operator << (Stream& s)
        -> decltype(s << 0) const
        { return s.write(_data, _size); }

    public: // properties

        const char* data() const { return _data; }

        bool is_valid() const
        {
            return (_size > 0) & is_valid_codepoint(_data);
        }

        size_t size() const { return _size; }

    public: // iterators

        const char* begin() const { return _data; }

        const char* end() const { return _data + _size; }
    };

    //--------------------------------------------------------------------------

    inline bool utf8_codepoint::operator()(const char32_t c)
    {
        const bool c_le_0x0000007F = (c <= 0x0000007F);
        const bool c_le_0x000007FF = (c <= 0x000007FF);
        const bool c_le_0x0000FFFF = (c <= 0x0000FFFF);
        const bool c_le_0x0010FFFF = (c <= 0x0010FFFF);

        const bool is_size_1 = c_le_0x0000007F;
        const bool is_size_2 = c_le_0x000007FF & not c_le_0x0000007F;
        const bool is_size_3 = c_le_0x0000FFFF & not c_le_0x000007FF;
        const bool is_size_4 = c_le_0x0010FFFF & not c_le_0x0000FFFF;

        const size_t size =
            (is_size_1 * 1) +
            (is_size_2 * 2) +
            (is_size_3 * 3) +
            (is_size_4 * 4);

        const bool is_legible = (size > 0);

        _data[0] =
            (is_size_1 * (c)) +
            (is_size_2 * (0xC0 + (c >>  6))) +
            (is_size_3 * (0xE0 + (c >> 12))) +
            (is_size_4 * (0xF0 + (c >> 18)));

        _data[1] =
            (is_size_2 * (0x80 + (0x3F & (c)))) +
            (is_size_3 * (0x80 + (0x3F & (c >>  6)))) +
            (is_size_4 * (0x80 + (0x3F & (c >> 12))));

        _data[2] =
            (is_size_3 * (0x80 + (0x3F & (c)))) +
            (is_size_4 * (0x80 + (0x3F & (c >> 6))));

        _data[3] =
            (is_size_4 * (0x80 + (0x3F & (c))));

        _size = size;

        return is_legible;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline bool utf8_codepoint::operator()(const char32_t*& p)
    {
        const char32_t c = p[0];

        const bool is_legible = operator()(c);

        p += is_legible;

        return is_legible;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline bool utf8_codepoint::operator()(const char16_t*& p)
    {
        char32_t c;

        const size_t utf16_size = to_codepoint(c, p);

        const bool is_legible_utf16 = (utf16_size > 0);
        const bool is_legible_utf32 = operator()(c);
        const bool is_legible       = is_legible_utf16 & is_legible_utf32;

        _size *= is_legible;

        p += is_legible * utf16_size;

        return is_legible;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline bool utf8_codepoint::operator()(const char*& p)
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

        const bool is_legible = (size > 0);

        _data[0] = (size >= 1) * c0;
        _data[1] = (size >= 2) * c1;
        _data[2] = (size >= 3) * c2;
        _data[3] = (size >= 4) * c3;

        _size = size;

        p += size;

        return is_legible;
    }

    //--------------------------------------------------------------------------

} // namespace xtd
