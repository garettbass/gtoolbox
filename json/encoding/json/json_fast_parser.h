#pragma once

namespace json {

    //==========================================================================

    class json_fast_parser
    {
        json::encoder* encoder;
        
        std::vector<char> utf8_buffer;

        std::vector<char16_t> utf16_buffer;

    public: // structors

        json_fast_parser(json::encoder& e = null_encoder::instance())
        : encoder(&e)
        {
            utf8_buffer.reserve(256);
            utf16_buffer.reserve(64);
        }

    public: // methods

        bool parse(const char* s)
        {
            skip_whitespace(s);

            return parse_value(s);
        }
        
    private: // methods

        bool parse_value(const char*& s)
        {
            switch (const int c = *s)
            {
                case int('n'): return parse_null(s);
                case int('f'): return parse_false(s);
                case int('t'): return parse_true(s);
                default:       return parse_number(s);
                case int('"'): return parse_string(s);
                case int('['): return parse_array(s);
                case int('{'): return parse_object(s);
            }
            assert(false);
        }

        bool parse_null(const char*& s)
        {
            if (skip_literal(s, "null"))
            {
                encoder->encode_null();

                return true;
            }
            return false;
        }

        bool parse_false(const char*& s)
        {
            if (skip_literal(s, "false"))
            {
                encoder->encode_boolean(false);

                return true;
            }
            return false;
        }

        bool parse_true(const char*& s)
        {
            if (skip_literal(s, "true"))
            {
                encoder->encode_boolean(true);

                return true;
            }
            return false;
        }

        bool parse_number(const char*& s)
        {
            char* end = nullptr;

            double value = strtod(s, &end);

            if (size_t(end) == size_t(s))
                return false;

            encoder->encode_number(value);

            s = end;

            return true;
        }

        bool parse_string(const char*& s)
        {
            if (not parse_string_into_utf8_buffer(s))
                return false;

            encoder->encode_string(utf8_buffer.data());

            utf8_buffer.clear();

            return true;
        }

        bool parse_string_into_utf8_buffer(const char*& s)
        {
            if (not skip_char<'"'>(s))
                return false;

            for (char c = *s; c != '"'; c = *s)
            {
                switch (c)
                {
                    case '\0':
                        return false;

                    case '\\':
                        if (parse_string_escape_sequence(s))
                            continue;

                        return false;

                    default:
                        utf8_buffer.push_back(c); ++s;

                        continue;
                }
            }

            if (not skip_char<'"'>(s))
                return false;

            utf8_buffer.push_back('\0');

            return true;
        }

        bool parse_string_escape_sequence(const char*& s)
        {
            assert(s[0] == '\\');

            switch (s[1])
            {
                case '\\': utf8_buffer.push_back('\\'); s += 2; return true;
                case  '"': utf8_buffer.push_back('"');  s += 2; return true;
                case  '/': utf8_buffer.push_back('/');  s += 2; return true;
                case  'b': utf8_buffer.push_back('\b'); s += 2; return true;
                case  'f': utf8_buffer.push_back('\f'); s += 2; return true;
                case  'n': utf8_buffer.push_back('\n'); s += 2; return true;
                case  'r': utf8_buffer.push_back('\r'); s += 2; return true;
                case  't': utf8_buffer.push_back('\t'); s += 2; return true;
                case  'u': return parse_string_escaped_utf16(s);
            }

            return false;
        }

        bool parse_string_escaped_utf16(const char*& s)
        {
            while (s[0] == '\\' and s[1] == 'u')
            {
                char16_t word = hex_to_int(s[2]);

                word = (word << 4) + hex_to_int(s[3]);
                word = (word << 4) + hex_to_int(s[4]);
                word = (word << 4) + hex_to_int(s[5]);

                s += 6;

                utf16_buffer.push_back(word);
            }

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

            return true;
        }

        bool parse_array(const char*& s)
        {
            if (not skip_char<'['>(s))
                return false;

            encoder->array_enter();

            skip_whitespace(s);

            while (*s != ']' and parse_value(s))
            {
                skip_whitespace(s);

                skip_char<','>(s);

                skip_whitespace(s);
            }

            if (not skip_char<']'>(s))
                return false;

            encoder->array_leave();

            return true;
        }

        bool parse_object(const char*& s)
        {
            if (not skip_char<'{'>(s))
                return false;

            encoder->object_enter();

            skip_whitespace(s);

            while (*s != '}' and parse_property(s))
            {
                skip_whitespace(s);

                skip_char<','>(s);

                skip_whitespace(s);
            }

            if (not skip_char<'}'>(s))
                return false;

            encoder->object_leave();

            return true;
        }

        bool parse_property(const char*& s)
        {
            skip_whitespace(s);

            if (not parse_string_into_utf8_buffer(s))
                return false;

            encoder->property_enter(utf8_buffer.data());

            utf8_buffer.clear();

            skip_whitespace(s);

            if (not skip_char<':'>(s))
                return false;

            skip_whitespace(s);

            if (not parse_value(s))
                return false;

            encoder->property_leave();

            return true;
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

            assert(is_hex(u));

            return _hex_to_int(u);
        };

        template <typename T>
        env_forceinline
        static int is_digit(T c)
        {
            return (c >= T('0') & c <= T('9'));
        }

        env_forceinline
        static int is_hex(const int c)
        {
            return (c >= int('0') & c <= int('9'))
                 | (c >= int('A') & c <= int('F'))
                 | (c >= int('a') & c <= int('f'));
        }

        env_forceinline
        static int is_whitespace(const int c)
        {
            enum : int { HT = '\t', LF = '\n', CR = '\r', SP = ' ' };

            return (c == SP | c == HT | c == LF | c == CR);
        }

        env_forceinline
        static void skip_whitespace(const char*& s)
        {
            while (is_whitespace(*s)) ++s;
        }

        template <char CHAR>
        env_forceinline
        static bool skip_char(const char*& s)
        {
            if (s[0] == CHAR) { ++s; return true; }

            return false;
        }

        env_forceinline
        static bool skip_literal(const char*& s, const char* literal)
        {
            const char* l = literal;

            while (*s and *l and *s == *l) { ++s; ++l; }

            return *l == '\0';
        }

    };
    
    //==========================================================================

} // namespace json
