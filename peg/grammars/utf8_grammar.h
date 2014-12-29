#pragma once
#include "./ascii_grammar.h"

namespace peg {

    //--------------------------------------------------------------------------
    
    peg_grammar(utf8_grammar)
    {
        peg_rules(
            utf8_head1,
            utf8_head2,
            utf8_head3,
            utf8_head4,
            utf8_tail,
            utf8_codepoint
        );

        peg_rule(utf8_head1, peg::ascii_grammar::ascii_char);
        
        peg_rule(utf8_head2, range<0xC0, 0xDF>); // 1100 0000 .. 1101 1111
        
        peg_rule(utf8_head3, range<0xE0, 0xEF>); // 1110 0000 .. 1110 1111
        
        peg_rule(utf8_head4, range<0xF0, 0xF7>); // 1111 0000 .. 1111 0111
        
        peg_rule(utf8_tail,  range<0x80, 0xBF>); // 1000 0000 .. 1011 1111
        
        peg_rule(
            utf8_codepoint,
            choice<
                utf8_head1,
                sequence<utf8_head2, rep<utf8_tail, 1>>,
                sequence<utf8_head3, rep<utf8_tail, 2>>,
                sequence<utf8_head4, rep<utf8_tail, 3>>
            >
        );
    };

    //--------------------------------------------------------------------------

} // namespace peg