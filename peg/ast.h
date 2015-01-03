#pragma once
#include <gtoolbox/peg/lexeme.h>
#include <gtoolbox/peg/utility.h>

namespace peg {

    template <typename Rule, typename... Rules>
    struct rule_parser;

    template <typename Grammar>
    class ast
    {
    private: // type declarations

        struct node;

        using nodes = std::vector<node>;

    public: // type declarations

        class child_iterator;

        class entry;

        class entry_iterator;

        using grammar = Grammar;

        struct parser;

        using rule = typename grammar::rule;

    private: // type definitions

        struct node
        {
            cursor    cursor;
            size_t    size   = 0;
            size_t    parent = 0;
            rule      rule   = ast::rule::none;

            peg::lexeme lexeme() const { return { cursor, size }; }
        };

    private: // fields

        nodes _nodes {{}};

    private: // structors

        ast(nodes nodes) : _nodes(std::move(nodes)) {}

    public: // structors

        ast() = default;

        ast(ast&&) = default;

        ast& operator = (ast&&) = default;

        template <typename StartRule>
        static ast parse(const char* s)
        {
            parser p;

            lex<StartRule>(p, s);

            return std::move(p.nodes);
        }

        template <typename StartRule, typename ASTRule, typename... ASTRules>
        static ast parse(const char* s)
        {
            rule_parser<StartRule, ASTRule, ASTRules...> p;

            lex<StartRule>(p, s);

            return std::move(p.nodes);
        }

    public: // properties

        bool empty() const { return _nodes.size() == 1; }

        size_t size() const { return _nodes.size() - 1; }

        entry_iterator begin() const {
            size_t index = size() ? 1 : 0;
            return { &_nodes, index };
        }

        entry_iterator end() const {
            return { &_nodes, _nodes.size() };
        }

        entry root() const { return *begin(); }

    };

    //--------------------------------------------------------------------------

    template <typename Grammar>
    struct ast<Grammar>::parser : peg::parser
    {
        ast::nodes nodes {{}};

        size_t parent = 0;

    public: // types

        struct marker { size_t node_index; };

    public: // methods

        using peg::parser::lexing;
        using peg::parser::accepted;
        using peg::parser::rejected;

        parser() { nodes.reserve(4096 / sizeof(node)); }

        template <typename R, typename E>
        marker lexing(const peg::rule<Grammar, R, E>& lexeme)
        {
            auto node_index = nodes.size();

            nodes.emplace_back();

            auto& node = nodes.back();

            node.cursor = lexeme.first;
            node.size   = lexeme.size();
            node.parent = parent;
            node.rule   = R::id();

            parent = node_index;

            return { node_index };
        }

        template <typename R, typename E>
        void accepted(
            const peg::rule<Grammar, R, E>& lexeme,
            const                   marker& marker)
        {
            assert(marker.node_index < nodes.size());

            auto& node = nodes[marker.node_index];

            assert(node.cursor == lexeme.first);
            assert(node.rule   == R::id());

            node.size = lexeme.size();

            parent = node.parent;
        }
        
        template <typename R, typename E>
        void rejected(
            const peg::rule<Grammar, R, E>& lexeme,
            const                   marker& marker)
        {
            assert(marker.node_index < nodes.size());

            auto& node = nodes[marker.node_index];

            assert(node.cursor == lexeme.first);
            assert(node.rule   == R::id());

            parent = node.parent;

            nodes.resize(marker.node_index);
        }
        
    };

    //--------------------------------------------------------------------------

    template <typename Arg>
    inline
    constexpr
    bool
    all(const Arg& arg)
    {
        return bool(arg);
    }

    template <typename Arg, typename... Args>
    inline
    constexpr
    bool
    all(const Arg& arg, const Args&... args)
    {
        return bool(arg) and all(args...);
    }

    template <typename Rule, typename... Rules>
    inline
    constexpr
    bool is_same_grammar()
    {
        using grammar = typename Rule::grammar;

        return all((std::is_same<grammar, typename Rules::grammar>::value)...);
    }

    template <typename T>
    struct iterator_range
    {
        T first, last;

        bool empty() const { return first == last; }

        size_t size() const { return std::distance(first, last); }

        T begin() const { return first; }

        T end() const { return last; }
    };

    template <typename T>
    iterator_range<T>
    iterate(T first, T last) { return { first, last }; }

    //--------------------------------------------------------------------------

    template <typename Rule>
    struct rule_parser<Rule> : ast<typename Rule::grammar>::parser
    {
        using grammar = typename Rule::grammar;

        using base = typename ast<grammar>::parser;

        using marker = typename base::marker;

        rule_parser() = default;

        using peg::parser::lexing;
        using peg::parser::accepted;
        using peg::parser::rejected;

        marker lexing(const Rule& l) { return base::lexing(l); }

        void accepted(const Rule& l, const marker& m) { base::accepted(l, m); }

        void rejected(const Rule& l, const marker& m) { base::rejected(l, m); }
    };

    template <typename Rule, typename... Rules>
    struct rule_parser : rule_parser<Rules...>
    {
        using grammar = typename Rule::grammar;

        using base = typename ast<grammar>::parser;

        using marker = typename base::marker;

        rule_parser() {
            static_assert(
                is_same_grammar<Rule, Rules...>(),
                "Rules must be from the same grammar");
        }

        using rule_parser<Rules...>::lexing;
        using rule_parser<Rules...>::accepted;
        using rule_parser<Rules...>::rejected;

        marker lexing(const Rule& l) { return base::lexing(l); }

        void accepted(const Rule& l, const marker& m) { base::accepted(l, m); }

        void rejected(const Rule& l, const marker& m) { base::rejected(l, m); }
    };

    //--------------------------------------------------------------------------

    template <typename Grammar>
    class ast<Grammar>::entry
    {
        friend class ast;

    public: // types

        using ast = ast<Grammar>;

        using child_range = iterator_range<child_iterator>;

    protected: // fields

        const nodes* _nodes = nullptr;
        size_t       _index = 0;

    protected: // structors

        entry() = default;

        entry(const nodes* nodes, size_t index)
        : _nodes(nodes)
        , _index(index)
        {}

    public: // operators

        bool operator == (const entry_iterator& that) {
            return (_nodes == that._nodes) and (_index == that._index);
        }

        bool operator != (const entry_iterator& that) {
            return not operator==(that);
        }

        entry operator [] (ast::rule rule) const
        {
            for (auto& child : children())
                if (child.rule() == rule)
                    return child;

            return {};
        }

    public: // properties

        cursor cursor() const { return node().cursor; }

        lexeme lexeme() const { return node().lexeme(); }

        entry ancestor(ast::rule rule) const {
            auto entry = parent();
            while (entry.rule() != rule) entry = entry.parent();
            return entry;
        }

        entry parent() const { return { _nodes, node().parent }; }

        ast::rule rule() const { return node().rule; }

        const char* name() const { return grammar::rule_name(rule()); }

    public: // iterators

        child_range children() const {
            return { begin(), end() };
        }

        child_iterator begin() const {
            const size_t next = _index + 1;
            return
                (node(next).parent == _index)
                ? child_iterator { _nodes, next }
                : end();
        }

        child_iterator end() const {
            return child_iterator { _nodes, _nodes->size() };
        }

    protected: // properties

        const struct node& node() const { return node(_index); }

        const struct node& node(size_t i) const { return (*_nodes)[i]; }
    };

    //--------------------------------------------------------------------------

    template <typename Grammar>
    class ast<Grammar>::entry_iterator : private entry
    {
        friend class ast;

    protected:

        using entry::_nodes;
        using entry::_index;

    protected:

        using entry::entry;

    public: // operators

        using entry::operator ==;
        using entry::operator !=;

        entry_iterator& operator ++ () { ++_index; return *this; }

        entry& operator *  () { return *this; }
        entry* operator -> () { return  this; }

        operator child_iterator& () { return *(child_iterator*)this; }
    };

    //--------------------------------------------------------------------------

    template <typename Grammar>
    class ast<Grammar>::child_iterator : public entry_iterator
    {
        friend class entry;

        using entry_iterator::_nodes;
        using entry_iterator::_index;

        using entry_iterator::entry_iterator;
        using entry_iterator::node;

    public: // operators

        using entry_iterator::operator==;
        using entry_iterator::operator!=;

        child_iterator& operator ++ () {
            const size_t parent = node().parent;
            const size_t size   = _nodes->size();
            while (++_index < size and node().parent != parent);
            return *this;
        }

        using entry_iterator::operator*;
        using entry_iterator::operator->;
    };

    template <typename StartRule>
    ast<typename StartRule::grammar>
    parse(const char* s)
    {
        using grammar = typename StartRule::grammar;

        using ast = typename grammar::ast;

        return ast::template parse<StartRule>(s);
    }

    template <typename StartRule, typename ASTRule, typename... ASTRules>
    ast<typename StartRule::grammar>
    parse(const char* s)
    {
        using grammar = typename StartRule::grammar;

        using ast = typename grammar::ast;

        return ast::template parse<StartRule, ASTRule, ASTRules...>(s);
    }

    template <typename Stream, typename Grammar>
    void
    write(Stream& s, typename ast<Grammar>::entry entry, peg::indent& indent)
    {
        if (0 == strcmp(entry.name(), "NamespaceTail"))
            fflush(stdout);

        s << indent << entry.name();

        auto lexeme = entry.lexeme();

        if (const size_t size = lexeme.size())
        {
            const char* head = entry.cursor();

            const char* tail = head + size;

            if (const char* newline = strchr(head, '\n')) {
                // truncate lexeme at first newline
                tail = (const char*)std::min(size_t(tail), size_t(newline));
            }

            const size_t count = size_t(tail) - size_t(head);

            assert(count <= size);

            const char* close = (count < size) ? "...\"" : "\"";

            s << " \""; s.write(lexeme.data(), count) << close;
        }

        s << '\n';

        ++indent;

        for (auto& child : entry)
            write<Stream, Grammar>(s, child, indent);

        --indent;
    }

    template <typename Stream, typename Grammar>
    void
    write(Stream& s, typename ast<Grammar>::entry entry)
    {
        peg::indent indent; write<Stream, Grammar>(s, entry, indent);
    }

    template <typename Stream, typename Grammar>
    void
    write(Stream& s, ast<Grammar>& a, peg::indent& indent)
    {
        write<Stream, Grammar>(s, a.root(), indent);
    }

    template <typename Stream, typename Grammar>
    void
    write(Stream& s, ast<Grammar>& a)
    {
        peg::indent indent; write(s, a, indent);
    }

    template <typename Stream, typename Grammar>
    auto
    operator << (Stream& s, typename ast<Grammar>::entry entry)
    -> decltype(s << 0)
    {
        write<Stream, Grammar>(s, entry); return s;
    }

    template <typename Stream, typename Grammar>
    auto
    operator << (Stream& s, ast<Grammar>& a)
    -> decltype(s << 0)
    {
        write<Stream, Grammar>(s, a); return s;
    }

} // namespace peg