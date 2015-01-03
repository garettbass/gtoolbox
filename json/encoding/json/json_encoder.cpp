#include <gtoolbox/xtd/utility/lookup_table.h>
#include "./json_encoder.h"
#include <iomanip>
#include <iostream>
#include <cmath>

namespace json {

    //--------------------------------------------------------------------------
        
    json_encoder::json_encoder(std::ostream& stream, config config)
    : _stream(stream)
    , _config(config)
    {}

    //--------------------------------------------------------------------------
    
    void json_encoder::encode_null()
    {
        emit_comma();
        emit_indent();

        _stream << "null";
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    void json_encoder::encode_boolean(bool value)
    {
        emit_comma();
        emit_indent();

        _stream << (value ? "true" : "false");
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    void json_encoder::encode_number(double value)
    {
        emit_comma();
        emit_indent();

        double intPart;
        if (std::modf(value, &intPart) == 0.0)
        {
            // write without .0000000
            _stream << std::fixed << std::setprecision(0) << intPart;
        }
        else
        {
            _stream << std::fixed << std::setprecision(7) << value;
        }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    union utf8_buffer
    {
        uint64_t block;
        uint8_t  bytes[8];
        char     chars[8];
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    static const char* encode_char(const char*& c, utf8_buffer& buffer)
    {
        static xtd::lookup_table<const char*, 128> escape_table {
            { '\x00', R"(\u0000)" },
            { '\x01', R"(\u0001)" },
            { '\x02', R"(\u0002)" },
            { '\x03', R"(\u0003)" },
            { '\x04', R"(\u0004)" },
            { '\x05', R"(\u0005)" },
            { '\x06', R"(\u0006)" },
            { '\x07', R"(\u0007)" },
            { '\x08', R"(\b)"     },
            { '\x09', R"(\t)"     },
            { '\x0A', R"(\n)"     },
            { '\x0B', R"(\u000B)" },
            { '\x0C', R"(\f)"     },
            { '\x0D', R"(\r)"     },
            { '\x0E', R"(\u000E)" },
            { '\x0F', R"(\u000F)" },
            { '\x10', R"(\u0010)" },
            { '\x11', R"(\u0011)" },
            { '\x12', R"(\u0012)" },
            { '\x13', R"(\u0013)" },
            { '\x14', R"(\u0014)" },
            { '\x15', R"(\u0015)" },
            { '\x16', R"(\u0016)" },
            { '\x17', R"(\u0017)" },
            { '\x18', R"(\u0018)" },
            { '\x19', R"(\u0019)" },
            { '\x1A', R"(\u001A)" },
            { '\x1B', R"(\u001B)" },
            { '\x1C', R"(\u001C)" },
            { '\x1D', R"(\u001D)" },
            { '\x1E', R"(\u001E)" },
            { '\x1F', R"(\u001F)" },
            { '\"',   R"(\")"     },
            { '\\',   R"(\\)"     },
            { '\x7F', R"(\u007F)" },

//            { '\x80', R"(\u0080)" },
//            { '\x81', R"(\u0081)" },
//            { '\x82', R"(\u0082)" },
//            { '\x83', R"(\u0083)" },
//            { '\x84', R"(\u0084)" },
//            { '\x85', R"(\u0085)" },
//            { '\x86', R"(\u0086)" },
//            { '\x87', R"(\u0087)" },
//            { '\x88', R"(\u0088)" },
//            { '\x89', R"(\u0089)" },
//            { '\x8A', R"(\u008A)" },
//            { '\x8B', R"(\u008B)" },
//            { '\x8C', R"(\u008C)" },
//            { '\x8D', R"(\u008D)" },
//            { '\x8E', R"(\u008E)" },
//            { '\x8F', R"(\u008F)" },
//
//            { '\x90', R"(\u0090)" },
//            { '\x91', R"(\u0091)" },
//            { '\x92', R"(\u0092)" },
//            { '\x93', R"(\u0093)" },
//            { '\x94', R"(\u0094)" },
//            { '\x95', R"(\u0095)" },
//            { '\x96', R"(\u0096)" },
//            { '\x97', R"(\u0097)" },
//            { '\x98', R"(\u0098)" },
//            { '\x99', R"(\u0099)" },
//            { '\x9A', R"(\u009A)" },
//            { '\x9B', R"(\u009B)" },
//            { '\x9C', R"(\u009C)" },
//            { '\x9D', R"(\u009D)" },
//            { '\x9E', R"(\u009E)" },
//            { '\x9F', R"(\u009F)" },
//
//            { '\xA0', R"(\u00A0)" },
        };

        const size_t char_index(0xFF & *c);

        const size_t high_bit(char_index >> 7);

        const size_t is_ascii = not high_bit;

        if (is_ascii)
        {
            const size_t escape_index(char_index * is_ascii);

            if (const char* escape_seq = escape_table[escape_index])
            {
                c += 1;

                return escape_seq;
            }

            buffer.block = 0;

            buffer.chars[0] = c[0];

            c += 1;

            return buffer.chars;
        }

        buffer.block = 0;

        if ((char_index & 0xE0) == 0xC0)
        {
            buffer.chars[0] = c[0];
            buffer.chars[1] = c[1];

            c += 2;

            return buffer.chars;
        }

        if ((char_index & 0xF0) == 0xE0)
        {
            buffer.chars[0] = c[0];
            buffer.chars[1] = c[1];
            buffer.chars[2] = c[2];

            c += 3;

            return buffer.chars;
        }

        if ((char_index & 0xF8) == 0xF0)
        {
            buffer.chars[0] = c[0];
            buffer.chars[1] = c[1];
            buffer.chars[2] = c[2];
            buffer.chars[3] = c[3];

            c += 4;

            return buffer.chars;
        }

        c += 1;

        return "";
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void json_encoder::encode_string(substring value)
    {
        emit_comma();
        emit_indent();

        _stream << '"';

        utf8_buffer buffer;

        const char* itr = value.begin();
        const char* end = value.end();

        while (*itr and itr != end)
        {
            const char* sequence = encode_char(itr, buffer);

            _stream.write(sequence, strlen(sequence));
        }

        _stream << '"';
    }

    //--------------------------------------------------------------------------
    
    void json_encoder::array_enter()
    {
        emit_comma();
        emit_indent();

        _stream << "[";

        _emit_next_comma = false;

        _indent_depth += 1;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void json_encoder::array_leave()
    {
        _indent_depth -= 1;

        _emit_next_comma = true;

        emit_indent();

        _stream << "]";
    }

    //--------------------------------------------------------------------------
    
    void json_encoder::object_enter()
    {
        emit_comma();
        emit_indent();

        _stream << "{";

        _emit_next_comma = false;

        _indent_depth += 1;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void json_encoder::object_leave()
    {
        _indent_depth -= 1;

        _emit_next_comma = true;

        emit_indent();

        _stream << "}";
    }

    //--------------------------------------------------------------------------
    
    void json_encoder::property_enter(substring key)
    {
        encode_string(key);

        _stream << ":" << _config.space;

        _emit_next_comma = false;

        _emit_next_indent = false;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void json_encoder::property_leave() {}

    //--------------------------------------------------------------------------

    void json_encoder::emit_comma()
    {
        if (_emit_next_comma)
        {
            _stream << ",";
        }

        _emit_next_comma = true;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void json_encoder::emit_indent()
    {
        if (_emit_next_indent)
        {
            _stream << _config.newline;
            
            for (size_t i = 0; i < _indent_depth; ++i)
            {
                _stream << _config.indent;
            }
        }

        _emit_next_indent = true;
    }

    //--------------------------------------------------------------------------

} // namespace json