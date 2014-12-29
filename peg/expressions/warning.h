#pragma once
#include "../inline.h"
#include "../parser.h"

namespace peg {

    //--------------------------------------------------------------------------

    template <typename Warning, typename Expr>
    struct warning : lexeme
    {
        using lexeme::lexeme;

        template <typename Parser>
        peg_inline
        static bool lex(Parser& p, cursor& s)
        {
            parser q;

            cursor t = s;
            
            if (Expr::lex(q, t))
            {
                p.parse(Warning(s, t));
                
                s = t;

                return true;
            };
            
            return false;
        }
    };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    #define peg_warning(identifier, msg, ...)                                  \
        struct identifier : ::peg::warning<identifier, __VA_ARGS__>            \
        {                                                                      \
            using warning::warning;                                            \
                                                                               \
            static const char* message() { return msg; }                       \
                                                                               \
            static const char* name() { return #identifier; }                  \
        }
    
    //--------------------------------------------------------------------------

} // namespace peg