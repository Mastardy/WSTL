#pragma once
#include "List.hpp"

namespace WSTL
{
    template<typename T>
    class Stack
    {
    protected:
        List<T> container; // TODO: Replace List<T> with a Vector<T>
        
    public:
        /**
         * \brief Default Constructor
         */
        Stack() : container() { }

        /**
         * \brief Copy Constructor
         */
        Stack(const Stack<T>& other) : container(other.container) { }

        /**
         * \brief Copy Assignment
         */
        Stack<T>& operator=(const Stack<T>& other)
        {
            if(this == &other) return *this;

            container = other.container;

            return *this;
        }
        
        /**
        * \brief Adds a value to the top of the queue
        */
        void Push(const T& value)
        {
            container.PushBack(value);
        }

        /**
         * \brief Removes and returns the value from the top of the stack
         */
        T Pop()
        {
            T value = container.Back();
            container.PopBack();
            return value;
        }

        /**
         * \brief Returns the value from the top of the stack
         */
        T Top()
        {
            return container.Back();
        }

        /**
         * \brief Returns if the stack is empty
         */
        bool IsEmpty() const noexcept
        {
            return container.IsEmpty();
        }

        /**
         * \brief Returns the size of the stack
         */
        size_t Size() const noexcept
        {
            return container.Size();
        }
    };
}