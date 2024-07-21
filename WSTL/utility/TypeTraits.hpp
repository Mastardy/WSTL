#pragma once

namespace WSTL
{
#pragma region Bool Constant
    template <class Type, Type Value>
    struct IntegralConstant
    {
        static constexpr Type value = Value;

        typedef Type ValueType;
        typedef IntegralConstant type;

        constexpr operator ValueType() const noexcept { return value; }

        [[nodiscard]]
        constexpr ValueType operator()() const noexcept { return value; }
    };

    template <bool Value>
    typedef IntegralConstant<bool, Value> BoolConstant;

    typedef BoolConstant<true> TrueType;
    typedef BoolConstant<false> FalseType;

#pragma endregion

#pragma region Conditional

    template <bool Test, class Type1, class Type2>
    struct Conditional
    {
        typedef Type1 type;
    };

    template <class Type1, class Type2>
    struct Conditional<false, Type1, Type2>
    {
        typedef Type2 type;
    };

    /**
     * @brief Returns Type1 if Test is true, otherwise returns Type2
     */
    template <bool Test, class Type1, class Type2>
    typedef typename Conditional<Test, Type1, Type2>::type ConditionalT;

#pragma endregion

#pragma region Enable If

    template <bool Test, class Type = void>
    struct EnableIf {};

    template <class Type>
    struct EnableIf<true, Type>
    {
        typedef Type type;
    };

    /**
     * @brief Enable Type if Test is true
     */
    template <bool Test, class Type = void>
    typedef typename EnableIf<Test, Type>::type EnableIfT;

#pragma endregion

#pragma region Is Type
    
    template <class>
    inline constexpr bool IsArrayV = false;
    template <class Type, Size Num>
    inline constexpr bool IsArrayV<Type[Num]> = true;
    /**
     * @brief Checks if given Type is an Array
     */
    template <class Type>
    inline constexpr bool IsArrayV<Type[]> = true;
    template <class Type>
    struct IsArray : BoolConstant<IsArrayV<Type>> {};

    template <class>
    inline constexpr bool IsBoundedArrayV = false;
    template <class Type, Size Num>
    inline constexpr bool IsBoundedArrayV<Type[Num]> = true;
    template <class Type>
    struct IsBoundedArray : BoolConstant<IsBoundedArrayV<Type>> {};

    template <class>
    inline constexpr bool IsUnboundedArrayV = false;
    template <class Type>
    inline constexpr bool IsUnboundedArrayV<Type[]> = true;
    template <class Type>
    struct IsUnboundedArray : BoolConstant<IsUnboundedArrayV<Type>> {};

    template <class Type>
    struct IsTrivial : BoolConstant<__is_trivially_constructible(Type) && __is_trivially_copyable(Type)> {};
    template <class Type>
    inline constexpr bool IsTrivialV = IsTrivial<Type>::value;
    inline constexpr bool IsTrivialV2 = IsTrivial<int>::value;
    
#pragma endregion
}
