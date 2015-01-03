#pragma once
#include <iostream>
#include <gtoolbox/xtd/substring.h>

namespace peg {

    template <typename, typename, typename>
    struct rule;

    //--------------------------------------------------------------------------
    
    template <typename Error, typename Expr>
    struct error;
    
    template <typename Warning, typename Expr>
    struct warning;
    
    struct parser
    {
        size_t errors = 0;

        char lexing(...) { return 0; }
        void accepted(...) {}
        void rejected(...) {}

        void parse(...) {}

        template <typename Error, typename Expr>
        void parse(const error<Error, Expr>& err)
        {
            if (errors)
                return;
                
            ++errors;

            std::string err_string(err.data(), err.size());

            printf(
                "\nerror @ %zu:%zu: ",
                err.line(),
                err.column());

            printf(
                Error::message(),
                err_string.c_str());

            puts("");
        }
        
        template <typename Warning, typename Expr>
        void parse(const warning<Warning, Expr>& warn)
        {
            if (errors)
                return;

            std::string warn_string(warn.data(), warn.size());

            printf(
                "\nwarning @ %zu:%zu: ",
                warn.line(),
                warn.column());

            printf(
                Warning::message(),
                warn_string.c_str());

            puts("");
        }
    };

    //--------------------------------------------------------------------------

    struct debug_parser
    : peg::parser
    {
        using peg::parser::parse;

        template <typename G, typename R, typename E>
        void parse(const peg::rule<G, R, E>& lexeme)
        {
            std::cout
                << G::name() << "." << lexeme.name()
                << " @ (" << lexeme.line() << ':' << lexeme.column() << ")\n\""
                << lexeme << "\"\n\n";
        }
    };

    //--------------------------------------------------------------------------

} // namespace peg