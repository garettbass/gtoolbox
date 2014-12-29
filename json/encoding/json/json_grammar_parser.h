#pragma once
#include "./json_grammar.h"

namespace json {

    //==========================================================================

    class json_grammar_parser : public peg::parser
    {
        json::encoder* encoder;
        
        std::vector<char> utf8_buffer;

        std::vector<char16_t> utf16_buffer;

    public: // structors

        json_grammar_parser(json::encoder& e = null_encoder::instance())
        : encoder(&e)
        {}

    public: // methods

        bool parse(const char* s)
        {
            return peg::lex<json_grammar::document>(*this, s);
        }

    public: // methods
    
        using parser::parse;

        void parse(const json_grammar::null_keyword&)
        {
            encoder->encode_null();
        }

        void parse(const json_grammar::false_keyword&)
        {
            encoder->encode_boolean(false);
        }

        void parse(const json_grammar::true_keyword&)
        {
            encoder->encode_boolean(true);
        }

        void parse(const json_grammar::number& number)
        {
            encoder->encode_number(atof(number.data()));
        }

        void parse(const json_string_grammar::escaped_backslash&)
        {
            utf8_buffer.push_back('\\');
        }
        
        void parse(const json_string_grammar::escaped_backspace&)
        {
            utf8_buffer.push_back('\b');
        }
        
        void parse(const json_string_grammar::escaped_doublequote&)
        {
            utf8_buffer.push_back('\"');
        }
        
        void parse(const json_string_grammar::escaped_formfeed&)
        {
            utf8_buffer.push_back('\f');
        }
        
        void parse(const json_string_grammar::escaped_linefeed&)
        {
            utf8_buffer.push_back('\n');
        }
        
        void parse(const json_string_grammar::escaped_return&)
        {
            utf8_buffer.push_back('\r');
        }
        
        void parse(const json_string_grammar::escaped_slash&)
        {
            utf8_buffer.push_back('/');
        }
        
        void parse(const json_string_grammar::escaped_tab&)
        {
            utf8_buffer.push_back('\t');
        }


        void parse(const json_string_grammar::escaped_utf16_hex_quad& lexeme)
        {
            assert(lexeme.size() == 4);

            char16_t word = hex_to_int(lexeme[0]);

            word = (word << 4) + hex_to_int(lexeme[1]);
            word = (word << 4) + hex_to_int(lexeme[2]);
            word = (word << 4) + hex_to_int(lexeme[3]);

            utf16_buffer.push_back(word);
        }

        void parse(const json_string_grammar::escaped_utf16_sequence&)
        {
            utf16_buffer.push_back(0);

            const char16_t* utf16 = utf16_buffer.data();

            xtd::utf8_codepoint utf8;

            while (*utf16)
            {
                if (utf8(utf16))
                {
                    for (char c : utf8) utf8_buffer.push_back(c);

                    continue;
                }
                assert(not "legible UTF8 sequence");
            }

            utf16_buffer.clear();
        }
        
        void parse(const json_string_grammar::valid_utf8_codepoint& lexeme)
        {
            for (char byte : lexeme)
            {
                utf8_buffer.push_back(byte);
            }
        }

        void parse(const json_string_grammar::string_enter&)
        {
            utf8_buffer.clear();
        }

        void parse(const json_string_grammar::string_leave&)
        {
            utf8_buffer.push_back('\0');
        }

        void parse(const json_grammar::string&)
        {
            encoder->encode_string(utf8_buffer.data());
        }

        void parse(const json_grammar::array_enter&)
        {
            encoder->array_enter();
        }

        void parse(const json_grammar::array_leave&)
        {
            encoder->array_leave();
        }

        void parse(const json_grammar::object_enter&)
        {
            encoder->object_enter();
        }

        void parse(const json_grammar::object_leave&)
        {
            encoder->object_leave();
        }
        
        void parse(const json_grammar::property_name&)
        {
            encoder->property_enter(utf8_buffer.data());
        }
        
        void parse(const json_grammar::property&)
        {
            encoder->property_leave();
        }

    private: // utilities

        env_forceinline
        static constexpr char16_t _hex_to_int(uint8_t u)
        {
            return ((u >> 6) * 9) + (u & 0xF);
        }

        env_forceinline
        static char16_t hex_to_int(uint8_t u)
        {
            env_static_assert(_hex_to_int('0') == 0);
            env_static_assert(_hex_to_int('1') == 1);
            env_static_assert(_hex_to_int('2') == 2);
            env_static_assert(_hex_to_int('3') == 3);
            env_static_assert(_hex_to_int('4') == 4);
            env_static_assert(_hex_to_int('5') == 5);
            env_static_assert(_hex_to_int('6') == 6);
            env_static_assert(_hex_to_int('7') == 7);
            env_static_assert(_hex_to_int('8') == 8);
            env_static_assert(_hex_to_int('9') == 9);

            env_static_assert(_hex_to_int('A') == 10);
            env_static_assert(_hex_to_int('B') == 11);
            env_static_assert(_hex_to_int('C') == 12);
            env_static_assert(_hex_to_int('D') == 13);
            env_static_assert(_hex_to_int('E') == 14);
            env_static_assert(_hex_to_int('F') == 15);

            env_static_assert(_hex_to_int('a') == 10);
            env_static_assert(_hex_to_int('b') == 11);
            env_static_assert(_hex_to_int('c') == 12);
            env_static_assert(_hex_to_int('d') == 13);
            env_static_assert(_hex_to_int('e') == 14);
            env_static_assert(_hex_to_int('f') == 15);

            return _hex_to_int(u);
        };

    };

    //==========================================================================

} // namespace json
