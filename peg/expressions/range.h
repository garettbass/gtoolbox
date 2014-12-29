#pragma once
#include "../inline.h"

namespace peg {

    //--------------------------------------------------------------------------
    
    template <uint8_t MIN, uint8_t MAX>
    struct range : lexeme
    {
        static_assert(MIN < MAX, "MIN must be less than MAX");

        using lexeme::lexeme;
    
        template <typename Parser>
        peg_inline
        static bool lex(const Parser&, cursor& s)
        {
            const uint8_t c = *s;
        
            if ((MIN <= c) and (c <= MAX))
            {
                s.advance();

                return true;
            }
            return false;
        }
    };

    //--------------------------------------------------------------------------

} // namespace peg