#pragma once

#include "trees/RBTree.hpp"

namespace WSTL
{
    template<typename Value>
    class Set
    {
        typedef Set<Value> Self;

    public:
        Set() : tree() {}

        Set(const Self& other)
        {
            *this = other;
        }

        Set(Self&& other) noexcept
        {
            *this = std::move(other);
        }

        ~Set() { Clear(); }

        Self& operator=(const Self& other)
        {
            if(this == &other) return *this;
            tree = other.tree;
            return *this;
        }

        Self& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;
            tree = other.tree;
            other.tree = RBTree<Value, bool>();
            return *this;
        }

        bool operator<=>(const Self& other) const = delete;

        [[nodiscard]]
        Size Size() const
        {
            return tree.Size();
        }

        [[nodiscard]]
        bool Empty() const
        {
            return tree.Empty();
        }

        void Clear()
        {
            tree.Clear();
        }

        [[nodiscard]]
        Value& operator[](::Size index)
        {
            auto pNode = tree.LeftToRight(index);
            if(pNode == nullptr) throw std::out_of_range("Index out of range");
            return pNode->key;
        }

        [[nodiscard]]
        Value& At(::Size index)
        {
            return (*this)[index];
        }

        [[nodiscard]]
        const Value& At(::Size index) const
        {
            return (*this)[index];
        }

        void Insert(const Value& value)
        {
            tree.Insert(value, true);
        }

        void Insert(Value&& value)
        {
            tree.Insert(std::move(value), true);
        }

        void Add(const Value& value)
        {
            Insert(value);
        }

        void Add(Value&& value)
        {
            Insert(std::move(value));
        }

        void Delete(const Value& value)
        {
            tree.Delete(value);
        }

        void Erase(const Value& value)
        {
            Delete(value);
        }
        
        void Remove(const Value& value)
        {
            Delete(value);
        }
        
        [[nodiscard]]
        bool Contains(const Value& value) const
        {
            return tree.Search(value) != nullptr;
        }

        [[nodiscard]]
        const Value& Min() const
        {
            return tree.Min()->key;
        }

        [[nodiscard]]
        const Value& Max() const
        {
            return tree.Max()->key;
        }
        
        Vector<Value> ToVector()
        {
            ::Size size = Size();
            Vector<Value> vec(size);
            for(::Size i = 0; i < size; i++)
            {
                vec.PushBack(At(i));
            }
            return vec;
        }
        
    private:
        RBTree<Value, bool> tree;
    };
}
