﻿#pragma once
#include "List.hpp"

namespace WSTL
{
    template<typename T>
    class Queue
    {
    protected:
        List<T> container; // TODO: Replace List<T> with a Vector<T>

    public:
        /**
         * \brief Default Constructor
         */
        Queue() : container() { }

        /**
         * \brief Copy Constructor
         */
        Queue(const Queue<T>& other) : container(other.container) { }

        /**
         * \brief Copy Assignment
         */
        Queue<T>& operator=(const Queue<T>& other)
        {
            if(other == *this) return *this;

            container = other.container;

            return *this;
        }
        
        /**
         * \brief Adds a value to the back of the queue
         */
        void Push(const T& value)
        {
            container.PushBack(value);
        }

        /**
         * \brief Removes and returns the value from the front of the queue
         */
        T Pop()
        {
            T value = container.Front();
            container.PopFront();
            return value;
        }

        /**
         * \brief Returns the value from the front of the queue
         */
        T& Front()
        {
            return container.Front();
        }

        /**
         * \brief Returns if the queue is empty
         */
        bool IsEmpty() const noexcept
        {
            return container.IsEmpty();
        }

        /**
         * \brief Returns the size of the queue
         */
        size_t Size() const noexcept
        {
            return container.Size();
        }

        // Delete move operations
        Queue<T>& operator=(Queue<T>&& other) = delete;
        Queue(Queue<T>&& other) = delete;
        ~Queue() = default;
    };
}
