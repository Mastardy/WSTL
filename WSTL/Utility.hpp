#pragma once

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