#pragma once
#include "../parser.h"

namespace peg {

    //--------------------------------------------------------------------------
    
    template <typename Expr>
    struct expression_count_parser : parser
    {
        size_t count = 0;
        
        using parser::parse;
    
        void parse(const Expr&) {
            ++count;
        }
    
    };

    //--------------------------------------------------------------------------

} // namespace peg