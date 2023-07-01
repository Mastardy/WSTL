#pragma once
#include "List.hpp"

namespace WSTL
{
    template<typename T> // TODO: Replace List<T> with a Vector<T>
    class Stack
    {
    protected:
        List<T> container;
        
    public:
        Stack() : container() { }

        void Push(const T& value)
        {
            container.PushBack(value);
        }

        T Pop()
        {
            T value = container.Back();
            std::cout << value << std::endl;
            container.PopBack();
            std::cout << value << std::endl;
            return value;
        }

        T Top()
        {
            return container.Back();
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