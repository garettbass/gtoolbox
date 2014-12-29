#pragma once

namespace nav {

    nav_inline
    static
    void
    push_heap_front(navigator::open_tile* first, ptrdiff_t len)
    {
        if (len > 1)
        {
            ptrdiff_t p = 0;

            auto* pp = first;

            ptrdiff_t c = 2;

            auto* cp = first + c;

            if (c == len || heap_compare(*cp, *(cp - 1)))
            {
                --c;
                --cp;
            }

            if (heap_compare(*pp, *cp))
            {
                auto temp = *pp;
                do
                {
                    *pp = *cp;

                    pp = cp;

                    p = c;

                    c = (p + 1) * 2;

                    if (c > len)
                        break;

                    cp = first + c;

                    if (c == len || heap_compare(*cp, *(cp - 1)))
                    {
                        --c;
                        --cp;
                    }

                } while (heap_compare(temp, *cp));

                *pp = temp;
            }
        }
    }

    nav_inline
    static
    void
    pop_heap(navigator::open_tile* first, navigator::open_tile* last)
    {
        ptrdiff_t len = last - first;

        if (len > 1)
        {
            std::swap(*first, *--last);
            push_heap_front(first, len-1);
        }
    }

}