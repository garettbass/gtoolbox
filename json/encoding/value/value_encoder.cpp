#include "./value_encoder.h"
#include <gtoolbox/xtd/echo.h>
#include <iostream>

namespace json {

    //--------------------------------------------------------------------------

    value_encoder::value_encoder(json::value& root)
    : _root(&root)
    {
        root = json::null();

        _stack.reserve(8);

        _stack.push_back(&root);
    }

    //--------------------------------------------------------------------------

    value_encoder::~value_encoder()
    {
        assert(_stack.size() == 1);
    }

    //--------------------------------------------------------------------------

    template <typename T, typename Stack>
    static void set_or_append(Stack& stack, const T& data)
    {
        json::value& value = *stack.back();

        if (json::array* array = value)
        {
            array->push_back(data);
        }
        else
        {
            assert(value.type() == json::type::null);

            value = data;
        }
    }

    //--------------------------------------------------------------------------

    template <typename T, typename Stack>
    static void set_or_push(Stack& stack)
    {
        json::value& top = *stack.back();

        if (json::array* array = top)
        {
            array->push_back(T());

            stack.push_back(&array->back());
        }
        else
        {
            assert(top.type() == json::type::null);

            top = T();
        }
    }

    //--------------------------------------------------------------------------

    template <typename Stack>
    static bool should_pop(Stack& stack)
    {
        const size_t size = stack.size();

        return (size >= 2) and stack[size - 2]->type() == json::type::array;
    }

    //--------------------------------------------------------------------------

    void value_encoder::encode_null()
    {
        set_or_append(_stack, nullptr);
    }

    void value_encoder::encode_boolean(json::boolean b)
    {
        set_or_append(_stack, b);
    }

    void value_encoder::encode_number(json::number n)
    {
        set_or_append(_stack, n);
    }

    void value_encoder::encode_string(json::substring s)
    {
        set_or_append(_stack, s);
    }

    //--------------------------------------------------------------------------

    void value_encoder::array_enter()
    {
        set_or_push<json::array>(_stack);
    }

    void value_encoder::array_leave()
    {
        if (should_pop(_stack))
            _stack.pop_back();
    }

    //--------------------------------------------------------------------------

    void value_encoder::object_enter()
    {
        set_or_push<json::object>(_stack);
    }

    void value_encoder::object_leave()
    {
        if (should_pop(_stack))
            _stack.pop_back();
    }

    //--------------------------------------------------------------------------

    void value_encoder::property_enter(json::substring key)
    {
        auto& top = *_stack.back();

        assert(top.type() == json::type::object);

        json::value& value = ((object&)top)[json::string(key)];

        if (value.type() != json::type::null)
        {
            std::cout
                << "duplicate property '" << key << "' "
                << "will replace existing value: \n" << value
                << "\n";

            value = json::null();
        }

        _stack.push_back(&value);
    }

    void value_encoder::property_leave()
    {
        _stack.pop_back();
    }

    //--------------------------------------------------------------------------

} // namespace json