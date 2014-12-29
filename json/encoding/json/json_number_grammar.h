#pragma once
#include <zna/source/thirdParty/gtoolbox/peg/grammar.h>

namespace json {

    //--------------------------------------------------------------------------

    peg_grammar(json_number_grammar)
    {
        peg_rules(
            digit_0,
            digit_0_9,
            digit_1_9,
            positive_integer,
            cardinal_integer,
            integer_sign,
            integer,
            exponent_separator,
            exponent_sign,
            exponent,
            fraction_separator,
            fraction,
            number
        );

        peg_rule(digit_0,   byte<'0'>);
        peg_rule(digit_0_9, range<'0','9'>);
        peg_rule(digit_1_9, range<'1','9'>);

        peg_rule(positive_integer, sequence<digit_1_9, star<digit_0_9>>);

        peg_rule(cardinal_integer, choice<digit_0, positive_integer>);

        peg_rule(integer_sign, byte<'-'>);

        peg_rule(integer, sequence<opt<integer_sign>, cardinal_integer>);

        peg_rule(exponent_separator, byte<'e', 'E'>);

        peg_rule(exponent_sign, byte<'-', '+'>);

        peg_rule(exponent, sequence<opt<exponent_sign>, plus<digit_0_9>>);

        peg_rule(fraction_separator, byte<'.'>);
        
        peg_rule(fraction, plus<digit_0_9>);

        peg_rule(
            number,
            sequence<
                integer,
                opt<sequence<fraction_separator, fraction>>,
                opt<sequence<exponent_separator, exponent>>
            >
        );
    };

    //--------------------------------------------------------------------------

} // namespace json
