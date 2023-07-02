#pragma once
#include "List.hpp"

namespace WSTL
{
    template<typename T>
    class Queue
    {
    protected:
        List<T> container;

    public:
        Queue() : container() { }

        void Push(const T& value)
        {
            container.PushBack(value);
        }

        T Pop()
        {
            T value = container.Front();
            container.PopFront();
            return value;
        }

        T& Front()
        {
            return container.Front();
        }
        
        bool IsEmpty() const noexcept
        {
            return container.IsEmpty();
        }
        
        size_t Size() const noexcept
        {
            return container.Size();
        }
    };
}
