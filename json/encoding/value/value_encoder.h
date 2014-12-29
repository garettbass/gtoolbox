#pragma once
#include <ostream>
#include <vector>
#include "../decoder.h"
#include "../../value.h"

namespace json {

    //--------------------------------------------------------------------------
    
    class value_encoder : public encoder
    {
    private: // fields

        value* _root;

        std::vector<json::value*> _stack;

    public: // structors
    
        value_encoder(json::value&);

       ~value_encoder();
        
    public: // encoding
    
        void encode_null() override;
        
        void encode_boolean(json::boolean) override;
        
        void encode_number(json::number) override;
        
        void encode_string(json::substring) override;

        void array_enter() override;
        void array_leave() override;
        
        void object_enter() override;
        void object_leave() override;
        
        void property_enter(json::substring key) override;
        void property_leave() override;
    };

    //--------------------------------------------------------------------------

} // namespace json