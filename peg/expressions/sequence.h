#pragma once
#include "../inline.h"

namespace peg {

    //--------------------------------------------------------------------------

    template <typename Expr, typename... Exprs>
    class _sequence
    : private _sequence<Exprs...>
    {
    protected:
    
        template <typename Parser>
        peg_inline
        static bool lex(Parser& p, cursor& s)
        {
            return Expr::lex(p, s) and _sequence<Exprs...>::lex(p, s);
        }
    };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    template <typename Expr>
    class _sequence<Expr>
    {
    protected:
    
        template <typename Parser>
        peg_inline
        static bool lex(Parser& p, cursor& s)
        {
            return Expr::lex(p, s);
        }        
    };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    template <typename... Exprs>
    struct sequence : lexeme, private _sequence<Exprs...>
    {
        using lexeme::lexeme;
    
        template <typename Parser>
        peg_inline
        static bool lex(Parser& p, cursor& s)
        {
            cursor t = s;

            if (_sequence<Exprs...>::lex(p, t))
            {
                s = t;
                
                return true;
            }

            return false;
        }
    };

    //--------------------------------------------------------------------------

} // namespace peg