#pragma once

#include <WSTL/utility/Hash.hpp>

#include "Vector.hpp"

namespace WSTL
{
    template <typename Key, typename Value>
    class HashMap
    {
    private:
        struct Node
        {
            Key key;
            Value value;
            Node* pNext;

            Node(const Key& key, const Value& value, Node* pNext = nullptr)
                : key(key), value(value), pNext(pNext) {}
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
        HashMap(const HashMap& other) : pBuckets(other.pBuckets), nElements(other.nElements), loadFactor(other.loadFactor) {}

        /**
         * @brief Move Constructor
         */
        HashMap(HashMap&& other) noexcept : pBuckets(std::move(other.pBuckets)), nElements(std::move(other.nElements)), loadFactor(std::move(other.loadFactor)) {}

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
        HashMap& operator=(const HashMap& other)
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
        HashMap& operator=(HashMap&& other) noexcept
        {
            if(this == &other) return *this;

            this->~HashMap();
            pBuckets = std::move(other.pBuckets);
            nElements = std::move(other.nElements);
            loadFactor = std::move(other.loadFactor);
            
            return *this;
        }

        // Delete comparison operators because they don't have a purpose for this container
        bool operator<=>(const HashMap& other) const = delete;

        Value& operator[](const Key& key)
        {
            
        }

        const Value& operator[](const Key& key) const
        {
            
        }

        void Clear()
        {
            for(auto i = pBuckets.Size() - 1; i >= 0; i--)
            {
                auto p = pBuckets.Erase(i);
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
