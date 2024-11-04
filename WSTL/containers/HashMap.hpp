#pragma once

#include "WSTL/Defines.hpp"
#include "WSTL/utility/Hash.hpp"

#include "Vector.hpp"
#include "WSTL.hpp"

namespace WSTL
{
    template <typename Key, typename Value>
    class HashMap
    {
    private:
        using Self = HashMap<Key, Value>;
        
        struct Node
        {
        private:
            using Self = Node<Key, Value>;
            
        public:
            Key key;
            Value value;
            Self* pNext;

            Node(const Key& key, const Value& value, Self* pNext = nullptr)
                : key(key), value(value), pNext(pNext) {}

            void Insert(const Key& key, const Value& value)
            {
                while(pNext)
                {
                    pNext = pNext->pNext;
                }

                pNext = new Self(key, value);
            }

            void Remove(const Key& key)
            {
                var pNode = this;
                
                while(pNode->pNext)
                {
                    if(pNode->pNext->key == key)
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
        HashMap(const Self& other) : pBuckets(other.pBuckets), nElements(other.nElements), loadFactor(other.loadFactor) {}

        /**
         * @brief Move Constructor
         */
        HashMap(Self&& other) noexcept : pBuckets(std::move(other.pBuckets)), nElements(std::move(other.nElements)), loadFactor(std::move(other.loadFactor)) {}

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
            if(this == &other) return *this;

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
            if(this == &other) return *this;

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
            var totalSize = 0;
            for(var i = 0; i < pBuckets.Size(); i++)
            {
                var pNode = pBuckets[i];
                while(pNode)
                {
                    totalSize++;
                    pNode = pNode->pNext;
                }
            }
            return totalSize;
        }

        bool IsEmpty() const
        {
            return Size() == 0;
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
            var hash = Hash(&key, sizeof(Key));
            var index = hash % pBuckets.Size();
            var pNode = pBuckets[index];

            while(pNode)
            {
                if(pNode->key == key)
                    return pNode->value;
                pNode = pNode->pNext;
            }
            
            throw std::out_of_range("Key not found");
        }

        const Value& Get(const Key& key) const
        {
            var hash = Hash(&key, sizeof(Key));
            var index = hash % pBuckets.Size();
            var pNode = pBuckets[index];

            while(pNode)
            {
                if(pNode->key == key)
                    return pNode->value;
                pNode = pNode->pNext;
            }
            
            throw std::out_of_range("Key not found");
        }

        void Insert(const Key& key, const Value& value)
        {
            
        }

        void Insert(const Key& key, Value&& value)
        {
            
        }
        
        void Clear()
        {
            for(var i = pBuckets.Size() - 1; i >= 0; i--)
            {
                var p = pBuckets.Erase(i);
                delete p;
            }
        }
        
    private:
        Vector<Node*> pBuckets;
        Size nElements;
        float loadFactor;

        static constexpr Size DEFAULT_CAPACITY = 16;
        static constexpr float DEFAULT_LOAD_FACTOR = 0.75f;
    };
}
