#pragma once
#include <gtoolbox/xtd/traits.h>
#include "./choice.h"
#include "./range.h"
#include "../inline.h"
#pragma clang diagnostic ignored "-Wmultichar"

namespace peg {

    template <char Char>
    using char_class_char_literal = byte<uint8_t(Char)>;

    template <char Min, char Max>
    using char_class_char_range = range<uint8_t(Min), uint8_t(Max)>;

    template <char A, char B, char C>
    class char_class_char_invalid {
        static_assert(
            (A and not A) and
            (B and not B) and
            (C and not C),
            "invalid char_class character literal");
    };

    template <unsigned Char>
    using char_class_char =
        xtd::conditional<
            (Char <= '~'),
            char_class_char_literal<Char & 0xFF>,
        xtd::conditional<
            ((Char >> 8) & 0xFF) == '-',
            char_class_char_range<
                ((Char >> 16) & 0xFF),
                ((Char >>  0) & 0xFF)
            >,
            char_class_char_invalid<
                ((Char >> 16) & 0xFF),
                ((Char >>  8) & 0xFF),
                ((Char >>  0) & 0xFF)
            >
        >>;

    template <unsigned... Chars>
    using char_class = choice<char_class_char<Chars>...>;

};