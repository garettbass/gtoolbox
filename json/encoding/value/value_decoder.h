#pragma once
#include "../decoder.h"
#include "../../value.h"

namespace json {

    //--------------------------------------------------------------------------
    
    class value_decoder : public decoder
    {
        const value* _value;
    
    public: // structors
        
        value_decoder(const value& v) : _value(&v) {}
        
    public: // decoding

        using decoder::decode_to;

        bool decode(json::encoder&) override;
    
    };

    //--------------------------------------------------------------------------

} // namespace json