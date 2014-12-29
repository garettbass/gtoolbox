#include "./value_decoder.h"

namespace json {
    
    //==========================================================================

    static void decode(json::encoder& encoder, const value& v)
    {
        switch (v.type())
        {
            case type::null:
                return encoder.encode_null();

            case type::boolean:
                return encoder.encode_boolean((const boolean&)v);

            case type::number:
                return encoder.encode_number((const number&)v);

            case type::string:
                return encoder.encode_string((const string&)v);

            case type::array:
            {
                encoder.array_enter();

                for (auto& element : (const array&)v)
                    decode(encoder, element);

                encoder.array_leave();

                return;
            }

            case type::object:
            {
                encoder.object_enter();

                for (auto& property : (const object&)v)
                {
                    encoder.property_enter(property.first);

                    decode(encoder, property.second);

                    encoder.property_leave();
                }

                encoder.object_leave();

                return;
            }
        }
    }

    //==========================================================================

    bool value_decoder::decode(json::encoder& encoder)
    {
        if (_value)
        {
            json::decode(encoder, *_value);

            return true;
        }

        return false;
    }

    //==========================================================================

} // namespace json