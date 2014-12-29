#pragma once
#include <zna/source/thirdParty/gtoolbox/peg/ast.h>
#include <zna/source/thirdParty/gtoolbox/peg/grammars/peg_grammar.h>
#include <zna/source/thirdParty/gtoolbox/peg/utility.h>
#include <sstream>

namespace peg {

    static void translate_expr(
        std::ostream&                  out,
        peg::indent&                   indent,
        const peg_grammar::ast::entry& expr_entry)
    {
        using rule = typename peg_grammar::ast::rule;

        const char* head = "???";

        switch (expr_entry.rule())
        {
            case rule::CharLiteral:
                out << indent << "peg_char(" << expr_entry.lexeme() << ")";
                return;
            case rule::StringLiteral:
                out << indent << "peg_literal(" << expr_entry.lexeme() << ")";
                return;
            case rule::RuleReference:
                out << indent << expr_entry.lexeme();
                return;
            case rule::CharClassRange:
            case rule::CharClassSingle:
                out << indent << "'" << expr_entry.lexeme() << "'";
                return;

            case rule::Dot:
                out << indent << "dot";
                return;

            case rule::Choice:
                head = "choice";
                break;
            case rule::Sequence:
                head = "seq";
                break;
            case rule::AndPredicate:
                head = "and_predicate";
                break;
            case rule::NotPredicate:
                head = "not_predicate";
                break;
            case rule::ZeroOrOne:
                head = "opt";
                break;
            case rule::ZeroOrMore:
                head = "star";
                break;
            case rule::OneOrMore:
                head = "plus";
                break;
            case rule::CharClass:
                head = "char_class";
                break;
            default: assert(false);
        }

        out << indent++ << head << "<\n";

        auto itr = expr_entry.begin();
        auto end = expr_entry.end();

        translate_expr(out, indent, *itr);

        while (++itr != end) {
            out << ",\n";
            translate_expr(out, indent, *itr);
        }

        out << "\n" << --indent << ">";
    }

    static void translate_rule(
        std::ostream&                  out,
        peg::indent&                   indent,
        const peg_grammar::ast::entry& rule_entry)
    {
        using rule = typename peg_grammar::ast::rule;

        assert(rule_entry.rule() == rule::Rule);

        auto name = rule_entry[rule::RuleName];

        out << indent++ << "peg_rule(\n"
            << indent << name.lexeme() << ",\n";

        for (auto& entry : rule_entry) switch(entry.rule())
        {
            case rule::RuleName:
                continue;

            default:
                translate_expr(out, indent, entry); out << "\n";
                continue;
        }

        out << --indent << ");\n";
    }

    static void translate_grammar(
        std::ostream&                  out,
        peg::indent&                   indent,
        const peg_grammar::ast::entry& grammar_entry)
    {
        using rule = typename peg_grammar::ast::rule;

        assert(grammar_entry.rule() == rule::Grammar);

        auto name = grammar_entry[rule::GrammarName];

        out << indent++ << "peg_grammar(" << name.lexeme() << ") {\n\n";

        out << indent++ << "peg_rules(\n";

        for (auto& child : grammar_entry) switch(child.rule())
        {
            case rule::GrammarName:
                continue;

            case rule::Rule:
                out << indent << child[rule::RuleName].lexeme() << ",\n";
                continue;

            default: assert(false);
        }

        out << indent << "COUNT\n";
        out << --indent << ");\n";

        for (auto& child : grammar_entry) switch(child.rule())
        {
            case rule::GrammarName:
                continue;

            case rule::Rule:
                out << "\n";
                translate_rule(out, indent, child);
                continue;

            default: assert(false);
        }

        out << --indent << "};\n";
    }

    static void translate_namespace(
        std::ostream&                  out,
        peg::indent&                   indent,
        const peg_grammar::ast::entry& namespace_entry)
    {
        using rule = typename peg_grammar::ast::rule;

        assert(namespace_entry.rule() == rule::Namespace);

        auto name = namespace_entry[rule::NamespaceName];

        out << indent++ << "namespace " << name.lexeme() << " {\n";

        for (auto& child : namespace_entry) switch(child.rule())
        {
            case rule::NamespaceName:
                continue;

            case rule::Namespace:
                out << "\n";
                translate_namespace(out, indent, child);
                continue;

            case rule::Grammar:
                out << "\n";
                translate_grammar(out, indent, child);
                continue;

            default: assert(false);
        }

        out << --indent << "}\n";
    }

    static void translate_file(
        std::ostream&                out,
        const peg::ast<peg_grammar>& ast)
    {
        peg::indent indent;

        using rule = peg_grammar::rule;

        if (ast.empty())
            return;

        auto file_entry = ast.root();

        assert(file_entry.rule() == rule::File);

        out << "#pragma once\n"
            << "#include <zna/source/thirdParty/gtoolbox/peg/grammar.h>\n";

        for (auto& child : file_entry) switch (child.rule())
        {
            case rule::Namespace:
                out << "\n";
                translate_namespace(out, indent, child);
                continue;

            case rule::Grammar:
                out << "\n";
                translate_grammar(out, indent, child);
                continue;

            default: assert(false);
        }
    }

    static void translate_file(
        std::ostream& out,
        const char*   src)
    {
        auto ast =
            peg::parse<
                peg_grammar::File,
                peg_grammar::Namespace,
                peg_grammar::NamespaceName,
                peg_grammar::Grammar,
                peg_grammar::GrammarName,
                peg_grammar::Rule,
                peg_grammar::RuleName,
                peg_grammar::Dot,
                peg_grammar::Choice,
                peg_grammar::Sequence,
                peg_grammar::AndPredicate,
                peg_grammar::NotPredicate,
                peg_grammar::ZeroOrOne,
                peg_grammar::ZeroOrMore,
                peg_grammar::OneOrMore,
                peg_grammar::RuleReference,
                peg_grammar::StringLiteral,
                peg_grammar::CharLiteral,
                peg_grammar::CharClass,
                peg_grammar::CharClassRange,
                peg_grammar::CharClassSingle
            >(src);

        translate_file(out, ast);
    }

} // namespace peg