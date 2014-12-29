#pragma once
#include <type_traits>
#include <utility>

namespace xtd {

    // type queries ------------------------------------------------------------

    // xtd::trait<T>() -> std::trait<T>::value
    #define xtd_trait(name)           \
        template <typename T>         \
        constexpr bool name()         \
        { return std::name<T>::value; }

        xtd_trait(is_array)
        xtd_trait(is_class)
        xtd_trait(is_enum)
        xtd_trait(is_floating_point)
        xtd_trait(is_function)
        xtd_trait(is_integral)
        xtd_trait(is_lvalue_reference)
        xtd_trait(is_member_function_pointer)
        xtd_trait(is_member_object_pointer)
        xtd_trait(is_pointer)
        xtd_trait(is_rvalue_reference)
        xtd_trait(is_union)
        xtd_trait(is_void)

        xtd_trait(is_arithmetic)
        xtd_trait(is_compound)
        xtd_trait(is_fundamental)
        xtd_trait(is_member_pointer)
        xtd_trait(is_reference)
        xtd_trait(is_scalar)

        xtd_trait(is_abstract)
        xtd_trait(is_const)
        xtd_trait(is_empty)
        xtd_trait(is_literal_type)
        xtd_trait(is_pod)
        xtd_trait(is_polymorphic)
        xtd_trait(is_signed)
        xtd_trait(is_standard_layout)
        xtd_trait(is_trivial)
        xtd_trait(is_trivially_copyable)
        xtd_trait(is_unsigned)
        xtd_trait(is_volatile)

        xtd_trait(has_virtual_destructor)
        xtd_trait(is_constructible)
        xtd_trait(is_copy_assignable)
        xtd_trait(is_copy_constructible)
        xtd_trait(is_destructible)
        xtd_trait(is_default_constructible)
        xtd_trait(is_move_assignable)
        xtd_trait(is_move_constructible)
        xtd_trait(is_nothrow_constructible)
        xtd_trait(is_nothrow_copy_assignable)
        xtd_trait(is_nothrow_copy_constructible)
        xtd_trait(is_nothrow_destructible)
        xtd_trait(is_nothrow_default_constructible)
        xtd_trait(is_nothrow_move_assignable)
        xtd_trait(is_nothrow_move_constructible)
        xtd_trait(is_trivially_constructible)
        xtd_trait(is_trivially_copy_assignable)
        xtd_trait(is_trivially_copy_constructible)
        xtd_trait(is_trivially_destructible)
        xtd_trait(is_trivially_default_constructible)
        xtd_trait(is_trivially_move_assignable)
        xtd_trait(is_trivially_move_constructible)

        xtd_trait(alignment_of)
        xtd_trait(rank)

    #undef xtd_trait

    // xtd::trait<T, U>() -> std::trait<T, U>::value
    #define xtd_trait(name)               \
        template <typename T, typename U> \
        constexpr bool name()             \
        { return std::name<T, U>::value; }

        xtd_trait(is_assignable)
        xtd_trait(is_nothrow_assignable)
        xtd_trait(is_trivially_assignable)

        xtd_trait(is_base_of)
        xtd_trait(is_convertible)
        xtd_trait(is_same)

    #undef xtd_trait

    template <typename T, unsigned I = 0>
    constexpr
    auto
    extent() -> decltype(std::extent<T, I>::value)
    { return std::extent<T, I>::value; }

    // type transformations ----------------------------------------------------

    #define xtd_trait(name) \
        template <typename T>        \
        using name = typename std::name<T>::type;

        xtd_trait(add_const)
        xtd_trait(add_cv)
        xtd_trait(add_volatile)

        xtd_trait(remove_const)
        xtd_trait(remove_cv)
        xtd_trait(remove_volatile)

        xtd_trait(add_pointer)
        xtd_trait(add_lvalue_reference)
        xtd_trait(add_rvalue_reference)
        xtd_trait(decay)
        xtd_trait(make_signed)
        xtd_trait(make_unsigned)
        xtd_trait(remove_all_extents)
        xtd_trait(remove_extent)
        xtd_trait(remove_pointer)
        xtd_trait(remove_reference)
        xtd_trait(underlying_type)

    #undef xtd_trait

    template <typename T>
    using add_const_pointer = add_pointer<add_const<remove_pointer<T>>>;

    template <typename T>
    using add_const_pointer_const = add_const<add_const_pointer<T>>;

    template <typename T>
    using add_const_lvalue_reference = add_const<add_lvalue_reference<T>>;

    template <bool C, typename T, typename F>
    using conditional = typename std::conditional<C, T, F>::type;

    template <typename T>
    using immutable =
        conditional<
            is_fundamental<T>(),
            add_const<T>,
        conditional<
            is_pointer<T>(),
            add_const_pointer_const<T>,
            add_const_lvalue_reference<T>
        >>;

    template <typename T>
    using in = immutable<T>;

    template <typename T>
    using argument =
        conditional<
            sizeof(T) <= sizeof(void*),
            add_const<T>,
            add_const_lvalue_reference<T>
        >;

    //--------------------------------------------------------------------------

    template <typename T>
    struct function_traits
    : function_traits<decltype(&T::operator())>
    {};

    template <typename Return, typename... Args>
    struct function_traits<Return(*)(Args...)>
    {
        enum { ARG_COUNT = sizeof...(Args) };

        typedef Return result_type;

        template <unsigned N>
        using argument_type =
            typename std::tuple_element<N, std::tuple<Args...>>::type;
    };

    template <typename Class, typename Return, typename... Args>
    struct function_traits<Return(Class::*)(Args...)>
    : function_traits<Return(*)(Args...)>
    {};

    template <typename Class, typename Return, typename... Args>
    struct function_traits<Return(Class::*)(Args...) const>
    : function_traits<Return(*)(Args...)>
    {};

} // namespace xtd

/*------------------------------------------------------------------------------
xtd_has_member(foo) declares the following functions:

    template <typename T>
    bool has_foo();

    template <typename T>
    bool has_foo(const T&);
------------------------------------------------------------------------------*/
#define xtd_has_member(X)                                                  \
    template<typename T>                                                   \
    class has_member_##X                                                   \
    {                                                                      \
        struct T0 {};                                                      \
        struct T1 { T0 X; };                                               \
        struct T2 : T, T1 {};                                              \
                                                                           \
        template<typename U, U>                                            \
        struct T3;                                                         \
                                                                           \
        template<typename U>                                               \
        static std::false_type sfinae(T3<int T1::*, &U::X> *);             \
                                                                           \
        template<typename U> static std::true_type sfinae(...);            \
                                                                           \
        using sfinae_type = decltype(sfinae<T2>(0));                       \
                                                                           \
    public:                                                                \
                                                                           \
        static constexpr bool                                              \
        value = is_same<sfinae_type, std::true_type>();                    \
    };                                                                     \
                                                                           \
    template <typename T>                                                  \
    bool has_##X() { return has_member_##X<T>::value; }                    \
                                                                           \
    template <typename T>                                                  \
    bool has_##X(const T&) { return has_member_##X<T>::value; }
