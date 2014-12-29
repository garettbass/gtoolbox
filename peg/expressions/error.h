#pragma once
#include "../inline.h"
#include "../parser.h"

namespace peg {

    //--------------------------------------------------------------------------

    template <typename Error, typename Expr>
    struct error : lexeme
    {
        using lexeme::lexeme;

        template <typename Parser>
        peg_inline
        static bool lex(Parser& p, const cursor s)
        {
            parser q; // do not lex into Parser during evaluation
            
            cursor t = s;
            
            if (Expr::lex(q, t))
            {
                p.parse(Error(s, t));
            };
            
            return false;
        }
    };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    #define peg_error(identifier, msg, ...)                                    \
        struct identifier : ::peg::error<identifier, __VA_ARGS__>              \
        {                                                                      \
            using error::error;                                                \
                                                                               \
            static const char* message() { return msg; }                       \
                                                                               \
            static const char* name() { return #identifier; }                  \
        }
    
    //--------------------------------------------------------------------------

} // namespace peg