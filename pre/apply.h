#pragma once

#define pre_eval0(...) __VA_ARGS__
#define pre_eval1(...) pre_eval0 (pre_eval0 (pre_eval0 (__VA_ARGS__)))
#define pre_eval2(...) pre_eval1 (pre_eval1 (pre_eval1 (__VA_ARGS__)))
#define pre_eval3(...) pre_eval2 (pre_eval2 (pre_eval2 (__VA_ARGS__)))
#define pre_eval4(...) pre_eval3 (pre_eval3 (pre_eval3 (__VA_ARGS__)))
#define pre_eval(...) pre_eval4 (pre_eval4 (pre_eval4 (__VA_ARGS__)))

#define pre_apply_end(...)
#define pre_apply_out

#define pre_apply_get_end() 0, pre_apply_end
#define pre_apply_next0(test, next, ...) next pre_apply_out
#define pre_apply_next1(test, next) pre_apply_next0 (test, next, 0)
#define pre_apply_next(test, next)  pre_apply_next1 (pre_apply_get_end test, next)

#define pre_apply0(f, x, peek, ...) \
    f(x) pre_apply_next (peek, pre_apply1) (f, peek, __VA_ARGS__)

#define pre_apply1(f, x, peek, ...) \
    f(x) pre_apply_next (peek, pre_apply0) (f, peek, __VA_ARGS__)

#define pre_apply(f, ...) pre_eval (pre_apply1 (f, __VA_ARGS__, (), 0))
