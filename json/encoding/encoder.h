#pragma once
#include <gtoolbox/xtd/constructors/interface.h>
#include "../types.h"

namespace json {

    //--------------------------------------------------------------------------

    struct encoder : xtd_interface
    {
        virtual void encode_null() = 0;
    
        virtual void encode_boolean(bool) = 0;
    
        virtual void encode_number(double) = 0;
        
        virtual void encode_string(substring) = 0;

        virtual void array_enter() = 0;
        virtual void array_leave() = 0;

        virtual void object_enter() = 0;
        virtual void object_leave() = 0;

        virtual void property_enter(substring) = 0;
        virtual void property_leave() = 0;
    };

    //--------------------------------------------------------------------------

    class null_encoder final : public encoder
    {
        ~null_encoder() {}

    public: // accessors

        static null_encoder& instance()
        {
            static null_encoder instance;

            return instance;
        }

    public: // methods

        virtual void encode_null() {}
    
        virtual void encode_boolean(bool) {}
    
        virtual void encode_number(double) {}
        
        virtual void encode_string(substring) {}

        virtual void array_enter() {}
        virtual void array_leave() {}

        virtual void object_enter() {}
        virtual void object_leave() {}

        virtual void property_enter(substring) {}
        virtual void property_leave() {}
    };

    //--------------------------------------------------------------------------

} // namespace json