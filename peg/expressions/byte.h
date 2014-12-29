#pragma once
#include "../inline.h"

namespace peg {

    //--------------------------------------------------------------------------
        
    template <uint8_t... BYTES>
    struct byte : lexeme
    {
        using lexeme::lexeme;

        template <typename Parser>
        peg_inline
        static bool lex(const Parser&, cursor& s)
        {
            static const char CHARS[] { char(BYTES)... };

            enum : size_t { SIZE = sizeof...(BYTES) };

            const char c = *s;

            for (int i = 0; i < SIZE; ++i)
            {
                if (c == CHARS[i])
                {
                    s.advance();

                    return true;
                }
            }

            return false;
        }
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    template <uint8_t BYTE>
    struct byte<BYTE> : lexeme
    {
        using lexeme::lexeme;

        template <typename Parser>
        peg_inline
        static bool lex(const Parser&, cursor& s)
        {
            const uint8_t u = *s;
            
            if (u == BYTE)
            {
                s.advance();

                return true;
            }
            
            return false;
        }
    };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    #define peg_char(c) ::peg::byte<c>

    //--------------------------------------------------------------------------

} // namespace peg