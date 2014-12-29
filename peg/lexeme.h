#pragma once
#include <cassert>
#include <cstring>
#include "./cursor.h"

namespace peg {

    //--------------------------------------------------------------------------

    struct lexeme
    {
        cursor first;
        cursor last;

    public: // structors
    
        lexeme() = default;

        lexeme(cursor cursor)
        : first(cursor)
        , last(cursor)
        {}

        lexeme(cursor cursor, size_t size)
        : first(cursor)
        , last(cursor)
        { last.advance(size); }
        
        lexeme(cursor first, cursor last)
        : first(first)
        , last(last)
        {}

    public: // operators

        bool operator == (const lexeme& l) const
        {
            const size_t this_size = size();
            
            const size_t that_size = l.size();
            
            if (this_size != that_size)
                return false;
            
            const char* this_data = data();
            
            const char* that_data = l.data();
        
            return not strncmp(this_data, that_data, this_size);
        }
        
        bool operator != (const lexeme& l) const { return not operator==(l); }

        const char& operator[] (size_t i) const
        {
            assert(i < size());
            
            return first[i];
        }

    public: // properties
    
        size_t column() const { return first.column(); }
        
        const char* data() const { return first; }
        
        bool empty() const { return not size(); }
        
        size_t line() const { return first.line(); }
        
        size_t offset() const { return first.offset(); }

        size_t size() const { return last.offset() - first.offset(); }

    public: // iterators

        cursor begin() const { return first; }
        
        cursor end() const { return last; }

    };
    
    template <typename Stream>
    auto
    operator << (Stream& stream, const lexeme& s)
    -> decltype(stream << 0)
    {
        return stream.write(s.data(), s.size());
    }

    //--------------------------------------------------------------------------

} // namespace peg