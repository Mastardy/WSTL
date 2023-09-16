#pragma once

#include "SharedPointer.hpp"

namespace WSTL
{
    struct RefCounter;
    template <typename T>
    class SharedPointer;
    
    template<typename T>
    class WeakPointer
    {
        typedef WeakPointer<T> Self;
        friend class SharedPointer<T>;
        
    public:
        /**
         * \brief Default constructor
         */
        WeakPointer()
        {
            pRefCounter = nullptr;
            pValue = nullptr;
        }

        WeakPointer(const SharedPointer<T>& sharedPointer)
        {
            pRefCounter = sharedPointer.pRefCounter;
            if(pRefCounter != nullptr) pRefCounter->IncrementWeakRefCount();
            pValue = sharedPointer.pValue;
        }
        
        WeakPointer(const Self& other)
        {
            pRefCounter = other.pRefCounter;
            pRefCounter->IncrementWeakRefCount();
            pValue = other.pValue;
        }

        WeakPointer(Self&& other) noexcept
        {
            pRefCounter = other.pRefCounter;
            pValue = other.pValue;
            other.pRefCounter = nullptr;
            other.pValue = nullptr;
        }

        ~WeakPointer()
        {
            if (pRefCounter)
            {
                pRefCounter->DecrementWeakRefCount();
                if (pRefCounter->WeakRefCount() == 0 && pRefCounter->RefCount() == 0)
                {
                    delete pRefCounter;
                    pRefCounter = nullptr;
                }
            }
        }

        Self& operator=(const Self& other)
        {
            if(this == &other) return *this;

            pRefCounter = other.pRefCounter;
            pRefCounter->IncrementWeakRefCount();
            pValue = other.pValue;
            
            return *this;
        }

        Self& operator=(const SharedPointer<T>& sharedPointer)
        {
            pRefCounter = sharedPointer.pRefCounter;
            pRefCounter->IncrementWeakRefCount();
            pValue = sharedPointer.pValue;

            return *this;
        }

        Self& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;

            pRefCounter = other.pRefCounter;
            pValue = other.pValue;
            other.pRefCounter = nullptr;
            other.pValue = nullptr;

            return *this;
        }

        void Reset() noexcept
        {
            pRefCounter->DecrementWeakRefCount();
            pRefCounter = nullptr;
            pValue = nullptr;
        }

        void Swap(const Self& other) noexcept
        {
            std::swap(pRefCounter, other.pRefCounter);
            std::swap(pValue, other.pValue);
        }

        Size UseCount() const noexcept
        {
            if(pRefCounter == nullptr) return 0;
            return pRefCounter->RefCount();
        }

        bool Expired() const noexcept
        {
            if(pValue == nullptr) return true;
            return pRefCounter->RefCount() == 0;
        }

        SharedPointer<T> Lock() const noexcept
        {
            if(Expired()) return SharedPointer<T>();
            return SharedPointer<T>(*this);
        }
        
    private:
        RefCounter* pRefCounter;
        T* pValue;
    };
}