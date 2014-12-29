#pragma
#include <cassert>
#include "./object.h"

namespace json {

    //--------------------------------------------------------------------------

    template <typename T>
    class typed_value final
    {
        friend class value;

        const type _type = json_typeof(T);

        T _value;

    private: // structors

        typed_value()                   = delete;
        typed_value(const typed_value&) = delete;

        typed_value(const T& v) : _value (v)            {}
        typed_value(     T&& v) : _value (std::move(v)) {}

    private: // operators

        const T* ptr() const { return is_type() ? &_value : nullptr; }
              T* ptr()       { return is_type() ? &_value : nullptr; }

        const T& ref() const { assert(is_type()); return *ptr(); }
              T& ref()       { assert(is_type()); return *ptr(); }

        bool is_type() const { return (_type == json_typeof(T)); }

    };

    //--------------------------------------------------------------------------

} // namespace json