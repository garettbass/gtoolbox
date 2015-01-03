#pragma once
#include <gtoolbox/peg/grammars/utf8_grammar.h>

namespace json {

    //--------------------------------------------------------------------------

    peg_grammar(json_string_grammar)
    {
        peg_rules(
            escaped_backslash,
            escaped_backspace,
            escaped_doublequote,
            escaped_formfeed,
            escaped_linefeed,
            escaped_return,
            escaped_slash,
            escaped_tab,
            escaped_utf16_head,
            escaped_utf16_hex_digit,
            escaped_utf16_hex_quad,
            escaped_utf16_codepoint,
            escaped_utf16_sequence,
            invalid_utf8_codepoint,
            valid_utf8_codepoint,
            string_character,
            string_enter,
            string_leave,
            string
        );

        // escape sequences \\..................................................

        peg_rule(escaped_backslash,   peg_literal(R"(\\)"));
        peg_rule(escaped_backspace,   peg_literal(R"(\b)"));
        peg_rule(escaped_doublequote, peg_literal(R"(\")"));
        peg_rule(escaped_formfeed,    peg_literal(R"(\f)"));
        peg_rule(escaped_linefeed,    peg_literal(R"(\n)"));
        peg_rule(escaped_return,      peg_literal(R"(\r)"));
        peg_rule(escaped_slash,       peg_literal(R"(\/)"));
        peg_rule(escaped_tab,         peg_literal(R"(\t)"));
        peg_rule(escaped_utf16_head,  peg_literal(R"(\u)"));

        peg_rule(
            escaped_utf16_hex_digit,
            choice<
                range<'0','9'>,
                range<'a','f'>,
                range<'A','F'>
            >
        );

        peg_rule(
            escaped_utf16_hex_quad,
            rep<escaped_utf16_hex_digit, 4>
        );

        
        peg_rule(
            escaped_utf16_codepoint,
            sequence<escaped_utf16_head, escaped_utf16_hex_quad>
        );

        peg_rule(
            escaped_utf16_sequence,
            plus<escaped_utf16_codepoint>
        );

        // characters \\........................................................

        peg_rule(
            invalid_utf8_codepoint,
            choice<
                peg::ascii_grammar::backslash,
                peg::ascii_grammar::doublequote,
                peg::ascii_grammar::null_char
            >
        );

        peg_rule(
            valid_utf8_codepoint,
            sequence<
                not_predicate<invalid_utf8_codepoint>,
                peg::utf8_grammar::utf8_codepoint
            >
        );
        
        peg_rule(
            string_character,
            choice<
                escaped_backslash,
                escaped_backspace,
                escaped_doublequote,
                escaped_formfeed,
                escaped_linefeed,
                escaped_return,
                escaped_slash,
                escaped_tab,
                escaped_utf16_sequence,
                valid_utf8_codepoint
            >
        );

        // strings \\...........................................................

        peg_rule(
            string_enter,
            peg::ascii_grammar::doublequote
        );

        peg_error(
            error_invalid_string_character,
            "invalid character in string: '%s'",
            not_predicate<peg::ascii_grammar::doublequote>
        );

        peg_rule(
            string_leave,
            choice<
                peg::ascii_grammar::doublequote,
                error_invalid_string_character
            >
        );

        peg_rule(
            string,
            sequence<
                string_enter,
                star<string_character>,
                string_leave
            >
        );
    };

    //--------------------------------------------------------------------------

} // namespace json
