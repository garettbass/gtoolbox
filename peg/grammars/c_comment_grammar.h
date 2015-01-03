#pragma once
#include <gtoolbox/peg/grammars/ascii_grammar.h>

namespace peg {

    peg_grammar(c_comment_grammar)
    {
        peg_rules(
            block_comment_head,
            block_comment_tail,
            block_comment_body,
            block_comment,
            line_comment_head,
            line_comment_body,
            line_comment_tail,
            line_comment,
            comment,
            whitespace,
            error_unexpected_eof,
            error_unexpected_control_char
        );

        peg_rule(
            error_unexpected_eof,
            peg::ascii_grammar::error_unexpected_eof);

        peg_rule(
            error_unexpected_control_char,
            peg::ascii_grammar::error_unexpected_control_char);

        // block comment \\.....................................................

        peg_rule(block_comment_head, peg_literal("/*"));

        peg_rule(block_comment_tail, peg_literal("*/"));

        peg_rule(
            block_comment_body,
            star<sequence<
                not_predicate<block_comment_tail>,
                choice<
                    peg::ascii_grammar::printable_char,
                    peg::ascii_grammar::newline,
                    error_unexpected_eof,
                    error_unexpected_control_char
                >
            >>
        );

        peg_rule(
            block_comment,
            sequence<
                block_comment_head,
                block_comment_body,
                block_comment_tail
            >
        );

        // line comment \\......................................................

        peg_rule(line_comment_head, peg_literal("//"));

        peg_rule(
            line_comment_tail,
            choice<eof, peg::ascii_grammar::newline>
        );

        peg_rule(
            line_comment_body,
            star<sequence<
                not_predicate<line_comment_tail>,
                peg::ascii_grammar::printable_char
            >>
        );

        peg_rule(
            line_comment,
            sequence<
                line_comment_head,
                line_comment_body,
                line_comment_tail
            >
        );

        // comment \\...........................................................

        peg_rule(
            comment,
            choice<
                block_comment,
                line_comment
            >
        );

        // whitespace \\........................................................

        peg_rule(
            whitespace,
            choice<
                peg::ascii_grammar::whitespace,
                comment
            >
        );

    };

} // namespace peg