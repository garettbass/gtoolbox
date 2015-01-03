#include <cstdio>
#include <gtoolbox/peg/main.h>
#include <gtoolbox/peg/grammars/peg_grammar2.h>
#include <gtoolbox/peg/peg_to_cpp.h>
#include <gtoolbox/xtd/echo.h>
#include <gtoolbox/xtd/filesystem/path.h>
#include <gtoolbox/xtd/traits.h>
#include <gtoolbox/xtd/utility/scope_timer.h>

namespace peg {

//------------------------------------------------------------------------------

static void peg_grammar_tests()
{
    assert(is<peg_grammar::Identifier>("foo"));

    assert(is<peg_grammar::PrimaryExpr>("foo"));

    assert(is_not<peg_grammar::PrimaryExpr>("foo="));
    assert(is_not<peg_grammar::PrimaryExpr>("foo ="));

    assert(is<peg_grammar::PrimaryExpr>("(foo)"));
    assert(is<peg_grammar::PrimaryExpr>("(foo)"));
    assert(is<peg_grammar::PrimaryExpr>("\"foo\""));
    assert(is<peg_grammar::PrimaryExpr>("'f'"));
    assert(is<peg_grammar::PrimaryExpr>("."));

    assert(is<peg_grammar::Repetition>("foo*"));
    assert(is<peg_grammar::Repetition>("foo?"));

    assert(is<peg_grammar::Predicate>("&foo*"));
    assert(is<peg_grammar::Predicate>("!foo?"));

    assert(is_not<peg_grammar::Sequence>("foo"));
    assert(is<peg_grammar::Sequence>("foo bar"));

    assert(is_not<peg_grammar::Choice>("foo"));
    assert(is<peg_grammar::Choice>("foo | bar"));

    assert(is<peg_grammar::Rule>("baz = foo;"));
    assert(is<peg_grammar::Rule>("baz = foo | bar;"));
    assert(is<peg_grammar::Rule>("baz = !(foo | bar);"));

    assert(is<peg_grammar::Grammar>("grammar fizzle {}"));

    assert(is<peg_grammar::Grammar>("grammar fizzle { baz = foo; }"));
    assert(is<peg_grammar::Grammar>("grammar fizzle { baz = foo; bar = fiz; }"));

    assert(is<peg_grammar::CharClass>("[abc]"));
    assert(is<peg_grammar::CharClass>("[c-z]"));
    assert(is<peg_grammar::CharClass>("[abc-z]"));
    assert(is<peg_grammar::CharClass>(R"([abc-z\]])"));

    assert(is<peg_grammar::File>(R"(
        grammar fizzle {
            baz = foo bar | baz;
            bar = fiz;
            str = "str";
            fiz = !(faz | wiz);
            wiz = [abc-z\]];
        }

        namespace fuzzle {
        }

        namespace wuzzle {

            grammar fazzle {
                baz = foo bar | baz;
                bar = fiz;
                str = "str";
                fiz = !(faz | wiz);
                wiz = [abc-z\]];
            }

        }
    )"));

    assert(is<ascii_grammar::printable_char>("f"));
    assert(is<c_literal_grammar::char_literal_body>("f"));
    assert(is<c_literal_grammar::char_literal>("'f'"));
}

//------------------------------------------------------------------------------

int main(const int argc, const char* argv[])
{
    peg_grammar_tests();

    std::stringstream out;

    std::cout << "\n--------\n";

    auto dir = xtd::path(__FILE__).parent();

    auto src_file = dir / "grammars/peg_grammar2.peg";

    assert(src_file.exists());

    TIMED_SCOPE("tree::parse") {

        auto src = src_file.read();

        assert(is<peg_grammar::File>(src.c_str()));

        translate_file(out, src.c_str());
    }

    auto out_file = dir / "grammars/peg_grammar2.h";

    out_file.write(out.str());

    std::cout << "\n--------\n";

    std::cout << out.str();

    return 0;
}

} // namespace peg