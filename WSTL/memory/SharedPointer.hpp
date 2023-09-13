#pragma once
#include "Types.hpp"

namespace WSTL
{
    struct RefCounter
    {
    public:
        RefCounter() noexcept : refCount(1), weakRefCount(0) { }
        
        Size RefCount() const noexcept
        {
            return refCount;
        }

        Size WeakRefCount() const noexcept
        {
            return weakRefCount;
        }

        void IncrementRefCount() noexcept
        {
            ++refCount;
        }

        void DecrementRefCount() noexcept
        {
            --refCount;
        }

        void IncrementWeakRefCount() noexcept
        {
            ++weakRefCount;
        }

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
        SharedPointer() noexcept : pValue(nullptr), pRefCount(nullptr) {}

        constexpr SharedPointer(decltype(nullptr)) noexcept : pValue(nullptr), pRefCount(nullptr) {}

        explicit SharedPointer(T* pValue) noexcept : pValue(pValue), pRefCount(new RefCounter()) {}

        SharedPointer(const Self& other)
        {
            pValue = other.pValue;
            pRefCount = other.pRefCount;
            pRefCount->IncrementRefCount();
        }

        SharedPointer(Self&& other) noexcept
        {
            if(this == &other) return *this;
            
            pValue = other.pValue;
            pRefCount = other.pRefCount;
            other.pValue = nullptr;
            other.pRefCount = nullptr;
            
            return *this;
        }
        
    private:
        T* pValue;
        RefCounter* pRefCount;
    };
}
