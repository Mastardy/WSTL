﻿#pragma once

#include <algorithm>
#include "WSTL/Defines.hpp"
#include "WSTL/containers/trees/BinaryHeap.hpp"

namespace WSTL
{
    template <typename Value>
    class PriorityQueue
    {
        typedef PriorityQueue<Value> Self;
    public:
        /**
         * \brief Default Constructor
         */
        PriorityQueue() = default;

        /**
         * \brief Constructor that takes a container
         */
        template <class Container>
        PriorityQueue(const Container& container)
        {
            for (var& value : container)
            {
                tree.Insert(value);
            }
        }

        /**
         * \brief Constructor that takes an initializer list
         */
        PriorityQueue(std::initializer_list<Value> list)
        {
            for(var& value : list)
            {
                tree.Insert(value);
            }
        }
        
        /**
         * \brief Copy Constructor
         */
        PriorityQueue(const Self& other) = default;

        /**
         * \brief Move Constructor
         */
        PriorityQueue(Self&& other) = default;

        /**
         * \brief Destructor
         */
        ~PriorityQueue() = default;

        /**
         * \brief Copy Assignment Operator
         */
        PriorityQueue& operator=(const Self& other) = default;

        /**
         * \brief Move Assignment Operator
         */
        PriorityQueue& operator=(Self&& other) = default;

        /**
         * \brief Assignment Operator that takes a container
         */
        template<typename Container>
        PriorityQueue& operator=(const Container& container)
        {
            tree.Clear();
            for (var& value : container)
            {
                tree.Insert(value);
            }
            return *this;
        }

        /**
         * \brief Assignment Operator that takes an initializer list
         */
        PriorityQueue& operator=(std::initializer_list<Value> list)
        {
            tree.Clear();
            for (var& value : list)
            {
                tree.Insert(value);
            }
            return *this;
        }
        
        /**
         * \brief Pushes a new value into the Binary Heap
         */
        void Push(const Value& value)
        {
            tree.Insert(value);
        }

        /**
         * \brief Pushes a new value into the Binary Heap by moving it
         */
        void Push(Value&& value)
        {
            tree.Insert(std::move(value));
        }

        /**
         * \brief Pushes a new value into the Binary Heap by constructing it in place
         */
        template<class...Args>
        void Emplace(Args&&...args)
        {
            tree.Insert(Value(std::forward<Args>(args)...));
        }

        /**
         * \brief Returns the top value of the Binary Heap
         */
        Value Top() const
        {
            return tree.Top();
        }

        /**
         * \brief Returns and deletes the top value of the Binary Heap
         */
        Value Pop()
        {
            return tree.Extract();
        }

        /**
         * \brief Returns the size of the Binary Heap
         */
        ::Size Size()
        {
            return tree.Size();
        }

        /**
         * \brief Returns whether the Binary Heap is empty
         */
        bool IsEmpty()
        {
            return tree.IsEmpty();
        }

        /**
         * \brief Clears the Binary Heap
         */
        void Clear()
        {
            tree.Clear();
        }

    private:
        BinaryHeap<Value> tree;
    };
}