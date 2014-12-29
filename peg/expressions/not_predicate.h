#pragma once
#include "../inline.h"
#include "./and_predicate.h"

namespace peg {

    //--------------------------------------------------------------------------

    template <typename Expr = void>
    struct not_predicate : lexeme
    {
        using lexeme::lexeme;
        
        template <typename Parser>
        peg_inline
        static bool lex(Parser& p, const cursor s)
        {
            return not and_predicate<Expr>::lex(p, s);
        }
    };

    template <>
    struct not_predicate<void> : lexeme
    {
        using lexeme::lexeme;

        template <typename Parser>
        peg_inline
        static bool lex(Parser&, const cursor)
        {
            return false;
        }
    };
    
    //--------------------------------------------------------------------------

} // namespace peg