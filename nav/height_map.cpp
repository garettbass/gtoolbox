#include <gtoolbox/nav/height_map.h>

namespace nav {

    //--------------------------------------------------------------------------

    height_map::height_map(unsigned row_count, unsigned col_count)
    : map_index(row_count, col_count)
    {
        assert(col_count <= std::numeric_limits<uint16_t>::max());
        assert(row_count <= std::numeric_limits<uint16_t>::max());

        const unsigned layer_size = base::cell_count();

        const unsigned data_size = LAYER_COUNT * layer_size;

        auto* head = new float[data_size];
        auto* tail = head + data_size;

        std::fill(head, tail, -INFINITY);

        _layers[0] = head;
        _layers[1] = head + layer_size;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    height_map::height_map(heights_2D terrain, heights_2D bridges)
    : height_map(
        unsigned(terrain.size()),
        unsigned(terrain.begin()->size()))
    {
        assert(terrain.size() == bridges.size());

        auto terrain_rows = terrain.begin();
        auto bridges_rows = bridges.begin();

        for (unsigned row = 0; row < _row_count; ++row)
        {
            assert(terrain_rows->size() == _col_count);
            assert(bridges_rows->size() == _col_count);

            auto terrain_cols = terrain_rows->begin();
            auto bridges_cols = bridges_rows->begin();

            for (unsigned col = 0; col < _col_count; ++col)
            {
                const auto i = cell_index(row, col);

                _layers[TERRAIN][i] = *terrain_cols;
                _layers[BRIDGES][i] = *bridges_cols;

                ++terrain_cols;
                ++bridges_cols;
            }

            ++terrain_rows;
            ++bridges_rows;
        }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        
    height_map::height_map(height_map&& h)
    : height_map((const height_map&)h) // copy 'h'
    {
        new(&h) height_map(); // reset 'h'
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    height_map&
    height_map::operator = (height_map&& h)
    {
        this->~height_map(); // release resources

        return *new(this) height_map(std::move(h)); // move construct
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    height_map::~height_map()
    {
        delete[] _layers[0];
    }

    //--------------------------------------------------------------------------

} // namespace nav