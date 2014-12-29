#pragma once
#include "../decoder.h"

namespace json {

    //--------------------------------------------------------------------------
    
    class debug_encoder : public encoder
    {
        size_t _depth = 0;

        encoder* _encoder = nullptr;

    public: // structors
    
        debug_encoder(encoder& = null_encoder::instance());
        
    public: // encoding
    
        void encode_null() override;
        
        void encode_boolean(bool) override;
        
        void encode_number(double) override;
        
        void encode_string(substring) override;

        void array_enter() override;
        void array_leave() override;
        
        void object_enter() override;
        void object_leave() override;
        
        void property_enter(substring key) override;
        void property_leave() override;

    };

    //--------------------------------------------------------------------------

} // namespace json