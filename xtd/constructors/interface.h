#pragma once
#include "./uncopyable.h"

namespace xtd {

    //--------------------------------------------------------------------------

    template <uint64_t unique_id>
    class interface
    : uncopyable<unique_id>
    {
    protected: // structors

        interface () = default;

        virtual ~interface () = default;

    };

    #define xtd_interface xtd::interface<xtd_source_id_hash64>

    //--------------------------------------------------------------------------

} // namespace xtd