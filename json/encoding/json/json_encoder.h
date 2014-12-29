#pragma once
#include <ostream>
#include "../decoder.h"

namespace json {

    //--------------------------------------------------------------------------
    
    class json_encoder : public encoder
    {    
    public: // types
    
        struct config
        {
            substring indent;
            
            substring newline;
            
            substring space;
        };

        static const config default_config() { return { "    ", "\n", " " }; }
    
    private: // fields
    
        std::ostream& _stream;
        
        config _config;
        
        size_t _indent_depth = 0;

        bool _emit_next_comma = false;

        bool _emit_next_indent = false;

    public: // structors
    
        json_encoder(std::ostream&, config = default_config());
        
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
        
    public: // utilities
    
        static const char* escape(char);

    private: // implementation
        
        void emit_comma();
        
        void emit_indent();

    };

    //--------------------------------------------------------------------------

} // namespace json