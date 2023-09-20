#pragma once

#include "Any.hpp"
#include "Optional.hpp"

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