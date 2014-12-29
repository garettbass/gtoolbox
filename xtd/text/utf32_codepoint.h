#pragma once
#include "./unicode.h"

namespace xtd {

    class utf32_codepoint
    {
        char32_t _data[2] { 0, 0 };

        size_t _size = 0;

    public: // structors

        utf32_codepoint() = default;

    public: // operators

        bool operator () (const char32_t);
        bool operator () (const char32_t*&);
        bool operator () (const char16_t*&);
        bool operator () (const char*&);

        const char32_t& operator [] (size_t i) const { return _data[i]; }

    public: // properties

        const char32_t* data() const { return _data; }

        bool is_valid() const
        {
            return (_size > 0) & is_valid_codepoint(_data[0]);
        }

        size_t size() const { return _size; }

    public: // iterators

        const char32_t* begin() const { return _data; }

        const char32_t* end() const { return _data + _size; }
    };

    //--------------------------------------------------------------------------

    inline bool utf32_codepoint::operator()(const char32_t u)
    {
        _data[0] = u;

        _size = 1;

        return true;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline bool utf32_codepoint::operator()(const char32_t*& p)
    {
        _data[0] = p[0];

        _size = 1;

        p += 1;

        return true;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline bool utf32_codepoint::operator()(const char16_t*& p)
    {
        const size_t utf16_size = to_codepoint(_data[0], p);

        const bool is_legible = (utf16_size > 0);

        _size = is_legible;

        p += utf16_size;

        return is_legible;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    inline bool utf32_codepoint::operator()(const char*& p)
    {
        const size_t utf8_size = to_codepoint(_data[0], p);

        const bool is_legible = (utf8_size > 0);

        _size = is_legible;

        p += utf8_size;

        return is_legible;
    }

    //--------------------------------------------------------------------------

} // namespace xtd