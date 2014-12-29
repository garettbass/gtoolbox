#pragma once
#include "./unconstructable.h"

namespace xtd {

    //--------------------------------------------------------------------------

    template <uint64_t unique_id>
    class uninstantiable
    : unconstructable<unique_id>
    {
       ~uninstantiable () = delete;
    };

    #define xtd_uninstantiable xtd::uninstantiable<xtd_source_id_hash64>

    //--------------------------------------------------------------------------

} // namespace xtd