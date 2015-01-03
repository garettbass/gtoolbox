#pragma once
#include <gtoolbox/xtd/int.h>
#include <gtoolbox/env/compiler.h>

namespace peg {

    //--------------------------------------------------------------------------
    
    class cursor
    {
    private: // fields
        
        union {
            const uint8_t* _byte_ptr = nullptr;
            const char*    _char_ptr;
        };

        unsigned _column  = 1;
        unsigned _line    = 1;
        size_t   _offset  = 0;

    public: // structors
    
        explicit cursor() = default;
        
        explicit cursor(const char* p) : _char_ptr(p) {}
        
        explicit cursor(const uint8_t* p) : _byte_ptr(p) {}
        
    public: // operators
        
        operator const uint8_t*() const { return _byte_ptr; }
    
        operator const char*() const { return _char_ptr; }
    
        bool operator == (const cursor& c) const
        {
            return _byte_ptr == c._byte_ptr;
        }
    
        bool operator != (const cursor& c) const { return not operator==(c); }
    
        const char& operator [] (size_t i) const { return _char_ptr[i]; }
        
        const char& operator * () const { return *_char_ptr; }
        
        const char* operator -> () const { return _char_ptr; }
        
        cursor& operator ++ () { advance(); return *this; }
        
        const cursor operator ++ (int) { auto c = *this; advance(); return c; }
        
        cursor& operator += (size_t i) { advance(i); return *this; }
        
    public: // properties
    
        unsigned column() const { return _column; }
        
        unsigned line() const { return _line; }
        
        size_t offset() const { return _offset; }
        
    public: // methods

        env_forceinline
        void advance()
        {
            const unsigned char_0 = _byte_ptr[0];
            
            const unsigned char_1 = _byte_ptr[1];
            
            const unsigned is_cr(char_0 == '\r' and char_1 != '\n');
            
            const unsigned is_lf(char_0 == '\n');
            
            const unsigned is_line(is_cr or is_lf);
            
            const unsigned bits_7_6(char_0 >> 6);
            
            const unsigned bit_6(bits_7_6 & 1);
            
            const unsigned bit_7(bits_7_6 >> 1);
            
            const unsigned is_utf8_header(bit_6 & bit_7);
            
            const unsigned is_ascii(not bit_7);
            
            const unsigned is_column(is_ascii | is_utf8_header);
            
            _column = (_column + is_column) * (not is_line);
            
            _line += is_line;
            
            _offset += 1;
            
            _byte_ptr += 1;
        }

        env_forceinline
        void advance(size_t count) { while(count--) advance(); }
        
    };

    //--------------------------------------------------------------------------

} // namespace peg