#pragma once

#include "WSTL/Defines.hpp"
#include "WSTL/containers/Vector.hpp"
#include "WSTL/containers/trees/RBTree.hpp"

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
         Size Size() const
        {
            return tree.Size();
        }

        /**
         * \brief Returns whether the container is empty
         */
         bool IsEmpty() const
        {
            return tree.IsEmpty();
        }

        /**
         * \brief Returns the value associated with the given key
         */
        Value& operator[](const Key& key)
        {
            var pTemp = tree.Search(key);
            if(pTemp != nullptr) return pTemp->value;
            var pNew = tree.Insert(key);
            return pNew->value;
        }

        /**
         * \brief Returns the value associated with the given key
         */
        Value& At(const Key& key)
        {
            return (*this)[key];
        }

        /**
         * \brief Returns the value associated with the given key as const
         */
        const Value& At(const Key& key) const
        {
            return (*this)[key];
        }

        /**
         * \brief Inserts a new entry into the container
         */
        void Insert(const Key& key, const Value& value)
        {
            tree.Insert(key, value);
        }
        void Insert(const Key& key, Value&& value)
        {
            tree.Insert(key, std::move(value));
        }
        void Add(const Key& key, const Value& value)
        {
            Insert(key, value);
        }
        void Add(const Key& key, Value&& value)
        {
            Insert(key, std::move(value));
        }

        /**
         * \brief Removes an entry from the container
        */
        void Delete(const Key& key)
        {
            tree.Delete(key);
        }
        void Erase(const Key& key)
        {
            Delete(key);
        }
        void Remove(const Key& key)
        {
            Delete(key);
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
        void Emplace(const Key& key, Args&&... args)  // NOLINT(cppcoreguidelines-missing-std-forward)
        {
            tree.Insert(key, Entry(key, Value(std::forward<Args>(args)...)));
        }

        /**
         * \brief Returns whether the container contains the given key
         */
        bool Contains(const Key& key)
        {
            return tree.Search(key) != nullptr;
        }
        bool ContainsKey(const Key& key)
        {
            return Contains(key);
        }

        /**
         * \brief Returns the entry associated with the given key
        */
        Value& Get(const Key& key)
        {
            var pTemp = tree.Search(key);
            if(pTemp == nullptr) throw std::out_of_range("Key not found");
            return pTemp->value;
        }
        Value& GetValue(const Key& key)
        {
            return Get(key);
        }

        /**
         * \brief Returns the entry associated with the given key as const
         */
        const Value& Find(const Key& key) const
        {
            return tree.Search(key)->value;
        }
        const Value& FindKey(const Key& key) const
        {
            return Find(key);
        }
        const Value& Get(const Key& key) const
        {
            return Find(key);
        }
        const Value& GetValue(const Key& key) const
        {
            return Find(key);
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
