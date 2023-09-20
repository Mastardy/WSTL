#pragma once

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
    
    template <typename T>
    class WeakPointer;
    
    template<typename T>
    class SharedPointer
    {
        typedef SharedPointer<T> Self;
        friend class WeakPointer<T>;
        
    public:
        /**
         * \brief Constructs a SharedPointer that owns nothing
         */
        SharedPointer() noexcept : pValue(nullptr), pRefCounter(nullptr) {}

        /**
         * \brief Constructs a SharedPointer that owns nothing
         */
        constexpr SharedPointer(decltype(nullptr)) noexcept : pValue(nullptr), pRefCounter(nullptr) {}

        /**
         * \brief Constructs a SharedPointer and a RefCounter that owns pValue
         */
        explicit SharedPointer(T* pValue) noexcept : pValue(pValue), pRefCounter(new RefCounter()) {}

        /**
         * \brief Constructs a SharedPointer and a RefCounter that owns pValue
         */
        SharedPointer(const WeakPointer<T>& weakPointer)
        {
            pValue = weakPointer.pValue;
            pRefCounter = weakPointer.pRefCounter;
            if(pRefCounter != nullptr) pRefCounter->IncrementRefCount();
        }
        
        /**
         * \brief Copy Constructor
         */
        SharedPointer(const Self& other)
        {
            pValue = other.pValue;
            pRefCounter = other.pRefCounter;
            pRefCounter->IncrementRefCount();
        }

        /**
         * \brief Move constructor
         */
        SharedPointer(Self&& other) noexcept
        {
            pValue = other.pValue;
            pRefCounter = other.pRefCounter;
            other.pValue = nullptr;
            other.pRefCounter = nullptr;
        }

        /**
         * \brief Destructor
         */
        ~SharedPointer()
        {
            if(pRefCounter == nullptr) return;
            
            pRefCounter->DecrementRefCount();
            
            if(pRefCounter->RefCount() == 0)
            {
                delete pValue;
                if(pRefCounter->WeakRefCount() == 0) delete pRefCounter;
            }

            pValue = nullptr;
            pRefCounter = nullptr;
        }

        /**
         * \brief Copy assignment operator
         */
        Self& operator=(const Self& other)
        {
            if(this == &other) return *this;
            
            pValue = other.pValue;
            pRefCounter = other.pRefCounter;
            pRefCounter->IncrementRefCount();
            
            return *this;
        }

        /**
         * \brief Move Assignment for nullptr (Resets the SharedPointer)
         */
        Self& operator=(decltype(nullptr)) noexcept
        {
            this->~SharedPointer();
            return *this;
        }

        /**
         * \brief Move assignment operator for raw pointers
         */
        Self& operator=(T* pValue) noexcept
        {
            this->~SharedPointer();
            this->pValue = pValue;
            this->pRefCounter = new RefCounter();
            return *this;
        }

        /**
         * \brief Move assignment operator
         */
        Self& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;

            pValue = other.pValue;
            pRefCounter = other.pRefCounter;
            other.pValue = nullptr;
            other.pRefCounter = nullptr;

            return *this;
        }

        /**
         * \brief Dereference operator
         */
        T& operator*() noexcept
        {
            return *pValue;
        }

        /**
         * \brief Dereference operator (const)
         */
        const T& operator*() const noexcept
        {
            return *pValue;
        }

        /**
         * \brief Member access operator
         */
        T* operator->() noexcept
        {
            return pValue;
        }

        /**
         * \brief Member access operator (const)
         */
        const T* operator->() const noexcept
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
         * \brief Returns the owned object as a const
         */
        const T* Get() const
        {
            return pValue;
        }

        /**
         * \brief Resets the SharedPointer to nullptr
         */
        void Reset()
        {
            this->~SharedPointer();
        }

        /**
         * \brief Resets the SharedPointer to pValue
         */
        void Reset(decltype(nullptr))
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
            this->pRefCounter = new RefCounter();
        }

        /**
         * \brief Resets the SharedPointer to share another object owned by another SharedPointer
         */
        void Reset(const Self& other)
        {
            this->~SharedPointer();
            this->pValue = other.pValue;
            this->pRefCounter = other.pRefCounter;
            this->pRefCounter->IncrementRefCount();
        }
        
        /**
         * \brief Swaps the contents of two SharedPointers
         */
        void Swap(Self& other) noexcept
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
            if(pRefCounter == nullptr) return 0;
            return pRefCounter->RefCount();
        }

        /**
         * \brief Returns true if the SharedPointer is the only SharedPointer that owns the managed object
         */
        bool IsUnique() const noexcept
        {
            if(pRefCounter == nullptr) return false;
            return pRefCounter->RefCount() == 1;
        }
        
    private:
        T* pValue;
        RefCounter* pRefCounter;
    };

    template<class T>
    class SharedPointer<T[]>
    {
        typedef SharedPointer<T[]> Self;
    public:
        /**
         * \brief Constructs a SharedPointer that owns nothing
         */
        SharedPointer() noexcept : pValue(nullptr), pRefCounter(nullptr) {}

        /**
         * \brief Constructs a SharedPointer that owns nothing
         */
        constexpr SharedPointer(decltype(nullptr)) noexcept : pValue(nullptr), pRefCounter(nullptr) {}

        /**
         * \brief Constructs a SharedPointer and a RefCounter that owns pValue
         */
        explicit SharedPointer(T* pValue) noexcept : pValue(pValue), pRefCounter(new RefCounter()) {}
        
        /**
         * \brief Copy Constructor
         */
        SharedPointer(const Self& other)
        {
            pValue = other.pValue;
            pRefCounter = other.pRefCounter;
            pRefCounter->IncrementRefCount();
        }

        /**
         * \brief Move constructor
         */
        SharedPointer(Self&& other) noexcept
        {
            if(this == &other) return *this;
            
            pValue = other.pValue;
            pRefCounter = other.pRefCounter;
            other.pValue = nullptr;
            other.pRefCounter = nullptr;
            
            return *this;
        }

        /**
         * \brief Destructor
         */
        ~SharedPointer()
        {
            if(pRefCounter == nullptr) return;
            
            pRefCounter->DecrementRefCount();
            
            if(pRefCounter->RefCount() == 0)
            {
                delete[] pValue;
                delete pRefCounter;
            }

            pValue = nullptr;
            pRefCounter = nullptr;
        }

        /**
         * \brief Copy assignment operator
         */
        Self& operator=(const Self& other)
        {
            if(this == &other) return *this;
            
            pValue = other.pValue;
            pRefCounter = other.pRefCounter;
            pRefCounter->IncrementRefCount();
            
            return *this;
        }

        /**
         * \brief Move assignment operator
         */
        Self& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;

            pValue = other.pValue;
            pRefCounter = other.pRefCounter;
            other.pValue = nullptr;
            other.pRefCounter = nullptr;

            return *this;
        }
        
        /**
         * \brief Move Assignment for nullptr (Resets the SharedPointer)
         */
        Self& operator=(decltype(nullptr)) noexcept
        {
            this->~SharedPointer();
            return *this;
        }

        /**
         * \brief 
         */
        Self& operator=(T* pValue) noexcept
        {
            this->~SharedPointer();
            this->pValue = pValue;
            this->pRefCounter = new RefCounter();
            return *this;
        }
        
        /**
         * \brief Dereference operator
         */
        T& operator*() noexcept
        {
            return *pValue;
        }

        /**
         * \brief Dereference operator (const)
         */
        const T& operator*() const noexcept
        {
            return *pValue;
        }

        /**
         * \brief Member access operator
         */
        T* operator->() noexcept
        {
            return pValue;
        }

        /** 
         * \brief Member access operator (const)
         */
        const T* operator->() const noexcept
        {
            return pValue;
        }

        /**
         * \brief Array access operator
         */
        T& operator[](Size index) const noexcept
        {
            return pValue[index];
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
         * \brief Resets the SharedPointer to nullptr
         */
        void Reset() const
        {
            this->~SharedPointer();
        }

        /**
         * \brief Resets the SharedPointer to pValue
         */
        void Reset(decltype(nullptr)) const
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
            this->pRefCounter = new RefCounter();
        }

        /**
         * \brief Resets the SharedPointer to share another object owned by another SharedPointer
         */
        void Reset(const Self& other)
        {
            this->~SharedPointer();
            this->pValue = other.pValue;
            this->pRefCounter = other.pRefCounter;
            this->pRefCounter->IncrementRefCount();
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
            if(pRefCounter == nullptr) return 0;
            return pRefCounter->RefCount();
        }

        /**
         * \brief Returns true if the SharedPointer is the only SharedPointer that owns the managed object
         */
        bool IsUnique() const noexcept
        {
            if(pRefCounter == nullptr) return false;
            return pRefCounter->RefCount() == 1;
        }
        
    private:
        T* pValue;
        RefCounter* pRefCounter;
    };

    template<typename T, class... Args>
    inline std::enable_if_t<!std::is_array_v<T>, SharedPointer<T>> MakeShared(Args&&... args)
    {
        return SharedPointer<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T>
    inline std::enable_if_t<!std::is_array_v<T>, SharedPointer<T>> MakeShared(T* pValue)
    {
        return SharedPointer<T>(pValue);
    }

    template<typename T, class... Args>
    inline std::enable_if_t<std::is_unbounded_array_v<T>, SharedPointer<T>> MakeShared(Args&&... args)
    {
        return SharedPointer<T>(new std::remove_extent_t<T>[args]);
    }

    template<typename T>
    inline std::enable_if_t<std::is_unbounded_array_v<T>, SharedPointer<T>> MakeShared(T* pValue)
    {
        return SharedPointer<T>(pValue);
    }
}
