#pragma once
#include <vector>
#include "./expressions.h"
#include "./parsers/expression_count_parser.h"

namespace peg {

    //--------------------------------------------------------------------------

    template <typename Expr>
    inline bool starts_with(const char* s)
    {
        parser p;
        
        cursor t(s);
        
        return Expr::lex(p, t);
    }

    template <typename Expr>
    inline bool is_match(const char* s)
    {
        parser p;
        
        cursor t(s);
        
        const bool accepted = Expr::lex(p, t);

        return accepted and (*t == '\0');
    }

    template <typename Expr>
    inline bool is(const char* s)
    {
        return is_match<Expr>(s);
    }

    template <typename Expr>
    inline bool is_not(const char* s)
    {
        return not is_match<Expr>(s);
    }

    template <typename Expr, typename Error>
    inline bool has_error(const char* s)
    {
        expression_count_parser<Error> p;
        
        cursor t(s);
        
        Expr::lex(p, t);

        return p.count > 0;
    }

    template <typename Expr, typename Parser>
    inline bool lex(Parser& p, const char* s)
    {
        cursor t(s);
    
        return Expr::lex(p, t);
    }

    template <typename Expr>
    struct find_all final
    {
        template <typename StartExpr>
        static std::vector<Expr> in(const char* s)
        {
            struct find_parser : parser
            {
                std::vector<Expr> exprs;

                void parse(...) {}

                void parse(const Expr& e)
                {
                    this->exprs.push_back(e);
                }
            };

            find_parser p;

            cursor t(s);

            StartExpr::lex(p, t);

            return std::move(p.exprs);
        }
    };

    template <typename Expr>
    struct find_first final
    {
        template <typename StartExpr>
        static Expr in(const char* s)
        {
            struct find_parser : debug_parser
            {
                Expr expr;

                bool found = false;

                void parse(...) {}

                void parse(const Expr& e)
                {
                    if (not this->found)
                    {
                        this->found = true;
                        this->expr = e;
                    }
                }
            };

            find_parser p;

            cursor t(s);

            StartExpr::lex(p, t);

            return p.expr;
        }
    };

    //--------------------------------------------------------------------------

    class indent
    {
        std::string _indent;

        size_t _strlen;

        size_t _depth = 0;

    public: // structors

        explicit
        indent(const char* str = "    ")
        : _indent(str)
        , _strlen(_indent.size())
        {}

    public: // operators

        indent& operator ++ () {
            ++_depth;
            assert(_depth);
            _indent.append(_indent, 0, _strlen);
            return *this;
        }

        const char* operator ++ (int) {
            return operator++().c_str() + _strlen;
        }

        indent& operator -- () {
            assert(_depth);
            --_depth;
            _indent.resize((1 + _depth) * _strlen);
            return *this;
        }

        const char* operator -- (int) {
            return operator--().c_str() - _strlen;
        }

    public: // properties

        size_t depth() const { return _depth; }

        void depth(const size_t value)
        {
            while (_depth > value) operator--();
            while (_depth < value) operator++();
        }

        const char* c_str() const { return _indent.c_str() + _strlen; }

        size_t size() const { return _indent.size() - _strlen; }
    };

    template <typename Stream>
    auto operator << (Stream& s, const indent& i) -> decltype(s << 0)
    {
        const char* c_str = i.c_str();
        const size_t size = i.size();

        assert(size % 4 == 0);

        return s.write(c_str, size);
    }

    //--------------------------------------------------------------------------

} // namespace peg