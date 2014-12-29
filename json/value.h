#pragma once
#include "./typed_value.h"

//--------------------------------------------------------------------------
// Json Values
// A Json Value is a class which can represent any of the Json types at a given time.
//
// json::value::parse on a string will get you back the root value of the whole json tree.
// A json::value can be cast to the individual objects that it represents.
//
// The null representation of a json::value is a value that is of json::type::null.
// The engine will pass around a json::null on some failures, such as failure to parse JSON.
//
// If casting the json::value with pointer (Ex: json::string*),
// then the cast will automatically return a nullptr if the cast is invalid.
// You can case from json::value to json::type but you cannot cast the other way around.
//
// Useful utilities for utilizing json::value:
// JsonUtility.h
// ABJsonUtility.h
// JsonNSDictionaryEncoding.h / JsonNSDictionaryDecoding.h
// JsonLuaTableEncoding.h / JsonLuaTableDecoding.h
// JsonToProtobuf.h / JsonFromProtobuf.h
// ABUtility.h
//

namespace json {

    //--------------------------------------------------------------------------

    class value final
    {
    public: // constants

        static const value undefined;

    private: // fields

        union {
            type                 _type = json::type(json::types::size());
            typed_value<array>   _array;
            typed_value<boolean> _boolean;
            typed_value<null>    _null;
            typed_value<number>  _number;
            typed_value<string>  _string;
            typed_value<object>  _object;
        };

    public: // structors

        value()     : _null (nullptr) {}
        value(null) : _null (nullptr) {}

        value(boolean  b) : _boolean (b) {}

        value(    int32_t i) : _number (i) {}
        value(   uint32_t i) : _number (i) {}
        value(    int64_t i) : _number (i) {}
        value(   uint64_t i) : _number (i) {}
        value(      float f) : _number (f) {}
        value(     double f) : _number (f) {}
        value(long double f) : _number (f) {}

        value(const char*      s) : _string (s) {}
        value(const string&    s) : _string (s) {}
        value(const substring& s) : _string (string(s)) {}
        value(        string&& s) : _string (std::move(s)) {}

        value(const  array& a) : _array  (a) {}
        value(const object& o) : _object (o) {}
        value(const  value&);

        value( array&& a) : _array  (std::move(a)) {}
        value(object&& o) : _object (std::move(o)) {}
        value( value&&);

        explicit
        value(std::istream&);

       ~value();

        static value parse(const char*);
        static value parse(const string&);
        static value parse(std::istream&);

    public: // operators

        value& operator = (const value&);
        value& operator = (      value&&);

        bool operator == (const value&) const;
        bool operator != (const value&) const;

        #define json_value_conversion(T) \
            operator const T* () const;  \
            operator       T* ();        \
            operator const T& () const;  \
            operator       T& ();

            json_value_conversion(boolean)
            json_value_conversion(number)
            json_value_conversion(string)
            json_value_conversion(array)
            json_value_conversion(object)

        #undef json_value_conversion

    public: // properties

        json::type type() const { return _type; }

    };

    //--------------------------------------------------------------------------

    template <typename T>
    inline
    bool operator == (const value& v, const T& t) { return v == value(t); }

    template <typename T>
    inline
    bool operator == (const T& t, const value& v) { return v == value(t); }

    //--------------------------------------------------------------------------

    template <typename T>
    inline
    bool operator != (const value& v, const T& t) { return v != value(t); }

    template <typename T>
    inline
    bool operator != (const T& t, const value& v) { return v != value(t); }

    //--------------------------------------------------------------------------
    // NOTE: these operators read/write JSON-formatted text, i.e.
    //
    //       * booleans are 'true' or 'false' (not quoted)
    //
    //       * strings are quoted and contain escape-sequences
    //
    //       * objects are { surrounded by braces }
    //
    //       * arrays are [ surrounded by brackets ]

    value& operator << (value&, std::istream&);

    std::ostream& operator << (std::ostream&, const value&);

    //--------------------------------------------------------------------------

} // namespace json
