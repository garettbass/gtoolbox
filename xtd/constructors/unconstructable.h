#pragma once
#include "uncopyable.h"

namespace xtd {

    //--------------------------------------------------------------------------

    template <uint64_t unique_id>
    class unconstructable
    : uncopyable<unique_id>
    {
        unconstructable () = delete;
    };

    #define xtd_unconstructable xtd::unconstructable<xtd_source_id_hash64>

    //--------------------------------------------------------------------------

} // namespace xtd