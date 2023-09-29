#pragma once

#include "WSTL/utility/Any.hpp"
#include "WSTL/utility/Optional.hpp"

namespace WSTL
{
    template<class T>
    void Swap(T& a, T& b)
    {
        T temp = a;
        a = b;
        b = temp;
    }
}