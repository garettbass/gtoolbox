#pragma once
#include "encoder.h"

namespace json {
    
    struct decoder : xtd_interface
    {
        template <typename Encoder, typename... Args>
        bool decode_to(Args&&... args)
        {
            Encoder encoder(std::forward<Args>(args)...);

            return decode(encoder);
        }

        virtual bool decode(encoder&) = 0;
    };

} // namespace json