#pragma once
#include "./cursor.h"
#include "./grammar.h"
#include "./utility.h"

namespace peg {

    struct keyword_struct;
    
    using _structs = rep<keyword_struct, 2>;

    peg_rule(keyword_struct, peg_literal("struct"));
    
    using _lf = byte<'\n'>;
    
    using _cr = byte<'\r'>;
    
    using _crlf = sequence<_cr, _lf>;
    
    using __ = byte<'_'>;

    using _0 = byte<'0'>;

    using _0_9 = range<'0','9'>;
    
    using _1_9 = range<'1','9'>;
    
    using _a_z = range<'a','z'>;
    
    using _A_Z = range<'A','Z'>;
    
    using _a_f = range<'a','f'>;
    
    using _A_F = range<'A','F'>;
    
    using _hex = choice<_0_9, _a_f, _A_F>;
    
    using _0x = sequence<_0, byte<'x','X'>>;
    
    using sign = byte<'-','+'>;
    
    using uniquad = sequence<byte<'\\'>, byte<'u','U'>, rep<_hex, 4>>;
    
    using positive_integer = sequence<_1_9, star<_0_9>>;
    
    using cardinal_integer = choice<_0, positive_integer>;
    
    using integer = sequence<opt<sign>, cardinal_integer>;
    
    using hex_number = sequence<_0x, plus<_hex>>;

    using identifier_head = choice<__, _a_z, _A_Z>;
        
    using identifier_body = choice<identifier_head, _0_9>;
    
    using identifier = sequence<identifier_head, star<identifier_body>>;
    
    using newline = choice<_lf, sequence<_cr, opt<_lf>>>;

    //--------------------------------------------------------------------------

    inline void run_tests()
    {
        cursor c("hello\nworld");
        
        assert(c.column()    ==  1);
        assert(c.line()      ==  1);
        assert(c.offset()    ==  0);
        assert(*c == 'h');
        
        ++c;
        
        assert(c.column()    ==  2);
        assert(c.line()      ==  1);
        assert(c.offset()    ==  1);
        assert(*c == 'e');
        
        c.advance(5);
        
        assert(c.column()    ==  1);
        assert(c.line()      ==  2);
        assert(c.offset()    ==  6);
        assert(*c == 'w');
        
        assert(+is_match<newline>("\n"));
        assert(+is_match<newline>("\r\n"));
        assert(+is_match<newline>("\r"));
    
        assert(+is_match<_0>("0"));
        assert(!is_match<_0>("1"));
        assert(+starts_with<_0>("01"));
        
        assert(+is_match<uniquad>(R"(\u0000)"));
        assert(!is_match<uniquad>(R"(\u000)"));
        assert(!is_match<uniquad>(R"(\u00000)"));
        
        assert(+is_match<_structs>("structstruct"));
        assert(!is_match<_structs>("structstruct "));
        
        assert(+starts_with<_structs>("structstruct "));
        assert(!starts_with<_structs>(" structstruct"));
        
        assert(+is_match<keyword_struct>("struct"));
        assert(!is_match<keyword_struct>("struct "));
        
        assert(+starts_with<keyword_struct>("struct "));
        assert(!starts_with<keyword_struct>(" struct"));
        
        printf("%s passed\n", __PRETTY_FUNCTION__);
    }

} // namespace peg