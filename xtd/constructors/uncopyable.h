#pragma once
#include "unassignable.h"

namespace xtd {

    //--------------------------------------------------------------------------

    template <uint64_t unique_id>
    class uncopyable : unassignable<unique_id>
    {
    protected: // structors

        uncopyable () = default;

        uncopyable (const uncopyable&) = delete;
        
       ~uncopyable () = default;

    };

    #define xtd_uncopyable xtd::uncopyable<xtd_source_id_hash64>

    //--------------------------------------------------------------------------

} // namespace xtd
