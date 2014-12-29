#pragma once
#include <zna/source/thirdParty/gtoolbox/xtd/text/utf8_codepoint.h>
#include "../inline.h"

namespace peg {

    //--------------------------------------------------------------------------

    struct dot : lexeme
    {
        using lexeme::lexeme;

        template <typename Parser>
        peg_inline
        static bool lex(const Parser&, cursor& s)
        {
            xtd::utf8_codepoint utf8;

            const char* p = s;

            if (utf8(p))
            {
                s.advance(utf8.size());

                return true;
            }

            return false;
        }
    };

    //--------------------------------------------------------------------------

} // namespace peg