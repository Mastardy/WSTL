#pragma once
#include <iostream>

#include "Types.hpp"

namespace WSTL
{
    struct RefCounter
    {
    public:
        RefCounter() noexcept : refCount(1), weakRefCount(0) { }

        /**
         * \brief Returns the number of SharedPointers that share ownership of the managed object
         */
        Size RefCount() const noexcept
        {
            return refCount;
        }

        /**
         * \brief Returns the number of WeakPointers that share ownership of the managed object
         */
        Size WeakRefCount() const noexcept
        {
            return weakRefCount;
        }

        /**
         * \brief Increments the number of SharedPointers that share ownership of the managed object
         */
        void IncrementRefCount() noexcept
        {
            ++refCount;
        }

        /**
         * \brief Decrements the number of SharedPointers that share ownership of the managed object
         */
        void DecrementRefCount() noexcept
        {
            --refCount;
        }

        /**
         * \brief Increments the number of WeakPointers that share ownership of the managed object
         */
        void IncrementWeakRefCount() noexcept
        {
            ++weakRefCount;
        }

        /**
         * \brief Decrements the number of WeakPointers that share ownership of the managed object
         */
        void DecrementWeakRefCount() noexcept
        {
            --weakRefCount;
        }
        
    private:
        Size refCount;
        Size weakRefCount;
    };
    
    template<typename T>
    class SharedPointer
    {
        typedef SharedPointer<T> Self;
    public:
        /**
         * \brief Constructs a SharedPointer that owns nothing
         */
        SharedPointer() noexcept : pValue(nullptr), pRefCount(nullptr) {}

        /**
         * \brief Constructs a SharedPointer that owns nothing
         */
        constexpr SharedPointer(decltype(nullptr)) noexcept : pValue(nullptr), pRefCount(nullptr) {}

        /**
         * \brief Constructs a SharedPointer and a RefCounter that owns pValue
         */
        explicit SharedPointer(T* pValue) noexcept : pValue(pValue), pRefCount(new RefCounter()) {}

        /**
         * \brief Copy Constructor
         */
        SharedPointer(const Self& other)
        {
            pValue = other.pValue;
            pRefCount = other.pRefCount;
            pRefCount->IncrementRefCount();
        }

        /**
         * \brief Move constructor
         */
        SharedPointer(Self&& other) noexcept
        {
            if(this == &other) return *this;
            
            pValue = other.pValue;
            pRefCount = other.pRefCount;
            other.pValue = nullptr;
            other.pRefCount = nullptr;
            
            return *this;
        }

        /**
         * \brief Destructor
         */
        ~SharedPointer()
        {
            if(pRefCount == nullptr) return;

            std::cout << "Deleted SharedPointer" << std::endl;
            
            pRefCount->DecrementRefCount();
            
            if(pRefCount->RefCount() == 0)
            {
                delete pValue;
                delete pRefCount;
            }

            pValue = nullptr;
            pRefCount = nullptr;
        }

        /**
         * \brief Copy assignment operator
         */
        SharedPointer& operator=(const Self& other)
        {
            if(this == &other) return *this;
            
            pValue = other.pValue;
            pRefCount = other.pRefCount;
            pRefCount->IncrementRefCount();
            
            return *this;
        }

        /**
         * \brief Move assignment operator
         */
        SharedPointer& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;

            pValue = other.pValue;
            pRefCount = other.pRefCount;
            other.pValue = nullptr;
            other.pRefCount = nullptr;

            return *this;
        }

        /**
         * \brief Dereference operator
         */
        T& operator*() const noexcept
        {
            return *pValue;
        }

        /**
         * \brief Member access operator
         */
        T* operator->() const noexcept
        {
            return pValue;
        }

        /**
         * \brief Returns true if the SharedPointer owns an object
         */
        explicit operator bool() const noexcept
        {
            return pValue != nullptr;
        }

        /**
         * \brief Returns the owned object
         */
        T* Get()
        {
            return pValue;
        }

        /**
         * \brief Releases ownership of the managed object
         */
        void Release() const
        {
            this->~SharedPointer();
        }

        /**
         * \brief Resets the SharedPointer to pValue
         */
        void Reset(T* pValue)
        {
            this->~SharedPointer();
            this->pValue = pValue;
            this->pRefCount = new RefCounter();
        }

        /**
         * \brief Resets the SharedPointer to share another object owned by another SharedPointer
         */
        void Reset(const Self& other)
        {
            this->~SharedPointer();
            this->pValue = other.pValue;
            this->pRefCount = other.pRefCount;
            this->pRefCount->IncrementRefCount();
        }
        
        /**
         * \brief Swaps the contents of two SharedPointers
         */
        void Swap(const Self& other) noexcept
        {
            if(this == &other) return;
            
            T* const pTemp = pValue;
            pValue = other.pValue;
            other.pValue = pTemp;
        }

        /**
         * \brief Returns the number of SharedPointers that share ownership of the managed object
         */
        Size UseCount() const noexcept
        {
            return pRefCount->RefCount();
        }

        /**
         * \brief Returns true if the SharedPointer is the only SharedPointer that owns the managed object
         */
        bool IsUnique() const noexcept
        {
            return pRefCount->RefCount() == 1;
        }
        
    private:
        T* pValue;
        RefCounter* pRefCount;
    };
}
