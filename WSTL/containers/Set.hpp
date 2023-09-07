#pragma once

#include "trees/RBTree.hpp"

namespace WSTL
{
    template<typename Value>
    class Set
    {
        typedef Set<Value> Self;

    public:
        /**
         * \brief Default constructor
         */
        Set() : tree() {}

        /**
         * \brief Copy constructor
         */
        Set(const Self& other)
        {
            *this = other;
        }

        /**
         * \brief Move constructor
         */
        Set(Self&& other) noexcept
        {
            *this = std::move(other);
        }

        /**
         * \brief Destructor
         */
        ~Set() { Clear(); }

        /**
         * \brief Copy assignment operator
         */
        Self& operator=(const Self& other)
        {
            if(this == &other) return *this;
            tree = other.tree;
            return *this;
        }

        /**
         * \brief Move assignment operator
         */
        Self& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;
            tree = other.tree;
            other.tree = RBTree<Value, bool>();
            return *this;
        }

        /**
         * \brief Comparison operator
         */
        bool operator<=>(const Self& other) const = delete;

        /**
         * \brief Returns size of the set
         */
        [[nodiscard]]
        Size Size() const
        {
            return tree.Size();
        }

        /**
         * \brief Checks if set is empty
         */
        [[nodiscard]]
        bool Empty() const
        {
            return tree.Empty();
        }

        /**
         * \brief Clears the set by deleting all elements
         */
        void Clear()
        {
            tree.Clear();
        }

        /**
         * \brief Returns value at specified index
         */
        [[nodiscard]]
        Value& operator[](::Size index)
        {
            auto pNode = tree.LeftToRight(index);
            if(pNode == nullptr) throw std::out_of_range("Index out of range");
            return pNode->key;
        }

        /**
         * \brief Returns value at specified index
         */
        [[nodiscard]]
        Value& At(::Size index)
        {
            return (*this)[index];
        }

        /**
         * \brief Returns value at specified index as const
         */
        [[nodiscard]]
        const Value& At(::Size index) const
        {
            return (*this)[index];
        }

        /**
         * \brief Inserts a new entry into the set
         */
        void Insert(const Value& value)
        {
            tree.Insert(value, true);
        }

        /**
         * \brief Inserts a new entry into the set
         */
        void Insert(Value&& value)
        {
            tree.Insert(std::move(value), true);
        }

        /**
         * \brief Adds a new entry into the set
         */
        void Add(const Value& value)
        {
            Insert(value);
        }

        /**
         * \brief Adds a new entry into the set
         */
        void Add(Value&& value)
        {
            Insert(std::move(value));
        }

        /**
         * \brief Deletes an entry from the set
         */
        void Delete(const Value& value)
        {
            tree.Delete(value);
        }

        /**
         * \brief Deletes an entry from the set
         */
        void Erase(const Value& value)
        {
            Delete(value);
        }

        /**
         * \brief Deletes an entry from the set
         */
        void Remove(const Value& value)
        {
            Delete(value);
        }

        /**
         * \brief Checks if set contains specified value
         */
        [[nodiscard]]
        bool Contains(const Value& value)
        {
            return tree.Search(value) != nullptr;
        }

        /**
         * \brief Returns the minimum value in the set
         */
        [[nodiscard]]
        const Value& Min()
        {
            return tree.Min()->key;
        }

        /**
         * \brief Returns the maximum value in the set
         */
        [[nodiscard]]
        const Value& Max()
        {
            return  tree.Max()->key;
        }

        /**
         * \brief Returns a Vector containing all values in the set in ascending order
         */
        [[nodiscard]]
        Vector<Value> ToVector()
        {
            ::Size size = Size();
            Vector<Value> vec(size);
            for(::Size i = 0; i < size; i++)
            {
                vec[i] = At(i);
            }
            return vec;
        }
        
    private:
        RBTree<Value, bool> tree;
    };
}
