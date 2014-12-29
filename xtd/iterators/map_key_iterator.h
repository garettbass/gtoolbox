#pragma once
#include "./iterator_adapter.h"
#include "./property_first_adapter.h"

namespace xtd {

    //==========================================================================

    template <typename Map>
    using
        map_key_iterator
        = iterator_adapter<
            typename Map::iterator,
            property_first_adapter<typename Map::value_type>>;

    //==========================================================================

    template <typename Map>
    using
        const_map_key_iterator
        = iterator_adapter<
            typename Map::const_iterator,
            property_first_adapter<
                typename std::add_const<
                    typename Map::value_type>::type>>;

    //==========================================================================


} // namespace xtd