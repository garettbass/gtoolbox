#pragma once
#include <algorithm>
#include <zna/source/thirdParty/gtoolbox/nav/map_index.h>

namespace nav {

    //--------------------------------------------------------------------------

    class height_map : public map_index
    {
        using base = map_index;

        using heights_1D = std::initializer_list<float>;
        using heights_2D = std::initializer_list<heights_1D>;

    private: // fields

        float* _layers[LAYER_COUNT] = { nullptr, nullptr };

    private: // structors

        height_map(const height_map&) = default;

    public: // structors

        height_map() = default;

        height_map(unsigned row_count, unsigned col_count);

        height_map(heights_2D terrain, heights_2D bridges);

        height_map(height_map&& h);

        height_map& operator = (height_map&& h);

       ~height_map();

    public: // iterators

        nav_inline
        const float* begin() const { return _layers[0]; }

        nav_inline
        float* begin() { return _layers[0]; }

        nav_inline
        const float* end() const { return _layers[1] + cell_count(); }

        nav_inline
        float* end() { return _layers[1] + cell_count(); }

    public: // queries

        nav_inline
        float
        height(layer_index layer, unsigned row, unsigned col) const
        { return _layers[layer][cell_index(row, col)]; }

        nav_inline
        float&
        height(layer_index layer, unsigned row, unsigned col)
        { return _layers[layer][cell_index(row, col)]; }

        nav_inline
        const float*
        layer(layer_index layer) const
        { return _layers[layer]; }

        nav_inline
        float*
        layer(layer_index layer)
        { return _layers[layer]; }

    public: // methods

        void
        rasterize(layer_index, const point&, const point&, const point&);

    };

    //--------------------------------------------------------------------------

} // namespace nav