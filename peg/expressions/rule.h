#pragma once
#include <zna/source/thirdParty/gtoolbox/pre/apply.h>
#include "../inline.h"

namespace peg {

    //--------------------------------------------------------------------------

    template <typename Grammar, typename Rule, typename Expr>
    struct rule : lexeme
    {
        using grammar = Grammar;

        using lexeme::lexeme;
        
        template <typename Parser>
        static bool lex(Parser& p, cursor& s)
        {
            Rule r = s;

            auto marker = p.lexing(r);

            if (Expr::lex(p, r.last))
            {
                p.accepted(r, marker);

                p.parse(r);

                s = r.last;
                
                return true;
            }

            p.rejected(r, marker);

            return false;
        }

        static typename grammar::rule id() { return Rule::id(); }

        static const xtd::substring name() { return Rule::name(); }
    };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    #define peg_rule(identifier, ...)                                          \
        struct identifier : ::peg::rule<grammar, identifier, __VA_ARGS__>      \
        {                                                                      \
            using rule::rule;                                                  \
                                                                               \
            static grammar::rule id() { return grammar::rule::identifier; }    \
                                                                               \
            static const xtd::substring name() { return #identifier; }         \
        }

    #define peg_forward(identifier) struct identifier

    #define peg_rule_declare(name) struct name;

    #define peg_rule_name(name) #name,

    #define peg_rule_enum(name) name,

    #define peg_rules(...)                            \
        pre_apply(peg_rule_declare, __VA_ARGS__)      \
        enum class rule : unsigned {                  \
            none = 0,                                 \
            pre_apply(peg_rule_enum, __VA_ARGS__)     \
        };                                            \
        static const char* rule_name(rule r) {        \
            static const char* names[] {              \
                "<none>",                             \
                pre_apply(peg_rule_name, __VA_ARGS__) \
            };                                        \
            return names[unsigned(r)];                \
        }

    #define peg_import(grammar, identifier) \
        peg_rule(identifier, grammar::identifier)
        
    //--------------------------------------------------------------------------

} // namespace peg