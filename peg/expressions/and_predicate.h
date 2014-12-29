#pragma once
#include "../inline.h"
#include "../parser.h"

namespace peg {

    //--------------------------------------------------------------------------

    template <typename Expr = void>
    struct and_predicate : lexeme
    {
        using lexeme::lexeme;
        
        template <typename Parser>
        peg_inline
        static bool lex(const Parser&, const cursor s)
        {
            parser q; // do not lex into to Parser during evaluation
            
            cursor t = s;
            
            if (Expr::lex(q, t))
            {
                return true;
            }
            
            return false;
        }
    };

    template <>
    struct and_predicate<void> : lexeme
    {
        using lexeme::lexeme;

        template <typename Parser>
        peg_inline
        static bool lex(const Parser&, const cursor)
        {
            return true;
        }
    };

    //--------------------------------------------------------------------------

} // namespace peg