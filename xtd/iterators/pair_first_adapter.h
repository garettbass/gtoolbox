#pragma once
#include <type_traits>

namespace xtd {

    //==========================================================================

    template <typename Pair>
    struct property_first_adapter
    {
        typedef typename Pair::first_type                       value_type;
        typedef typename std::add_const<value_type>::type const_value_type;
        
        typedef       value_type&       reference;
        typedef const_value_type& const_reference;
        
        const_reference operator() (Pair& pair) const { return pair.first; }
              reference operator() (Pair& pair)       { return pair.first; }
    };

    //==========================================================================

} // namespace xtd