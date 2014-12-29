#pragma once
#include "../inline.h"

namespace peg {

    //--------------------------------------------------------------------------

    template <typename Expr, size_t MIN, size_t MAX = MIN>
    struct repetition : lexeme
    {
        using lexeme::lexeme;
    
        template <typename Parser>
        peg_inline
        static bool lex(Parser& p, cursor& s)
        {
            cursor t = s;
            
            if (_lex(p, t, MIN))
            {
                _lex(p, t, MAX - MIN);
                
                s = t;

                return true;
            }
            
            return false;
        }
        
    private:
        
        template <typename Parser>
        peg_inline
        static bool _lex(Parser& p, cursor& s, size_t count)
        {
            for (size_t i = 0; i < count; ++i)
                if (not Expr::lex(p, s))
                    return false;
            
            return true;
        }
    };
    
    template <typename Expr, size_t MIN, size_t MAX = MIN>
    using rep = repetition<Expr, MIN, MAX>;

    template <typename Expr>
    using opt = repetition<Expr, 0, 1>; // zero or one repetition
    
    template <typename Expr>
    using star = repetition<Expr, 0, ~size_t(0)>; // zero or more repetitions
    
    template <typename Expr>
    using plus = repetition<Expr, 1, ~size_t(0)>; // one or more repetitions
    
    //--------------------------------------------------------------------------

} // namespace peg