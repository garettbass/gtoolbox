#pragma once
#include <string>
#include <vector>
#include <unordered_set>

namespace xtd {

    //--------------------------------------------------------------------------

    class permutor
    {
        std::string _start;

        std::string _string;

        std::vector<unsigned> _indices;

        bool _done = false;

    public: // structors

        permutor() = default;

        permutor(std::string string)
        : _start   (std::move(string))
        , _string  (_start)
        , _indices (_start.size(), '\0')
        {
            unsigned i = 0;

            for (auto& index : _indices) index = i++;
        }

        permutor(const char* string)
        : permutor(std::string(string))
        {}

    public: // properties

        const std::string& string() const { return _string; }

    public: // methods

        bool permute()
        {
            if (_done) return false;

            if (not std::next_permutation(_indices.begin(), _indices.end()))
            {
                _done = true;
                return false;
            }

            char* b = &_string[0];

            for (auto& index : _indices) *b++ = _start[index];

            return true;
        }

        void reset()
        {
            auto start = _start;

            this->~permutor();

            new(this) permutor(start);
        }
    };

    //--------------------------------------------------------------------------

} // namespace xtd