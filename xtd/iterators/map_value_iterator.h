#pragma once
#include "./iterator_adapter.h"
#include "./pair_second_adapter.h"

namespace xtd {

    //==========================================================================

    template <typename Map>
    using
        map_value_iterator
        = iterator_adapter<
            typename Map::iterator,
            pair_second_adapter<typename Map::value_type>>;

    //==========================================================================

    template <typename Map>
    using
        const_map_value_iterator
        = iterator_adapter<
            typename Map::const_iterator,
            pair_second_adapter<
                typename std::add_const<
                    typename Map::value_type>::type>>;

    //==========================================================================

} // namespace xtd