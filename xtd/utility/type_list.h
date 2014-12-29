#pragma once
#include <stddef.h>

namespace xtd {

    //--------------------------------------------------------------------------

    template <typename Type = void, typename... Types>
    class type_list_base : type_list_base<Types...>
    {
    protected:

        typedef type_list_base<Types...> base;

        enum : size_t { SIZE = base::SIZE + 1 };

        using base::index_of;

        static constexpr size_t index_of(const Type*, size_t list_size)
        {
            return list_size - SIZE;
        }
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    template <>
    class type_list_base<void>
    {
    protected:

        enum : size_t { SIZE = 0 };

        template <typename T>
        static constexpr size_t index_of(const T*, size_t list_size)
        {
            static_assert(not sizeof(T), "type not found in type list");

            return ~0u;
        }
    };

    //--------------------------------------------------------------------------

    template <typename... Types>
    struct type_list : type_list_base<Types...>
    {
        typedef type_list_base<Types...> base;

        using index_type = size_t;
        
        enum : index_type { SIZE = base::SIZE };

        template <typename T>
        static constexpr index_type index_of()
        {
            return base::index_of((T*)nullptr, SIZE);
        }

        static constexpr index_type size()
        {
            return SIZE;
        }
    };

    //--------------------------------------------------------------------------
    
} // namespace xtd
