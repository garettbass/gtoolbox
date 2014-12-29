#pragma once

namespace math {

    //==========================================================================

    using float32 = float;
    using float64 = double;

    inline
    constexpr
    float32 f32(double d) { return float32(static_cast<float>(d)); }

    inline
    constexpr
    float64 f64(double d) { return d; }

    //==========================================================================

} // namespace math