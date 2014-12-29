#pragma once
#include <zna/source/thirdParty/gtoolbox/peg/grammar.h>

namespace peg {

    peg_grammar(peg_grammar2) {

        peg_rules(
            Comment,
            Eof,
            Identifier,
            NewLine,
            Space,
            Tab,
            Whitespace,
            sp,
            __,
            BlockComment,
            BlockCommentHead,
            BlockCommentBody,
            BlockCommentTail,
            LineComment,
            LineCommentHead,
            LineCommentBody,
            LineCommentTail,
            File,
            Namespace,
            NamespaceName,
            NamespaceBody,
            Grammar,
            GrammarName,
            GrammarBody,
            Rule,
            RuleName,
            RuleBody,
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
            CharLiteral,
            CharSpecifier,
            EscapeSequence,
            EscapedNul,
            EscapedQuote,
            EscapedDoubleQuote,
            EscapedQuestionMark,
            EscapedBackslash,
            EscapedBell,
            EscapedBackspace,
            EscapedFormFeed,
            EscapedLineFeed,
            EscapedReturn,
            EscapedTab,
            EscapedVerticalTab,
            EscapedHexSequence,
            HexDigit,
            COUNT
        );

        peg_rule(
            Comment,
            choice<
                BlockComment,
                LineComment
            >
        );

        peg_rule(
            Eof,
            peg_char('\0')
        );

        peg_rule(
            Identifier,
            seq<
                char_class<
                    '_',
                    'a-z',
                    'A-Z'
                >,
                star<
                    char_class<
                        '_',
                        'a-z',
                        'A-Z',
                        '0-9'
                    >
                >
            >
        );

        peg_rule(
            NewLine,
            choice<
                peg_char('\n'),
                seq<
                    peg_char('\r'),
                    opt<
                        peg_char('\n')
                    >
                >
            >
        );

        peg_rule(
            Space,
            peg_char(' ')
        );

        peg_rule(
            Tab,
            peg_char('\t')
        );

        peg_rule(
            Whitespace,
            choice<
                Comment,
                NewLine,
                Space,
                Tab
            >
        );

        peg_rule(
            sp,
            plus<
                Whitespace
            >
        );

        peg_rule(
            __,
            star<
                Whitespace
            >
        );

        peg_rule(
            BlockComment,
            seq<
                BlockCommentHead,
                BlockCommentBody,
                BlockCommentTail
            >
        );

        peg_rule(
            BlockCommentHead,
            peg_literal("/*")
        );

        peg_rule(
            BlockCommentBody,
            star<
                seq<
                    not_predicate<
                        BlockCommentTail
                    >,
                    dot
                >
            >
        );

        peg_rule(
            BlockCommentTail,
            peg_literal("*/")
        );

        peg_rule(
            LineComment,
            seq<
                LineCommentHead,
                LineCommentBody,
                LineCommentTail
            >
        );

        peg_rule(
            LineCommentHead,
            peg_literal("//")
        );

        peg_rule(
            LineCommentBody,
            star<
                seq<
                    not_predicate<
                        LineCommentTail
                    >,
                    dot
                >
            >
        );

        peg_rule(
            LineCommentTail,
            choice<
                Eof,
                NewLine
            >
        );

        peg_rule(
            File,
            seq<
                __,
                NamespaceBody,
                Eof
            >
        );

        peg_rule(
            Namespace,
            seq<
                peg_literal("namespace"),
                sp,
                NamespaceName,
                __,
                peg_char('{'),
                __,
                NamespaceBody,
                peg_char('}')
            >
        );

        peg_rule(
            NamespaceName,
            Identifier
        );

        peg_rule(
            NamespaceBody,
            star<
                seq<
                    choice<
                        Namespace,
                        Grammar
                    >,
                    __
                >
            >
        );

        peg_rule(
            Grammar,
            seq<
                peg_literal("grammar"),
                sp,
                GrammarName,
                __,
                peg_char('{'),
                __,
                GrammarBody,
                peg_char('}')
            >
        );

        peg_rule(
            GrammarName,
            Identifier
        );

        peg_rule(
            GrammarBody,
            star<
                seq<
                    Rule,
                    __
                >
            >
        );

        peg_rule(
            Rule,
            seq<
                RuleName,
                __,
                peg_char('='),
                __,
                RuleBody,
                __,
                peg_char(';')
            >
        );

        peg_rule(
            RuleName,
            Identifier
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
            Choice,
            seq<
                ChoiceExpr,
                plus<
                    seq<
                        __,
                        peg_char('|'),
                        __,
                        ChoiceExpr
                    >
                >
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
            Sequence,
            seq<
                SequenceExpr,
                plus<
                    seq<
                        __,
                        peg_char('|'),
                        __,
                        SequenceExpr
                    >
                >
            >
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
            seq<
                PrimaryExpr,
                peg_char('?')
            >
        );

        peg_rule(
            ZeroOrMore,
            seq<
                PrimaryExpr,
                peg_char('*')
            >
        );

        peg_rule(
            OneOrMore,
            seq<
                PrimaryExpr,
                peg_char('+')
            >
        );

        peg_rule(
            PrimaryExpr,
            Identifier
        );

        peg_rule(
            CharLiteral,
            seq<
                peg_literal("'"),
                plus<
                    CharSpecifier
                >,
                peg_literal("'")
            >
        );

        peg_rule(
            CharSpecifier,
            choice<
                EscapeSequence,
                not_predicate<
                    char_class<
                        '\'',
                        '\\',
                        '\n'
                    >
                >
            >
        );

        peg_rule(
            EscapeSequence,
            choice<
                EscapedNul,
                EscapedQuote,
                EscapedDoubleQuote,
                EscapedQuestionMark,
                EscapedBackslash,
                EscapedBell,
                EscapedBackspace,
                EscapedFormFeed,
                EscapedLineFeed,
                EscapedReturn,
                EscapedTab,
                EscapedVerticalTab,
                EscapedHexSequence
            >
        );

        peg_rule(
            EscapedNul,
            peg_literal("\\0")
        );

        peg_rule(
            EscapedQuote,
            peg_literal("\\'")
        );

        peg_rule(
            EscapedDoubleQuote,
            peg_literal("\\\"")
        );

        peg_rule(
            EscapedQuestionMark,
            peg_literal("\\?")
        );

        peg_rule(
            EscapedBackslash,
            peg_literal("\\\\")
        );

        peg_rule(
            EscapedBell,
            peg_literal("\\a")
        );

        peg_rule(
            EscapedBackspace,
            peg_literal("\\b")
        );

        peg_rule(
            EscapedFormFeed,
            peg_literal("\\f")
        );

        peg_rule(
            EscapedLineFeed,
            peg_literal("\\n")
        );

        peg_rule(
            EscapedReturn,
            peg_literal("\\r")
        );

        peg_rule(
            EscapedTab,
            peg_literal("\\t")
        );

        peg_rule(
            EscapedVerticalTab,
            peg_literal("\\v")
        );

        peg_rule(
            EscapedHexSequence,
            seq<
                peg_literal("\\x"),
                plus<
                    HexDigit
                >
            >
        );

        peg_rule(
            HexDigit,
            char_class<
                '0-9',
                'a-f',
                'A-F'
            >
        );
    };
}
