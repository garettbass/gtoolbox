#pragma once
#include "../inline.h"

namespace peg {

    //--------------------------------------------------------------------------
    
    template <typename Expr, typename... Exprs>
    struct choice : lexeme
    {
        using lexeme::lexeme;
    
        template <typename Parser>
        peg_inline
        static bool lex(Parser& p, cursor& s)
        {
            cursor t = s;
            
            if (Expr::lex(p, t) or choice<Exprs...>::lex(p, t))
            {
                s = t;
                
                return true;
            }
            
            return false;
        }

    };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    template <typename Expr>
    struct choice<Expr> : lexeme
    {
        using lexeme::lexeme;
    
        template <typename Parser>
        peg_inline
        static bool lex(Parser& p, cursor& s)
        {
            return Expr::lex(p, s);
        }
    };
    
    //--------------------------------------------------------------------------

} // namespace peg