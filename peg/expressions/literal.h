#pragma once
#include "../inline.h"

namespace peg {

    //--------------------------------------------------------------------------

    template <unsigned SIZE, char... CHARS>
    struct literal : lexeme
    {
        static_assert(SIZE <= sizeof...(CHARS), "literal size mismatch");

        using lexeme::lexeme;

        template <typename Parser>
        peg_inline
        static bool lex(const Parser&, cursor& s)
        {
            static const char* str = c_str();

            static const size_t len = strlen(str);

            if (strncmp(s, str, len) == 0)
            {
                s.advance(len);

                return true;
            }

            return false;
        }

        static const char* c_str()
        {
            static const char* c_str = [](){
                static char c_str[SIZE] { 0 };

                const char chars[] { CHARS... };

                strcpy(c_str, chars);

                return c_str;
            }();

            return c_str;
        };
    };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    template <unsigned SIZE>
    constexpr
    char
    literal_char(unsigned i, const char (&literal)[SIZE])
    {
        static_assert(
            SIZE < 32,
            "string literal must be shorter than 32 chars");

        return (i < SIZE) ? literal[i] : 0;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    #define peg_literal(str)              \
        ::peg::literal<                   \
            sizeof(str),                  \
            ::peg::literal_char( 0, str), \
            ::peg::literal_char( 1, str), \
            ::peg::literal_char( 2, str), \
            ::peg::literal_char( 3, str), \
            ::peg::literal_char( 4, str), \
            ::peg::literal_char( 5, str), \
            ::peg::literal_char( 6, str), \
            ::peg::literal_char( 7, str), \
            ::peg::literal_char( 8, str), \
            ::peg::literal_char( 9, str), \
            ::peg::literal_char(10, str), \
            ::peg::literal_char(11, str), \
            ::peg::literal_char(12, str), \
            ::peg::literal_char(13, str), \
            ::peg::literal_char(14, str), \
            ::peg::literal_char(15, str), \
            ::peg::literal_char(16, str), \
            ::peg::literal_char(17, str), \
            ::peg::literal_char(18, str), \
            ::peg::literal_char(19, str), \
            ::peg::literal_char(20, str), \
            ::peg::literal_char(21, str), \
            ::peg::literal_char(22, str), \
            ::peg::literal_char(23, str), \
            ::peg::literal_char(24, str), \
            ::peg::literal_char(25, str), \
            ::peg::literal_char(26, str), \
            ::peg::literal_char(27, str), \
            ::peg::literal_char(28, str), \
            ::peg::literal_char(29, str), \
            ::peg::literal_char(30, str), \
            ::peg::literal_char(31, str), \
            ::peg::literal_char(32, str)  \
        >

    //--------------------------------------------------------------------------

} // namespace peg