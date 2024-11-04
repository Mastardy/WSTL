#pragma once

#include "WSTL/Defines.hpp"
#include "WSTL/utility/Hash.hpp"

#include "WSTL/containers/Vector.hpp"

namespace WSTL
{
    template <typename T, typename = void>
    struct HasCustomHash : FalseType {};

    template <typename T>
    struct HasCustomHash<T, std::void_t<decltype(std::declval<T>().Hash())>> : TrueType {};
    
    template <typename Key, typename Value>
    class HashMap
    {
    private:
        using Self = HashMap<Key, Value>;

        template <typename K, typename V>
        struct Node
        {
        private:
            using Self = Node<K, V>;

        public:
            K key;
            V value;
            Self* pNext;

            Node(const K& key, const V& value, Self* pNext = nullptr)
                : key(key), value(value), pNext(pNext) {}

            Node(K&& key, V&& value, Self* pNext = nullptr)
                : key(std::move(key)), value(std::move(value)), pNext(pNext) {}

            void Insert(const K& key, const V& value)
            {
                while (pNext)
                {
                    pNext = pNext->pNext;
                }

                pNext = new Self(key, value);
            }

            void Remove(const K& key)
            {
                var pNode = this;

                while (pNode->pNext)
                {
                    if (pNode->pNext->key == key)
                    {
                        var p = pNode->pNext;
                        pNode->pNext = pNode->pNext->pNext;
                        delete p;
                        return;
                    }

                    pNode = pNode->pNext;
                }
            }
        };

    public:
        /**
         * @brief Default Constructor
         */
        HashMap(Size capacity = DEFAULT_CAPACITY, float loadFactor = DEFAULT_LOAD_FACTOR)
            : pBuckets(capacity), nElements(0), loadFactor(loadFactor) {}

        /**
         * @brief Copy Constructor
         */
        HashMap(const Self& other)
            : pBuckets(other.pBuckets), nElements(other.nElements), loadFactor(other.loadFactor) {}

        /**
         * @brief Move Constructor
         */
        HashMap(Self&& other) noexcept
            : pBuckets(std::move(other.pBuckets)), nElements(std::move(other.nElements)),
              loadFactor(std::move(other.loadFactor)) {}

        /**
         * @brief Destructor
         */
        ~HashMap()
        {
            Clear();
        }

        /**
         * @brief Copy Assignment Operator
         */
        Self& operator=(const Self& other)
        {
            if (this == &other) return *this;

            this->~HashMap();
            pBuckets = other.pBuckets;
            nElements = other.nElements;
            loadFactor = other.loadFactor;

            return *this;
        }

        /**
         * @brief Move Assignment Operator
         */
        Self& operator=(Self&& other) noexcept
        {
            if (this == &other) return *this;

            this->~HashMap();
            pBuckets = std::move(other.pBuckets);
            nElements = std::move(other.nElements);
            loadFactor = std::move(other.loadFactor);

            return *this;
        }

        // Delete comparison operators because they don't have a purpose for this container
        bool operator<=>(const Self& other) const = delete;

        Size Size() const
        {
            return nElements;
        }

        bool IsEmpty() const
        {
            return nElements == 0;
        }

        Value& operator[](const Key& key)
        {
            return Get(key);
        }

        const Value& operator[](const Key& key) const
        {
            return Get(key);
        }

        Value& At(const Key& key)
        {
            return Get(key);
        }

        const Value& At(const Key& key) const
        {
            return Get(key);
        }

        Value& Get(const Key& key)
        {
            var index = GetIndex(key);
            
            var pNode = pBuckets[index];

            while (pNode)
            {
                if (pNode->key == key)
                    return pNode->value;
                pNode = pNode->pNext;
            }

            throw std::out_of_range("Key not found");
        }

        const Value& Get(const Key& key) const
        {
            var index = GetIndex(key);
            var pNode = pBuckets[index];

            while (pNode)
            {
                if (pNode->key == key)
                    return pNode->value;
                pNode = pNode->pNext;
            }

            throw std::out_of_range("Key not found");
        }

        void Insert(const Key& key, const Value& value)
        {
            var index = GetIndex(key);
            
            var pNode = pBuckets[index];
            if (pNode == nullptr)
            {
                pBuckets[index] = new Node(key, value);
                return;
            }

            var found = false;

            while (pNode)
            {
                if (pNode->key == key)
                {
                    pNode->value = value;
                    found = true;
                }
                pNode = pNode->pNext;
            }

            if (found) return;
            
            while (pNode->pNext)
            {
                pNode = pNode->pNext;
            }
            pNode->pNext = new Node(key, value);

            nElements++;
            VerifyVectorSize();
        }

        void Insert(const Key& key, Value&& value)
        {
            var index = GetIndex(key);
            
            var pNode = pBuckets[index];
            
            if (pNode == nullptr)
            {
                pBuckets[index] = new Node(key, std::move(value));
                return;
            }

            var found = false;

            while (pNode)
            {
                if (pNode->key == key)
                {
                    pNode->value = value;
                    found = true;
                }
                pNode = pNode->pNext;
            }

            if (found) return;
            
            while (pNode->pNext)
            {
                pNode = pNode->pNext;
            }
            pNode->pNext = new Node(key, std::move(value));
            
            nElements++;
            VerifyVectorSize();
        }

        void Add(const Key& key, const Value& value)
        {
            Insert(key, value);
        }

        void Add(const Key& key, Value&& value)
        {
            Insert(key, std::move(value));
        }

        void Delete(const Key& key)
        {
            var hash = Hash(&key, sizeof(Key));
            var index = hash % pBuckets.Size();

            var pNode = pBuckets[index];

            if (pNode == nullptr) return;

            if (pNode->key == key)
            {
                pBuckets[index] = pNode->pNext;
                delete pNode;
                nElements--;
            }
            else
            {
                while (pNode->pNext)
                {
                    if (pNode->pNext->key == key)
                    {
                        var p = pNode->pNext;
                        pNode->pNext = pNode->pNext->pNext;
                        delete p;
                        nElements--;
                        break;
                    }
                    pNode = pNode->pNext;
                }
            }
        }

        void Erase(const Key& key)
        {
            Delete(key);
        }

        void Remove(const Key& key)
        {
            Delete(key);
        }

        void Clear()
        {
            pBuckets.Clear();

            pBuckets = Vector<Node<Key, Value>*>(DEFAULT_CAPACITY);
            nElements = 0;
        }

        template <class... Args>
        void Emplace(const Key& key, Args&&... args)
        {
            Insert(key, Value(std::forward<Args>(args)...));
        }

        bool Contains(const Key& key)
        {
            var hash = Hash(&key, sizeof(Key));
            var index = hash % pBuckets.Size();

            if (pBuckets[index] == nullptr) return false;

            var pNode = pBuckets[index];
            while (pNode)
            {
                if (pNode->key == key)
                    return true;
                pNode = pNode->pNext;
            }
            return false;
        }

        bool ContainsKey(const Key& key)
        {
            return Contains(key);
        }

        Vector<const Key> GetKeys() const
        {
            Vector<const Key> keys;

            for (var i = 0; i < pBuckets.Size(); i++)
            {
                var pNode = pBuckets[i];
                while (pNode)
                {
                    keys.PushBack(pNode->key);
                    pNode = pNode->pNext;
                }
            }

            return keys;
        }

        Vector<const Value> GetValues() const
        {
            Vector<const Value> values;

            for (var i = 0; i < pBuckets.Size(); i++)
            {
                var pNode = pBuckets[i];
                while (pNode)
                {
                    values.PushBack(pNode->value);
                    pNode = pNode->pNext;
                }
            }
            
            return values;
        }

    private:

        template<typename T>
        static inline ::Size Hash(const T& key)
        {
            if constexpr (HasCustomHash<Key>::value)
            {
                return key.Hash();
            }
            else
            {
                return WSTL::Hash(&key, sizeof(T));
            }
        }

        static inline ::Size Hash(const std::string& str)
        {
            return WSTL::Hash(str.data(), static_cast<::Size>(str.size()));
        }

        ::Size GetIndex(const Key& key) const
        {
            return Hash(key) % pBuckets.Size();
        }
        
        void VerifyVectorSize()
        {
            if (Size() < static_cast<::Size>(static_cast<float>(pBuckets.Size()) * loadFactor)) return;

            var newCapacity = pBuckets.Size() * 2;
            Vector<Node<Key, Value>*> newBuckets(newCapacity);

            for (::Size i = 0; i < pBuckets.Size(); i++)
            {
                var pNode = pBuckets[i];

                while (pNode)
                {
                    var pNext = pNode->pNext;
                    pNode->pNext = nullptr;

                    var newIndex = Hash(pNode->key) % newCapacity;

                    if (newBuckets[newIndex] == nullptr)
                    {
                        newBuckets[newIndex] = pNode;
                    }
                    else
                    {
                        var pTemp = newBuckets[newIndex];
                        while (pTemp->pNext)
                        {
                            pTemp = pTemp->pNext;
                        }
                        pTemp->pNext = pNode;
                    }

                    pNode = pNext;
                }
            }

            pBuckets = std::move(newBuckets);
        }

        Vector<Node<Key, Value>*> pBuckets;
        ::Size nElements;
        float loadFactor;

        static constexpr ::Size DEFAULT_CAPACITY = 16;
        static constexpr float DEFAULT_LOAD_FACTOR = 0.75f;
    };
}
