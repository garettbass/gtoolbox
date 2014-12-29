#pragma once
#include <map>
#include "./types.h"

namespace json {

    //==========================================================================

    struct case_insensitive_compare
    {
        bool operator () (const string& a, const string& b) const
        {
            return strcasecmp(a.c_str(), b.c_str()) < 0;
        }
    };

    struct object : std::map<string, value, case_insensitive_compare>
    {
    public: // types

        using base = std::map<string, value, case_insensitive_compare>;

    public: // structors

        object();

        explicit
        object(const object&);
        object(      object&&);

        object(std::initializer_list<json::pair>);

        static object parse(const char*);
        static object parse(const string&);
        static object parse(std::istream&);

    public: // operators

        object& operator = (const object&);
        object& operator = (      object&&);

        bool operator == (const object&) const;
        bool operator != (const object&) const;

    public: // algorithms

        bool contains(const string&) const;

        const value* find(const string&) const;
              value* find(const string&);

        value remove(const string&);
    };

    //--------------------------------------------------------------------------

    object& operator << (object&, const char*);
    object& operator << (object&, const string&);
    object& operator << (object&, std::istream&);

    std::ostream& operator << (std::ostream&, const object&);

    //==========================================================================

};

// for lldb debugging, lldb often doesn't grok namespaces and std::cout
extern void debugPrintJSONObject(const json::object&);