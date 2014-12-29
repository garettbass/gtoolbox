#pragma once

namespace nav {

    nav_inline
    static
    bool heap_compare(
        const navigator::open_tile& a,
        const navigator::open_tile& b)
    {
        return a.f > b.f;
    }


    nav_inline
    static
    void
    push_heap_back(
        navigator::open_tile* first,
        navigator::open_tile* last,
        ptrdiff_t len)
    {
        if (len > 1)
        {
            len = (len - 2) / 2;

            auto* ptr = first + len;

            if (heap_compare(*ptr, *--last))
            {
                auto temp = *last;
                do
                {
                    *last = *ptr;

                    last = ptr;

                    if (len == 0)
                        break;

                    len = (len - 1) / 2;

                    ptr = first + len;

                } while (heap_compare(*ptr, temp));

                *last = temp;
            }
        }
    }

    nav_inline
    static
    void
    push_heap(navigator::open_tile* first, navigator::open_tile* last)
    {
        nav::push_heap_back(first, last, last - first);
    }

}