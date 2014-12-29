#if TEST
#include <vector>
#include <catch.h>
#include <zna/source/thirdParty/gtoolbox/xtd/echo.h>
#include "scope_timer.h"

TEST_CASE("scope_timer tests", "[scope_timer]")
{
    std::vector<uint64_t> lines;

    TIMED_SCOPE("create 65535 lines")
    {
        lines.resize(65535);
    }

    TIMED_SCOPE("assign 65535 lines")
    {
        uint64_t offset = 0;
        for (auto& line : lines)
        {
            line = offset;

            offset += 10;
        }
    }

    TIMED_SCOPE("increment 65535 lines")
    {
        for (auto& line : lines)
        {
            line += 1;
        }
    }
}

#endif // TEST