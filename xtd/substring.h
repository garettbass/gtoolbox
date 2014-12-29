#pragma once
#include <cassert>
#include <functional>
#include <string>
#include "./hash.h"

namespace xtd {

    //==========================================================================

    class substring
    {
    public: // types
    
        typedef const char* const_iterator;
    
    private: // fields
    
        const char* _data = "";
        std::size_t _size = 0;

    public: // structors

        constexpr substring() = default;
        
        template <size_t N>
        constexpr substring(const char (&s)[N])
        : _data(s)
        , _size(N - 1)
        {}

        constexpr substring(const char* s, size_t size)
        : _data(s ? s : "")
        , _size(s ? size : 0)
        {}

        substring(const char* s)
        : _data(s ? s : "")
        , _size(strlen(_data))
        {}

        substring(const std::string& s)
        : _data(s.data() ? s.data() : "")
        , _size(s.size())
        {}

        constexpr substring(const substring&) = default;

        substring& operator = (const substring&) = default;

    public: // operators

        explicit operator std::string() const
        {
            return std::string(data(), size());
        }

        constexpr char operator[](std::size_t i) const
        {
            return (i < _size) ? _data[i] : '\0';
        }

    public: // properties

        constexpr bool empty() const { return _size == 0; }
        
        constexpr const char* data() const { return _data; }

        constexpr size_t size() const { return _size; }

    public: // methods
    
        const char& at(size_t i) const
        {
            assert(i < _size);

            return _data[i];
        }

        int compare(const char* s) const
        {
            return strncmp(_data, s, _size);
        }

        int compare(const std::string& s) const
        {
            return strncmp(_data, s.c_str(), _size);
        }

        int compare(const substring& s) const
        {
            return strncmp(_data, s._data, _size);
        }

        constexpr substring tail(size_t i = 1) const
        {
            return
                (_size > i)
                ? substring(_data + i, _size - i)
                : substring();
        }
        
    public: // iterators
    
        const_iterator cbegin() const { return _data; }
        const_iterator  begin() const { return _data; }
        
        const_iterator cend() const { return _data + _size; }
        const_iterator  end() const { return _data + _size; }

    };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    inline
    bool operator < (const substring& a, const substring& b)
    {
        return a.compare(b) < 0;
    }
    
    inline
    bool operator > (const substring& a, const substring& b)
    {
        return a.compare(b) > 0;
    }

    inline
    bool operator == (const substring& a, const substring& b)
    {
        return a.compare(b) == 0;
    }

    template <typename Stream>
    auto
    operator << (Stream& stream, const substring& s)
    -> decltype(stream << 0)
    {
        stream.write(s.data(), s.size());

        return stream;
    }

    //==========================================================================

} // namespace xtd

namespace std {

    template <>
    struct hash<xtd::substring>
    {
        typedef uint32_t result_type;

        typedef xtd::substring argument_type;

        uint32_t operator()(const xtd::substring& s) const
        {
            return xtd::hash32(s.data(), uint32_t(s.size()));
        }
    };

} // namespace std
