#pragma once
#include <gtoolbox/peg/grammars/ascii_grammar.h>

namespace peg {

    peg_grammar(c_literal_grammar)
    {
        peg_rules(
            dec_digit,
            dec_digit_nonzero,
            dec_int_literal,
            hex_digit,
            hex_int_literal,
            int_literal,
            float_integer,
            float_fraction,
            float_exponent_sign,
            float_exponent,
            float_literal,
            escaped_nul,
            escaped_quote,
            escaped_dquote,
            escaped_question,
            escaped_backslash,
            escaped_bell,
            escaped_backspace,
            escaped_formfeed,
            escaped_linefeed,
            escaped_return,
            escaped_tab,
            escaped_vtab,
            escaped_hex_head,
            escaped_hex_digits,
            escaped_hex,
            escape_sequence,
            char_literal_head,
            char_literal_tail,
            char_literal_body,
            char_literal,
            string_literal_head,
            string_literal_tail,
            string_literal_body,
            string_literal,
            error_unexpected_eof,
            error_unexpected_control_char,
            error_unexpected_utf8_sequence
        );

        peg_rule(
            error_unexpected_eof,
            peg::ascii_grammar::error_unexpected_eof
        );

        peg_rule(
            error_unexpected_control_char,
            peg::ascii_grammar::error_unexpected_control_char
        );

        peg_rule(
            error_unexpected_utf8_sequence,
            peg::ascii_grammar::error_unexpected_utf8_sequence
        );

        // int_literal \\.......................................................

        peg_rule(
            dec_digit,
            range<'0','9'>
        );

        peg_rule(
            dec_digit_nonzero,
            range<'1','9'>
        );

        peg_rule(
            dec_int_literal,
            choice<
                byte<'0'>,
                sequence<dec_digit_nonzero, star<dec_digit>>
            >
        );

        peg_rule(
            hex_digit,
            choice<
                range<'0','9'>,
                range<'a','f'>,
                range<'A','F'>
            >
        );

        peg_rule(
            hex_int_literal,
            sequence<
                byte<'0'>,
                choice<
                    byte<'x'>,
                    byte<'X'>
                >,
                plus<hex_digit>
            >
        );

        peg_rule(
            int_literal,
            choice<
                dec_int_literal,
                hex_int_literal
            >
        );

        // double_literal \\....................................................

        peg_rule(
            float_integer,
            choice<
                byte<'0'>,
                sequence<dec_digit_nonzero, star<dec_digit>>
            >
        );

        peg_rule(
            float_fraction,
            choice<
                sequence<
                    star<dec_digit>,
                    peg::ascii_grammar::dot,
                    plus<dec_digit>
                >,
                sequence<
                    plus<dec_digit>,
                    peg::ascii_grammar::dot
                >
            >
        );

        peg_rule(
            float_exponent_sign,
            choice<byte<'+'>, byte<'-'>>
        );

        peg_rule(
            float_exponent,
            sequence<
                choice<byte<'e'>, byte<'E'>>,
                opt<float_exponent_sign>,
                plus<dec_digit>
            >
        );

        peg_rule(
            float_literal,
            choice<
                sequence<
                    float_fraction,
                    opt<float_exponent>
                >,
                sequence<
                    float_integer,
                    float_exponent
                >
            >
        );

        // char_literal \\......................................................

        peg_rule(escaped_nul,       peg_literal(R"(\0)"));
        peg_rule(escaped_quote,     peg_literal(R"(\')"));
        peg_rule(escaped_dquote,    peg_literal(R"(\")"));
        peg_rule(escaped_question,  peg_literal(R"(\?)"));
        peg_rule(escaped_backslash, peg_literal(R"(\\)"));
        peg_rule(escaped_bell,      peg_literal(R"(\a)"));
        peg_rule(escaped_backspace, peg_literal(R"(\b)"));
        peg_rule(escaped_formfeed,  peg_literal(R"(\f)"));
        peg_rule(escaped_linefeed,  peg_literal(R"(\n)"));
        peg_rule(escaped_return,    peg_literal(R"(\r)"));
        peg_rule(escaped_tab,       peg_literal(R"(\t)"));
        peg_rule(escaped_vtab,      peg_literal(R"(\v)"));

        peg_rule(escaped_hex_head,   peg_literal("\\x"));
        peg_rule(escaped_hex_digits, plus<hex_digit>);
        peg_rule(escaped_hex,        seq<escaped_hex_head, escaped_hex_digits>);

        peg_rule(
            escape_sequence,
            choice<
                escaped_nul,
                escaped_quote,
                escaped_dquote,
                escaped_question,
                escaped_backslash,
                escaped_bell,
                escaped_backspace,
                escaped_formfeed,
                escaped_linefeed,
                escaped_return,
                escaped_tab,
                escaped_vtab,
                escaped_hex
            >
        );

        peg_error(
            error_newline_in_char_literal,
            "newline in character literal",
            peg::ascii_grammar::newline
        );

        peg_error(
            error_empty_char_literal,
            "empty character literal",
            peg::ascii_grammar::quote
        );

        peg_rule(
            char_literal_head,
            peg::ascii_grammar::quote
        );

        peg_rule(
            char_literal_tail,
            peg::ascii_grammar::quote
        );

        peg_rule(
            char_literal_body,
            choice<
                error_empty_char_literal,
                escape_sequence,
                peg::ascii_grammar::printable_char,
                error_unexpected_eof,
                error_newline_in_char_literal,
                error_unexpected_control_char
            >
        );

        peg_rule(
            char_literal,
            sequence<
                char_literal_head,
                char_literal_body,
                char_literal_tail
            >
        );

        // string_literal \\....................................................

        peg_error(
            error_newline_in_string_literal,
            "newline in string literal",
            peg::ascii_grammar::newline
        );

        peg_rule(
            string_literal_head,
            peg::ascii_grammar::doublequote
        );

        peg_rule(
            string_literal_tail,
            peg::ascii_grammar::doublequote
        );

        peg_rule(
            string_literal_body,
            star<sequence<
                not_predicate<string_literal_tail>,
                choice<
                    escape_sequence,
                    peg::ascii_grammar::printable_char,
                    error_unexpected_eof,
                    error_newline_in_string_literal,
                    error_unexpected_control_char
                >
            >>
        );

        peg_rule(
            string_literal,
            sequence<
                string_literal_head,
                string_literal_body,
                string_literal_tail
            >
        );

    };

} // namespace peg