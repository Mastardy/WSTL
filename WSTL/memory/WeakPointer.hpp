#pragma once

#include "WSTL/utility/Utility.hpp"
#include "WSTL/memory/SharedPointer.hpp"

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

        /** 
         * \brief Constructor from SharedPointer
         */
        WeakPointer(const SharedPointer<T>& sharedPointer)
        {
            pRefCounter = sharedPointer.pRefCounter;
            pValue = sharedPointer.pValue;
            if(pRefCounter != nullptr) pRefCounter->IncrementWeakRefCount();
        }

        /**
         * \brief Copy constructor
         */
        WeakPointer(const Self& other)
        {
            pRefCounter = other.pRefCounter;
            pValue = other.pValue;
            if(pRefCounter != nullptr) pRefCounter->IncrementWeakRefCount();
        }

        /**
         * \brief Move constructor
         */
        WeakPointer(Self&& other) noexcept
        {
            pRefCounter = other.pRefCounter;
            pValue = other.pValue;
            other.pRefCounter = nullptr;
            other.pValue = nullptr;
        }

        /**
         * \brief Destructor
         */
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
            pValue = nullptr;
        }

        /**
         * \brief Copy assignment operator
         */
        Self& operator=(const Self& other)
        {
            if(this == &other) return *this;

            pRefCounter = other.pRefCounter;
            pRefCounter->IncrementWeakRefCount();
            pValue = other.pValue;
            
            return *this;
        }

        /**
         * \brief Copy assignment operator from SharedPointer
         */
        Self& operator=(const SharedPointer<T>& sharedPointer)
        {
            pRefCounter = sharedPointer.pRefCounter;
            pRefCounter->IncrementWeakRefCount();
            pValue = sharedPointer.pValue;
            
            return *this;
        }

        /**
         * \brief Move assignment operator
         */
        Self& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;

            pRefCounter = other.pRefCounter;
            pValue = other.pValue;
            other.pRefCounter = nullptr;
            other.pValue = nullptr;
            
            return *this;
        }

        /**
         * \brief Reset the WeakPointer
         */
        void Reset() noexcept
        {
            pRefCounter->DecrementWeakRefCount();
            pRefCounter = nullptr;
            pValue = nullptr;
        }

        /**
         * \brief Swap the WeakPointer with another WeakPointer
         */
        void Swap(Self& other) noexcept
        {
            auto temp = pRefCounter;
            pRefCounter = other.pRefCounter;
            other.pRefCounter = temp;

            WSTL::Swap(pValue, other.pValue);
        }

        /**
         * \brief Get the number of SharedPointer that point to the same object
         */
        Size UseCount() const noexcept
        {
            if(pRefCounter == nullptr) return 0;
            return pRefCounter->RefCount();
        }

        /**
         * \brief Get the number of WeakPointer that point to the same object
         */
        Size WeakUseCount() const noexcept
        {
            if(pRefCounter == nullptr) return 0;
            return pRefCounter->WeakRefCount();
        }

        /**
         * \brief Check if the WeakPointer is expired
         */
        bool Expired() const noexcept
        {
            if(pRefCounter == nullptr) return true;
            return pRefCounter->RefCount() == 0;
        }

        /**
         * \brief Lock the WeakPointer and return a SharedPointer
         */
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