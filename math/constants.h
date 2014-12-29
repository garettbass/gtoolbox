#pragma once
#include <zna/source/thirdParty/gtoolbox/math/forward/float.h>

namespace math {

    //--------------------------------------------------------------------------

    constexpr const float PI = 3.141592653589793f;

    constexpr const float RADIANS_PER_DEGREE = PI / 180.f;

    constexpr const float DEGREES_PER_RADIAN = 180.f / PI;

    constexpr const float RADIANS_PER_REVOLUTION = 2.f * PI;

    constexpr const float DEGREES_PER_REVOLUTION = 360.f;

    //--------------------------------------------------------------------------

    class identity_t final {};

    extern identity_t identity;

    //--------------------------------------------------------------------------

} // namespace math