#include <gtoolbox/nav/height_map.h>
#include <gtoolbox/nav/point.h>

namespace nav {

    // based on: http://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/

    //--------------------------------------------------------------------------
    // signed_area() computes the signed area of a triangle

    nav_inline
    static
    float
    edge_function(const point& a, const point& b, const point& c)
    {
        return
            (b.col - a.col) * (c.row - a.row) -
            (b.row - a.row) * (c.col - a.col);
    }

    //--------------------------------------------------------------------------

    void
    height_map::rasterize(
        const layer_index layer,
        const point&      a,
        const point&      b,
        const point&      c)
    {
        float e = edge_function(a, b, c);

        const bool counter_clockwise = (e < 0);

        e = std::abs(e);

        // fix winding order
        const point& v0 = a;
        const point& v1 = counter_clockwise ? c : b;
        const point& v2 = counter_clockwise ? b : c;

        const float h0 = v0.height;
        const float h1 = v1.height;
        const float h2 = v2.height;

        // compute triangle bounds
        float min_row = std::ceil(math::min(v0.row, v1.row, v2.row));
        float min_col = std::ceil(math::min(v0.col, v1.col, v2.col));
        float max_row = std::floor(math::max(v0.row, v1.row, v2.row));
        float max_col = std::floor(math::max(v0.col, v1.col, v2.col));

        // clip triangle bounds to height map bounds
        min_row = math::clamp<float>(min_row, 0, row_count() - 1);
        min_col = math::clamp<float>(min_col, 0, col_count() - 1);
        max_row = math::clamp<float>(max_row, 0, row_count() - 1);
        max_col = math::clamp<float>(max_col, 0, col_count() - 1);

        point p { min_row, min_col, 0.f };

        float* row = &_layers[layer][cell_index(p.row, p.col)];

        const unsigned row_stride = map_index::row_stride();

        for (p.row = min_row; p.row <= max_row; ++p.row, row += row_stride)
        {
            float* col = row;

            for (p.col = min_col; p.col <= max_col; ++p.col, col += 1)
            {
                // compute edge function of each subtriangle
                float e0 = edge_function(v1, v2, p);
                float e1 = edge_function(v2, v0, p);
                float e2 = edge_function(v0, v1, p);

                // if all are positive, p is inside triangle (v0, v1, v2)
                const bool p_in_triangle = (e0 >= 0 & e1 >= 0 & e2 >= 0);

                if (p_in_triangle)
                {
                    const float hp = (e0 * h0 + e1 * h1 + e2 * h2) / e;

                    *col = math::max(*col, hp);
                }
            }
        }
    }

    //--------------------------------------------------------------------------

} // namespace nav