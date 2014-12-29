#pragma once
#include <istream>
#include "../decoder.h"

namespace json {

    //--------------------------------------------------------------------------
    
    class json_decoder : public decoder
    {
        std::string _buffer;

        const char* _chars;

    public: // structors
        
        json_decoder(const char* s) : _chars(s) {}

        json_decoder(const std::string& s) : _chars(s.c_str()) {}

        json_decoder(std::istream&);

    public: // decoding

        using decoder::decode_to;
    
        bool decode(json::encoder&) override;
    
    };

    //--------------------------------------------------------------------------

} // namespace json