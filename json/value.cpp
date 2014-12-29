#include <zna/source/thirdParty/gtoolbox/env/compiler.h>
#include "value.h"
#include "./encoding/json/json_decoder.h"
#include "./encoding/json/json_encoder.h"
#include "./encoding/value/value_decoder.h"
#include "./encoding/value/value_encoder.h"

namespace json {

    const value value::undefined(nullptr);

    //--------------------------------------------------------------------------

    value::value(const value& that)
    : _type(that._type)
    {
        switch (_type) {
        #define JSON_TYPE(T)                         \
            case json_typeof(T): {                   \
                const T* that_##T = that._##T.ptr(); \
                      T* this_##T =      _##T.ptr(); \
                new(this_##T) T(*that_##T);          \
                return;                              \
            }
            JSON_TYPES
        #undef JSON_TYPE
        }

        assert(false);
    }

    //--------------------------------------------------------------------------

    value::value(value&& that)
    : _type(that._type)
    {
        assert(_type < json::type(json::types::size()));

        switch (_type) {
        #define JSON_TYPE(T)                           \
            case json_typeof(T): {                     \
                const T* that_##T = that._##T.ptr();   \
                      T* this_##T =      _##T.ptr();   \
                new(this_##T) T(std::move(*that_##T)); \
                return;                                \
            }
            JSON_TYPES
        #undef JSON_TYPE
        }

        assert(false);
    }

    //--------------------------------------------------------------------------

    value::value(std::istream& s)
    : value()
    {
        json_decoder(s).decode_to<value_encoder>(*this);
    }

    //--------------------------------------------------------------------------

    value::~value()
    {
        assert(_type < json::type(json::types::size()));

        // ensure _type fields are overlaid
        #define JSON_TYPE(T)                       \
            env_static_assert(                     \
                offsetof(value,          _type) == \
                offsetof(typed_value<T>, _type));
            JSON_TYPES
        #undef JSON_TYPE

        switch (_type) {
        #define JSON_TYPE(T)              \
            case json_typeof(T): {        \
                T* this_##T = _##T.ptr(); \
                this_##T->~T();           \
                return;                   \
            }
            JSON_TYPES
        #undef JSON_TYPE
        }

        assert(false);
    }

    //--------------------------------------------------------------------------

    value value::parse(const char* s)
    {
        value v;

        if (not json_decoder(s).decode_to<value_encoder>(v))
            v = json::null();

        return v;
    }

    //--------------------------------------------------------------------------

    value value::parse(const std::string& s)
    {
        return value::parse(s.c_str());
    }

    //--------------------------------------------------------------------------

    value value::parse(std::istream& s)
    {
        value v;

        json_decoder(s).decode_to<value_encoder>(v);

        return v;
    }

    //--------------------------------------------------------------------------

    value& value::operator = (const value& v)
    {
        this->~value();

        new(this) value(v);

        return *this;
    }

    //--------------------------------------------------------------------------

    value& value::operator = (value&& v)
    {
        this->~value();

        new(this) value(std::move(v));

        return *this;
    }

    //--------------------------------------------------------------------------

    bool value::operator == (const value& v) const
    {
        if (_type != v._type)
            return false;

        switch (_type)
        {
            case type::null:
                return true;

            case type::boolean:
                return _boolean.ref() == v._boolean.ref();

            case type::number:
                return _number.ref() == v._number.ref();

            case type::string:
                return _string.ref() == v._string.ref();

            case type::array:
            {
                auto& a =   _array.ref();
                auto& b = v._array.ref();
                return
                    a.size() == b.size()
                    and
                    std::equal(a.begin(), a.end(), b.begin());
            }

            case type::object:
                return _object.ref() == v._object.ref();
        }

        assert(false);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    bool value::operator != (const value& v) const
    {
        return not operator==(v);
    }

    //--------------------------------------------------------------------------

    #define json_value_conversion(T)                             \
        value::operator const T* () const { return _##T.ptr(); } \
        value::operator       T* ()       { return _##T.ptr(); } \
        value::operator const T& () const { return _##T.ref(); } \
        value::operator       T& ()       { return _##T.ref(); }

        json_value_conversion(boolean)
        json_value_conversion(number)
        json_value_conversion(string)
        json_value_conversion(array)
        json_value_conversion(object)

    #undef json_value_conversion

    //--------------------------------------------------------------------------

    value& operator << (value& v, std::istream& s)
    {
        json_decoder(s).decode_to<value_encoder>(v);

        return v;
    }

    //--------------------------------------------------------------------------

    std::ostream& operator << (std::ostream& s, const value& v)
    {
        value_decoder(v).decode_to<json_encoder>(s);

        return s;
    }

    //--------------------------------------------------------------------------

} // namespace json
