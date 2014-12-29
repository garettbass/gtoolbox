#pragma once

namespace xtd {

    template <typename Function>
    struct scope_exit
    {
        Function function;

       ~scope_exit() { function(); }
    };

    template <typename Function>
    inline
    scope_exit<Function>
    make_scope_exit(Function function) { return { function }; }

    #define xtd_scope_exit_id_(counter) xtd_scope_exit_##counter
    #define xtd_scope_exit_id(counter) xtd_scope_exit_id_(counter)
    #define xtd_scope_exit(code) \
        auto xtd_scope_exit_id(__COUNTER__) = \
            ::xtd::make_scope_exit([&](){code;})

} // namespace xtd
