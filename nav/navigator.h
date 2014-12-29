#pragma once
#include <vector>
#include <zna/source/thirdParty/gtoolbox/nav/distance.h>
#include <zna/source/thirdParty/gtoolbox/nav/map.h>
#include <zna/source/thirdParty/gtoolbox/nav/point.h>

namespace nav {

    //--------------------------------------------------------------------------

    class navigator
    {
    public: // types

        using cell_map = std::array<tile, LAYER_COUNT>;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        class tile_iterator
        {
            const navigator* _navigator = nullptr;

            tile _tile = unreachable();

        public: // structors

            tile_iterator() = default;

            tile_iterator(const navigator* navigator, tile tile)
            : _navigator(navigator), _tile(tile)
            {}

        public: // operators

            bool operator == (const tile_iterator& i) const
            { return _tile == i._tile; }

            bool operator != (const tile_iterator& i) const
            { return _tile != i._tile; }

            tile operator * () const
            { return _tile; }

            const tile& operator -> () const
            { return _tile; }

            tile_iterator& operator ++ ()
            { _tile = _navigator->next(_tile); return *this; }
        };

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        class tile_path
        {
            tile_iterator _begin, _end;

        public:
            tile_path() = default;

            tile_path(const navigator* n)
            : _begin(n, n->_start), _end(n, n->next(n->_stop))
            {}

            tile_iterator begin() const { return _begin; }

            tile_iterator end() const { return _end; }
        };

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        struct open_tile : tile
        {
            float g = 0;

            float f = 0;

            open_tile() = default;

            open_tile(nav::tile t, float g, float f)
            : tile(t), g(g), f(f)
            {}
        };

    private: // fields

        const map* _map = nullptr;

        cell_map* _cell_map = nullptr;

        open_tile* _open_head = nullptr;

        open_tile* _open_tail = nullptr;

        nav::tile _start;

        nav::tile _stop;

    public: // tests

        static void tests();

    private: // structors

        navigator(const navigator&) = default;

    public: // structors

        navigator() = default;

        navigator(const map&);

        navigator(navigator&&);

        navigator& operator = (navigator&&);

       ~navigator();

    public: // properties

        bool
        empty() const
        { return _start == _stop; }

        const nav::map&
        map() const
        { assert(_map); return *_map; }

        const tile&
        start() const
        { return _start; }

        const tile&
        stop() const
        { return _stop; }

        tile_path
        path() const
        { return this; }

    public: // methods

        void
        clear();

        bool
        navigate(tile start, tile goal);

        nav_inline
        nav::tile
        next(tile t) const
        { return _cell_map[_map->cell_index(t)][t.layer]; }

        nav_inline
        bool
        visited(tile t) const
        { return _cell_map[_map->cell_index(t)][t.layer].visited; }

    private: // methods

        nav_inline
        bool
        a_star(const tile, const tile);

        nav_inline
        bool
        done() const;

        nav_inline
        void
        push(
            nav::tile   next,
            nav::tile   curr,
            const float curr_g,
            const float curr_f);

        nav_inline
        open_tile
        pop();

        nav_inline
        void
        set_next(tile curr, tile next)
        { _cell_map[_map->cell_index(curr)][curr.layer] = next; }

        nav_inline
        void
        reverse();
    };

    //--------------------------------------------------------------------------

    nav_inline
    std::vector<tile>&
    operator << (std::vector<tile>& out, const navigator::tile_path& path)
    {
        for (auto tile : path) out.push_back(tile);

        return out;
    }

    //--------------------------------------------------------------------------

} // namespace nav