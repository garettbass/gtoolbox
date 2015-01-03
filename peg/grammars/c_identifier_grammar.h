#pragma once
#include <gtoolbox/peg/grammars/ascii_grammar.h>

namespace peg {

    peg_grammar(c_identifier_grammar)
    {
        peg_rules(
            identifier
        );

        peg_rule(
            identifier,
            seq<
                char_class<'_','a-z','A-Z'>,
                star<char_class<'_','a-z','A-Z','0-9'>>
            >
        );
    };

} // namespace peg