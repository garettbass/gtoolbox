#pragma once
#include <array>
#include <cassert>

namespace xtd {

    template <typename T, size_t SIZE>
    struct lookup_table : std::array<T, SIZE>
    {
        typedef std::array<T, SIZE> base;

        typedef std::pair<size_t, T> entry;

        typedef std::initializer_list<entry> entry_list;

        lookup_table(entry_list entries, T default_value = T())
        {
            base::fill(default_value);

            for (auto& entry : entries)
            {
                base::at(entry.first) = entry.second;
            }
        }

    };

    template <typename T, size_t SIZE>
    struct lookup_table<T*, SIZE> : std::array<T*, SIZE>
    {
        typedef std::array<T*, SIZE> base;

        typedef std::pair<size_t, T*> entry;

        typedef std::initializer_list<entry> entry_list;

        lookup_table(entry_list entries, T* default_value = nullptr)
        {
            base::fill(default_value);
            
            for (auto* p : *this)
            {
                assert(p == default_value);
            }

            for (auto& entry : entries)
            {
                base::at(entry.first) = entry.second;
            }
        }

    };


} // namespace xtd