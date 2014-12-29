#include "types.h"

namespace json {

    const char* json_typename(type t)
    {
        const char* const typenames[] {
        #define JSON_TYPE(T) #T,
            JSON_TYPES
        #undef JSON_TYPE
        };

        return typenames[(int)t];
    }

} // namespace json