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
        }

        /** 
         * \brief Constructor from SharedPointer
         */
        WeakPointer(const SharedPointer<T>& sharedPointer)
        {
            pRefCounter = sharedPointer.pRefCounter;
            if(pRefCounter != nullptr) pRefCounter->IncrementWeakRefCount();
        }

        /**
         * \brief Copy constructor
         */
        WeakPointer(const Self& other)
        {
            pRefCounter = other.pRefCounter;
            pRefCounter->IncrementWeakRefCount();
        }

        /**
         * \brief Move constructor
         */
        WeakPointer(Self&& other) noexcept
        {
            pRefCounter = other.pRefCounter;
            other.pRefCounter = nullptr;
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
        }

        /**
         * \brief Copy assignment operator
         */
        Self& operator=(const Self& other)
        {
            if(this == &other) return *this;

            pRefCounter = other.pRefCounter;
            pRefCounter->IncrementWeakRefCount();
            
            return *this;
        }

        /**
         * \brief Move assignment operator
         */
        Self& operator=(const SharedPointer<T>& sharedPointer)
        {
            pRefCounter = sharedPointer.pRefCounter;
            pRefCounter->IncrementWeakRefCount();

            return *this;
        }

        /**
         * \brief Move assignment operator
         */
        Self& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;

            pRefCounter = other.pRefCounter;
            other.pRefCounter = nullptr;

            return *this;
        }

        /**
         * \brief Reset the WeakPointer
         */
        void Reset() noexcept
        {
            pRefCounter->DecrementWeakRefCount();
            pRefCounter = nullptr;
        }

        /**
         * \brief Swap the WeakPointer with another WeakPointer
         */
        void Swap(const Self& other) noexcept
        {
            std::swap(pRefCounter, other.pRefCounter);
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
         * \brief Check if the WeakPointer is expired
         */
        bool Expired() const noexcept
        {
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
    };
}