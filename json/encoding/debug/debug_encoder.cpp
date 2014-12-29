#include <iostream>
#include "./debug_encoder.h"

namespace json {

    //--------------------------------------------------------------------------

    static void indent(size_t depth)
    {
        for (; depth; --depth) std::cout << "    ";
    }

    //--------------------------------------------------------------------------

    debug_encoder::debug_encoder(encoder& e) : _encoder(&e) {}

    //--------------------------------------------------------------------------
    
    void debug_encoder::encode_null()
    {
        indent(_depth);

        std::cout << __func__ << "(null)" << std::endl;

        _encoder->encode_null();
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    void debug_encoder::encode_boolean(bool value)
    {
        indent(_depth);

        std::cout
            << __func__ << "("
            << (value ? "true" : "false")
            << ")" << std::endl;

        _encoder->encode_boolean(value);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    void debug_encoder::encode_number(double value)
    {
        indent(_depth);

        std::cout << __func__ << "(" << value << ")" << std::endl;

        _encoder->encode_number(value);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void debug_encoder::encode_string(substring value)
    {
        indent(_depth);

        std::cout << __func__ << "(\"" << value << "\")" << std::endl;

        _encoder->encode_string(value);
    }

    //--------------------------------------------------------------------------
    
    void debug_encoder::array_enter()
    {
        indent(_depth++);

        std::cout << __func__ << "()" << std::endl;

        _encoder->array_enter();
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void debug_encoder::array_leave()
    {
        indent(--_depth);

        std::cout << __func__ << "()" << std::endl;

        _encoder->array_leave();
    }

    //--------------------------------------------------------------------------
    
    void debug_encoder::object_enter()
    {
        indent(_depth++);

        std::cout << __func__ << "()" << std::endl;

        _encoder->object_enter();
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void debug_encoder::object_leave()
    {
        indent(--_depth);

        std::cout << __func__ << "()" << std::endl;

        _encoder->object_leave();
    }

    //--------------------------------------------------------------------------
    
    void debug_encoder::property_enter(substring key)
    {
        indent(_depth++);

        std::cout << __func__ << "(\"" << key << "\")" << std::endl;

        _encoder->property_enter(key);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void debug_encoder::property_leave()
    {
        indent(--_depth);

        std::cout << __func__ << "()" << std::endl;

        _encoder->property_leave();
    }

    //--------------------------------------------------------------------------

} // namespace json