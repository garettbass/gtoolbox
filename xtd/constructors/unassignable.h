#pragma once
#include <cstdint>
#include <type_traits>
#include "../utility/source_id.h"

namespace xtd {

    //==========================================================================

    template <uint64_t unique_id>
    class unassignable
    {
    protected: // structors

        unassignable () = default;

        unassignable (const unassignable&) = default;
        
       ~unassignable () = default;

    protected: // operators

        void operator = (const unassignable&) = delete;
    };

    #define xtd_unassignable xtd::unassignable<xtd_source_id_hash64>

    //==========================================================================

} // namespace xtd