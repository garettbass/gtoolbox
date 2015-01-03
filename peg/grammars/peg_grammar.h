#pragma once
#include <gtoolbox/peg/grammars/c_comment_grammar.h>
#include <gtoolbox/peg/grammars/c_identifier_grammar.h>
#include <gtoolbox/peg/grammars/c_literal_grammar.h>

namespace peg {

    peg_grammar(peg_grammar)
    {
        peg_rules(
            __,
            sp,
            Identifier,
            File,
            Namespace,
            NamespaceName,
            NamespaceHead,
            NamespaceBody,
            NamespaceTail,
            Grammar,
            GrammarName,
            GrammarHead,
            GrammarBody,
            GrammarTail,
            Rule,
            RuleName,
            RuleHead,
            RuleBody,
            RuleTail,
            RuleReference,
            Expr,
            Dot,
            Choice,
            ChoiceExpr,
            Sequence,
            SequenceExpr,
            Predicate,
            AndPredicate,
            NotPredicate,
            Repetition,
            ZeroOrOne,
            ZeroOrMore,
            OneOrMore,
            PrimaryExpr,
            NestedExpr,
            Literal,
            CharLiteral,
            StringLiteral,
            CharClass,
            CharClassHead,
            CharClassTail,
            CharClassRange,
            CharClassSingle,
            CharClassChar,
            escaped_hyphen,
            escaped_lbrack,
            escaped_rbrack,
            unescaped_char,
            escape_sequence,
            Char,
            COUNT
        );

        peg_rule(__, star<peg::c_comment_grammar::whitespace>);
        peg_rule(sp, plus<peg::c_comment_grammar::whitespace>);

        peg_rule(
            Identifier,
            seq<
                char_class<'_','a-z','A-Z'>,
                star<char_class<'_','a-z','A-Z','0-9'>>
            >
        );

        peg_rule(
            File,
            seq<
                __,
                NamespaceBody,
                eof
            >
        );

        peg_rule(
            Namespace,
            seq<
                NamespaceHead,
                NamespaceBody,
                NamespaceTail
            >
        );

        peg_rule(
            NamespaceName,
            Identifier
        );

        peg_rule(
            NamespaceHead,
            seq<
                peg_literal("namespace"),sp,
                NamespaceName,__,
                peg_char('{'),__
            >
        );

        peg_rule(
            NamespaceBody,
            star<seq<choice<Namespace, Grammar>,__>>
        );

        peg_rule(
            NamespaceTail,
            peg_char('}')
        );

        peg_rule(
            Grammar,
            seq<
                GrammarHead,
                GrammarBody,
                GrammarTail
            >
        );

        peg_rule(
            GrammarName,
            Identifier
        );

        peg_rule(
            GrammarHead,
            seq<
                peg_literal("grammar"),
                sp,
                GrammarName,
                __,
                peg_char('{'),
                __
            >
        );

        peg_rule(
            GrammarBody,
            star<seq<Rule,__>>
        );

        peg_rule(
            GrammarTail,
            peg_char('}')
        );

        peg_rule(
            RuleReference,
            Identifier
        );

        peg_rule(
            RuleName,
            Identifier
        );

        peg_rule(
            Rule,
            seq<
                RuleHead,
                RuleBody,
                RuleTail
            >
        );

        peg_rule(
            RuleHead,
            seq<
                RuleName,
                __,
                peg_char('='),
                __
            >
        );

        peg_rule(
            RuleBody,
            choice<
                Choice,
                Sequence,
                Predicate,
                Repetition,
                PrimaryExpr
            >
        );

        peg_rule(
            RuleTail,
            seq<
                __,
                peg_char(';')
            >
        );

        peg_rule(
            ChoiceExpr,
            choice<
                Sequence,
                Predicate,
                Repetition,
                PrimaryExpr
            >
        );

        peg_rule(
            Choice,
            seq<ChoiceExpr,plus<seq<__,peg_char('|'),__,ChoiceExpr>>>
        );

        peg_rule(
            SequenceExpr,
            choice<
                Predicate,
                Repetition,
                PrimaryExpr
            >
        );

        peg_rule(
            Sequence,
            seq<SequenceExpr,plus<seq<__,SequenceExpr>>>
        );

        peg_rule(
            Predicate,
            choice<
                AndPredicate,
                NotPredicate
            >
        );

        peg_rule(
            AndPredicate,
            seq<
                peg_char('&'),
                choice<
                    Repetition,
                    PrimaryExpr
                >
            >
        );

        peg_rule(
            NotPredicate,
            seq<
                peg_char('!'),
                choice<
                    Repetition,
                    PrimaryExpr
                >
            >
        );

        peg_rule(
            Repetition,
            choice<
                ZeroOrOne,
                ZeroOrMore,
                OneOrMore
            >
        );

        peg_rule(
            ZeroOrOne,
            seq<PrimaryExpr,peg_char('?')>
        );

        peg_rule(
            ZeroOrMore,
            seq<PrimaryExpr,peg_char('*')>
        );

        peg_rule(
            OneOrMore,
            seq<PrimaryExpr,peg_char('+')>
        );

        peg_rule(
            PrimaryExpr,
            choice<
                RuleReference,
                seq<peg_char('('),__,NestedExpr,__,peg_char(')')>,
                Literal,
                CharClass,
                Dot
            >
        );

        peg_rule(
            Dot,
            peg_char('.')
        );

        peg_rule(
            NestedExpr,
            choice<
                Choice,
                Sequence,
                Predicate,
                Repetition,
                PrimaryExpr
            >
        );

        peg_rule(
            Literal,
            choice<
                CharLiteral,
                StringLiteral
            >
        );

        peg_rule(
            CharLiteral,
            peg::c_literal_grammar::char_literal
        );

        peg_rule(
            StringLiteral,
            peg::c_literal_grammar::string_literal
        );

        peg_rule(
            CharClassHead,
            peg_char('[')
        );

        peg_rule(
            CharClassTail,
            peg_char(']')
        );

        peg_rule(
            CharClass,
            seq<
                CharClassHead,
                plus<
                    choice<
                        CharClassRange,
                        CharClassSingle
                    >
                >,
                CharClassTail
            >
        );

        peg_rule(
            CharClassRange,
            seq<CharClassChar,peg_char('-'),CharClassChar>
        );

        peg_rule(
            CharClassSingle,
            CharClassChar
        );

        peg_rule(escaped_hyphen, peg_literal(R"(\-)"));
        peg_rule(escaped_lbrack, peg_literal(R"(\[)"));
        peg_rule(escaped_rbrack, peg_literal(R"(\])"));
        peg_rule(unescaped_char, char_class<'\t',' -~'>);

        peg_rule(escape_sequence, peg::c_literal_grammar::escape_sequence);

        peg_rule(
            CharClassChar,
            seq<
                not_predicate<peg_char(']')>,
                not_predicate<peg_char('-')>,
                choice<
                    escaped_hyphen,
                    escaped_lbrack,
                    escaped_rbrack,
                    escape_sequence,
                    unescaped_char
                >
            >
        );
    };

} // namespace peg