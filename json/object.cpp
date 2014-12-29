#include "./value.h"
#include <iostream>

namespace json {

    //==========================================================================

    object::object()
    : base()
    {}

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    object::object(const object&  o)
    : base((const base&)o)
    {}

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    object::object(object&& o)
    : base(std::move((base&)o))
    {}

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    object object::parse(const char* s)
    {
        return std::move((object&)value::parse(s));
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    object object::parse(const std::string& s)
    {
        return std::move((object&)value::parse(s));
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    object object::parse(std::istream& s)
    {
        return std::move((object&)value::parse(s));
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    object::object(std::initializer_list<json::pair> pairs)
    {
        for (auto& pair : pairs)
        {
            const bool inserted =
                base::emplace(std::move(pair.first), std::move(pair.second))
                .second;

            assert(inserted);
        }
    }

    //--------------------------------------------------------------------------

    object& object::operator = (const object& o)
    {
        base::operator=((const base&)o);

        return *this;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    object& object::operator = (object&& o)
    {
        base::operator=(std::move((base&)o));

        return *this;
    }

    //--------------------------------------------------------------------------

    bool object::operator == (const object& b) const
    {
        auto& a = *this;
        return
            a.size() == b.size()
            and
            std::equal(a.begin(), a.end(), b.begin());
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    bool object::operator != (const object& b) const
    {
        return not operator==(b);
    }

    //--------------------------------------------------------------------------

    bool object::contains(const string& key) const
    {
        return base::count(key);
    }

    //--------------------------------------------------------------------------

    const value* object::find(const string& key) const
    {
        auto itr = base::find(key), end = base::end();

        return (itr == end) ? nullptr : &itr->second;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    value* object::find(const string& key)
    {
        auto itr = base::find(key), end = base::end();

        return (itr == end) ? nullptr : &itr->second;
    }

    //--------------------------------------------------------------------------

    value object::remove(const string& key)
    {
        auto itr = base::find(key), end = base::end();

        value v;

        if (itr != end)
        {
            v = std::move(itr->second);

            base::erase(itr);
        }

        return v;
    }

    //--------------------------------------------------------------------------

    object& operator << (object& o, const char* s)
    {
        for (auto& pair : object::parse(s))
            o[pair.first] = pair.second;

        return o;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    object& operator << (object& o, const string& s)
    {
        for (auto& pair : object::parse(s))
            o[pair.first] = pair.second;

        return o;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    object& operator << (object& o, std::istream& s)
    {
        for (auto& pair : object::parse(s))
            o[pair.first] = pair.second;

        return o;
    }

    //--------------------------------------------------------------------------

    std::ostream& operator << (std::ostream& s, const object& o)
    {
        return s << value(o);
    }

    //==========================================================================

};

void debugPrintJSONObject(const json::object& obj)
{
    std::cout << obj;
}
