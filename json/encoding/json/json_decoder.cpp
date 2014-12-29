#include <zna/source/thirdParty/gtoolbox/xtd/text/utf8_codepoint.h>
#include <zna/source/thirdParty/gtoolbox/peg/utility.h>
#include "./json_decoder.h"
#include "./json_grammar_parser.h"
#include "./json_fast_parser.h"

namespace json {

    //==========================================================================

    json_decoder::json_decoder(std::istream& s)
    {
        assert(s);

        s.seekg(0, std::ios::end);

        const size_t length = s.tellg();

        _buffer.resize(length + 1, '\0');

        s.seekg(0, std::ios::beg);

        s.read(&_buffer[0], _buffer.size());

        _chars = _buffer.data();

        s.seekg(0, std::ios::beg);
    }

    //--------------------------------------------------------------------------

    bool json_decoder::decode(json::encoder& encoder)
    {
    #if 1
        if (json_fast_parser(encoder).parse(_chars))
            return true;

        json_grammar_parser().parse(_chars);
    #else
        json_grammar_parser(encoder).parse(_chars);
    #endif

        return false;
    }

    //==========================================================================

} // namespace json
