#pragma once
#include "../grammar.h"

namespace peg {

    //--------------------------------------------------------------------------
    
    peg_grammar(ascii_grammar)
    {
        peg_rules(
            ascii_char,
            control_char,
            printable_char,
            backspace,
            carriage_return,
            line_feed,
            null_char,
            ampersand,
            asterisk,
            angle_left,
            angle_right,
            at_sign,
            backquote,
            backslash,
            brace_left,
            brace_right,
            bracket_left,
            bracket_right,
            caret,
            colon,
            comma,
            dollar_sign,
            doublequote,
            dot,
            equal_sign,
            exclamation_mark,
            hyphen,
            minus_sign,
            number_sign,
            percent_sign,
            period,
            paren_left,
            paren_right,
            plus_sign,
            question_mark,
            quote,
            semicolon,
            slash,
            space,
            tab,
            tilde,
            underscore,
            vertical_bar,
            newline,
            whitespace
        );

        // categories \\........................................................

        peg_rule(ascii_char, range<0x00, 0x7F>); // 0000 0000 .. 0111 1111

        peg_rule(control_char, choice<range<0x00, 0x1F>, byte<0x7F>>);

        peg_rule(printable_char, choice<byte<'\t'>, range<' ','~'>>);

        // control characters \\................................................
        
        peg_rule(backspace,         byte<'\b'>);
        peg_rule(carriage_return,   byte<'\r'>);
        peg_rule(line_feed,         byte<'\n'>);
        peg_rule(null_char,         byte<0x00>);
        
        // symbols \\...........................................................
        
        peg_rule(ampersand,         byte<'&'>);
        peg_rule(asterisk,          byte<'*'>);
        peg_rule(angle_left,        byte<'<'>);
        peg_rule(angle_right,       byte<'>'>);
        peg_rule(at_sign,           byte<'@'>);
        peg_rule(backquote,         byte<'`'>);
        peg_rule(backslash,         byte<'\\'>);
        peg_rule(brace_left,        byte<'{'>);
        peg_rule(brace_right,       byte<'}'>);
        peg_rule(bracket_left,      byte<'['>);
        peg_rule(bracket_right,     byte<']'>);
        peg_rule(caret,             byte<'^'>);
        peg_rule(colon,             byte<':'>);
        peg_rule(comma,             byte<','>);
        peg_rule(dollar_sign,       byte<'$'>);
        peg_rule(doublequote,       byte<'"'>);
        peg_rule(dot,               byte<'.'>);
        peg_rule(equal_sign,        byte<'='>);
        peg_rule(exclamation_mark,  byte<'!'>);
        peg_rule(hyphen,            byte<'-'>);
        peg_rule(minus_sign,        byte<'-'>);
        peg_rule(number_sign,       byte<'#'>);
        peg_rule(percent_sign,      byte<'%'>);
        peg_rule(period,            byte<'.'>);
        peg_rule(paren_left,        byte<'('>);
        peg_rule(paren_right,       byte<')'>);
        peg_rule(plus_sign,         byte<'+'>);
        peg_rule(question_mark,     byte<'?'>);
        peg_rule(quote,             byte<'\''>);
        peg_rule(semicolon,         byte<';'>);
        peg_rule(slash,             byte<'/'>);
        peg_rule(space,             byte<' '>);
        peg_rule(tab,               byte<'\t'>);
        peg_rule(tilde,             byte<'~'>);
        peg_rule(underscore,        byte<'_'>);
        peg_rule(vertical_bar,      byte<'|'>);
        
        // whitespace \\........................................................

        peg_rule(
            newline,
            choice<
                line_feed,
                sequence<carriage_return, opt<line_feed>>
            >
        );
        
        peg_rule(whitespace, choice<space, tab, newline>);

        peg_error(
            error_unexpected_eof,
            "unexpected end of file",
            eof
        );

        peg_error(
            error_unexpected_control_char,
            "unexpected control character",
            control_char
        );

        peg_error(
            error_unexpected_utf8_sequence,
            "unexpected UTF8 sequence",
            range<0x80, 0xFF>
        );
    };

    //--------------------------------------------------------------------------

} // namespace peg