#pragma once
#include <type_traits>
#include <gtoolbox/env/compiler.h>
#include "./lexeme.h"
#include "./expressions/and_predicate.h"
#include "./expressions/byte.h"
#include "./expressions/char_class.h"
#include "./expressions/choice.h"
#include "./expressions/dot.h"
#include "./expressions/error.h"
#include "./expressions/literal.h"
#include "./expressions/not_predicate.h"
#include "./expressions/range.h"
#include "./expressions/repetition.h"
#include "./expressions/rule.h"
#include "./expressions/sequence.h"
#include "./expressions/warning.h"

namespace peg {

    template <typename Grammar>
    class ast;

    //--------------------------------------------------------------------------

    template <typename Grammar>
    struct grammar
    {
    public: // types

        using ast = ast<Grammar>;

    protected: // types

        template <typename Expr = void>
        using and_predicate = peg::and_predicate<Expr>;

        template <uint8_t... Bytes>
        using byte = peg::byte<Bytes...>;

        template <typename... Exprs>
        using choice = peg::choice<Exprs...>;

        using dot = peg::dot;

        using eof = and_predicate<byte<0>>;

        template <typename Expr = void>
        using not_predicate = peg::not_predicate<Expr>;

        using nul = byte<0>;

        template <typename Expr>
        using opt = peg::opt<Expr>;
        
        template <typename Expr>
        using plus = peg::plus<Expr>;
        
        template <uint8_t MIN, uint8_t MAX>
        using range = peg::range<MIN, MAX>;
            
        template <typename Expr, size_t MIN, size_t MAX = MIN>
        using rep = peg::rep<Expr, MIN, MAX>;
    
        template <typename... Exprs>
        using sequence = peg::sequence<Exprs...>;
    
        template <typename... Exprs>
        using seq = peg::sequence<Exprs...>;

        template <typename Expr>
        using star = peg::star<Expr>;

        using todo = not_predicate<void>;
    };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    #define peg_grammar(identifier)                                            \
        struct identifier;                                                     \
        struct identifier##_base : ::peg::grammar<identifier>                  \
        {                                                                      \
            using grammar = identifier;                                        \
                                                                               \
            static const xtd::substring name() { return #identifier; }         \
        };                                                                     \
        struct identifier : identifier##_base

    //--------------------------------------------------------------------------

} // namespace peg