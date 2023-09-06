#pragma once

#include "Vector.hpp"
#include "trees/RBTree.hpp"

namespace WSTL
{
    template<typename Key, typename Value>
    struct MapEntry
    {
        Key key;
        Value value;

        MapEntry() : key(), value() {}
        MapEntry(const Key& key, Value value) : key(key), value(std::move(value)) {}
        MapEntry(const MapEntry& other) : key(other.key), value(other.value) {}
        MapEntry(MapEntry&& other) noexcept : key(std::move(other.key)), value(std::move(other.value)) {}
        MapEntry& operator=(const MapEntry& other) = delete;
        MapEntry& operator=(MapEntry&& other) = delete;
        ~MapEntry() = default;
    };
    
    template<typename Key, typename Value>
    class Map
    {
        typedef MapEntry<const Key, Value> Entry;
        typedef Map<Key, Value> Self;
        
    public:
        Map() = default;
        ~Map() = default;
        
        Map(const Self& other)
        {
            *this = other;
        }

        Map(Self&& other) noexcept
        {
            *this = std::move(other);
        }

        Self& operator=(const Self& other)
        {
            if(this == &other) return *this;
            tree = other.tree;
            return *this;
        }

        Self& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;
            tree = std::move(other.tree);
            return *this;
        }

        bool operator==(const Self& other) const
        {
            return tree == other.tree;
        }

        bool operator!=(const Self& other) const
        {
            return !(*this == other);
        }

        bool operator<(const Self& other) const = delete;
        bool operator>(const Self& other) const = delete;
        bool operator<=(const Self& other) const = delete;
        bool operator>=(const Self& other) const = delete;

        inline Size Size() const
        {
            return tree.Size();
        }

        inline bool IsEmpty() const
        {
            return tree.IsEmpty();
        }

        Value& operator[](const Key& key)
        {
            auto pTemp = tree.Search(key);
            if(pTemp != nullptr) return pTemp->value.value;
            return tree.Insert(key)->value.value;
        }

        Value& At(const Key& key)
        {
            return *this->operator[](key);
        }

        const Value& At(const Key& key) const
        {
            return *this->operator[](key);
        }

        void Insert(const Key& key, const Value& value)
        {
            tree.Insert(key, Entry(key, value));
        }

        void Erase(const Key& key)
        {
            tree.Delete(key);
        }
        
        void Clear()
        {
            tree.Clear();
        }

        template<class... Args>
        void Emplace(const Key& key, Args&&... args)
        {
            tree.Insert(key, Entry(key, Value(std::forward<Args>(args)...)));
        }

        bool Contains(const Key& key) const
        {
            return tree.Search(key) != nullptr;
        }

        Entry* Find(const Key& key)
        {
            return tree.Search(key)->value;
        }

        const Entry* Find(const Key& key) const
        {
            return tree.Search(key)->value;
        }

        Vector<Key> GetKeys() const
        {
            return tree.GetKeys();
        }

        Vector<Value> GetValues() const
        {
            return tree.GetValues();
        }

    private:
        RBTree<Key, MapEntry<const Key, Value>> tree;
    };
}
