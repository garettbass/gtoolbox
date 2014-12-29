#pragma once
#include "range/array_range.h"

namespace xtd {

    template <typename Iterator>
    struct range
    {
        Iterator first;

        Iterator last;

        range(Iterator first, Iterator last)
        : first(first)
        , last(last)
        {}

        range(Iterator first, size_t size)
        : first(first)
        , last(first)
        { std::advance(last, size); }

        Iterator begin() const { return first; }

        Iterator end() const { return last; }

        bool empty() const { return first == last; }

        size_t size() const { return std::distance(first, last); }
    };

} // namespace xtd
