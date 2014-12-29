#pragma once
#include <zna/source/thirdParty/gtoolbox/nav/forward/types.h>

namespace nav {

    void log(const char* s);

    void tests();

    void print_heights(const height_map& h, std::FILE* = stdout);

    void print_islands(const map& m, std::FILE* = stdout);

} // namespace nav