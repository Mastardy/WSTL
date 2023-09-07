#pragma once

#include "SmartPointers.hpp"

namespace WSTL
{
    template<typename T>
     void Destruct(T* first, T* last)
    {
        for (; first != last; ++first)
        {
            if(std::is_trivial_v<T> == false)
            {
                first->~T();
            }
        }
    }

    template<typename T>
     void Destruct(T* ptr)
    {
        if(std::is_trivial_v<T> == false)
        {
            ptr->~T();
        }
    }

    template<typename T>
     void Free(T** ptr)
    {
        if(*ptr != nullptr)
        {
            delete *ptr;
            *ptr = nullptr;
        }
    }

    template<typename T>
     void FreeArray(T** ptr)
    {
        if(*ptr != nullptr)
        {
            delete[] *ptr;
            *ptr = nullptr;
        }
    }
}