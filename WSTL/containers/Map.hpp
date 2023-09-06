#pragma once

#include "Vector.hpp"
#include "trees/RBTree.hpp"

namespace WSTL
{
        template<typename Key, typename Value>
    class Map
    {
        typedef Map<Key, Value> Self;
        
    public:
        /**
         * \brief Default constructor
         */
        Map() : tree() {}

        /**
         * \brief Copy constructor
         */
        Map(const Self& other)
        {
            *this = other;
        }

        /**
         * \brief Move constructor
         */
        Map(Self&& other) noexcept
        {
            *this = std::move(other);
        }

        /**
         * \brief Destructor
         */
        ~Map() { Clear(); }

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
            other.tree = RBTree<Key, Value>();
            return *this;
        }
            
        // Delete comparison operators because they don't have a purpose for this container
        bool operator<=>(const Self& other) const = delete;

        /**
         * \brief Returns the size of the container
         */
        inline Size Size() const
        {
            return tree.Size();
        }

        /**
         * \brief Returns whether the container is empty
         */
        inline bool IsEmpty() const
        {
            return tree.IsEmpty();
        }

        /**
         * \brief Returns the value associated with the given key
         */
        Value& operator[](const Key& key)
        {
            auto pTemp = tree.Search(key);
            if(pTemp != nullptr) return pTemp->value;
            return tree.Insert(key)->value;
        }

        /**
         * \brief Returns the value associated with the given key
         */
        Value& At(const Key& key)
        {
            return *this->operator[](key);
        }

        /**
         * \brief Returns the value associated with the given key as const
         */
        const Value& At(const Key& key) const
        {
            return *this->operator[](key);
        }

        /**
         * \brief Inserts a new entry into the container
         */
        void Insert(const Key& key, const Value& value)
        {
            tree.Insert(key, value);
        }

        /**
         * \brief Removes an entry from the container
         */
        void Erase(const Key& key)
        {
            tree.Delete(key);
        }

        /**
         * \brief Removes all entries from the container
         */
        void Clear()
        {
            tree.Clear();
        }

        /**
         * \brief Emplaces a new entry into the container
         */
        template<class... Args>
        void Emplace(const Key& key, Args&&... args)
        {
            tree.Insert(key, Entry(key, Value(std::forward<Args>(args)...)));
        }

        /**
         * \brief Returns whether the container contains the given key
         */
        bool Contains(const Key& key) const
        {
            return tree.Search(key) != nullptr;
        }

        /**
         * \brief Returns the entry associated with the given key
         */
        Value* Find(const Key& key)
        {
            return tree.Search(key)->value;
        }

        /**
         * \brief Returns the entry associated with the given key as const
         */
        const Value* Find(const Key& key) const
        {
            return tree.Search(key)->value;
        }

        /**
         * \brief Returns the keys of the container
         */
        Vector<Key> GetKeys() const
        {
            return tree.GetKeys();
        }

        /**
         * \brief Returns the values of the container
         */
        Vector<Value> GetValues() const
        {
            return tree.GetValues();
        }

    private:
        RBTree<Key, Value> tree;
    };
}
