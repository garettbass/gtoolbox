#pragma once
#include <map>
#include <string>
#include <vector>
#include <gtoolbox/xtd/substring.h>
#include <gtoolbox/xtd/utility/type_list.h>

namespace json {

    class value;

    class object;

    using array = std::vector<value>;

    using boolean = bool;

    using null = decltype(nullptr);

    using number = double;

    using string = std::string;

    using substring = xtd::substring;

    using pointer = const void*;

    using pair = std::pair<string, value>;

    #define JSON_TYPES     \
        JSON_TYPE(null)    \
        JSON_TYPE(boolean) \
        JSON_TYPE(number)  \
        JSON_TYPE(string)  \
        JSON_TYPE(array)   \
        JSON_TYPE(object)

    #define JSON_TYPE(T) T,
    using types = ::xtd::type_list<JSON_TYPES void>;
    #undef JSON_TYPE

    enum class type : size_t
    {
        null    = json::types::index_of<json::null>(),
        boolean = json::types::index_of<json::boolean>(),
        number  = json::types::index_of<json::number>(),
        string  = json::types::index_of<json::string>(),
        array   = json::types::index_of<json::array>(),
        object  = json::types::index_of<json::object>(),
    };

    template <typename T>
    constexpr type json_typeof()
    {
        return type(types::index_of<T>());
    }

    #define json_typeof(T) (json::json_typeof<T>())

    const char* json_typename(type);

} // namespace json
