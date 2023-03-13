#pragma once
#include <iostream>

namespace WSTL
{
    template<class T>
    class UniquePointer
    {
    public:
        /**
         * \brief Default constructor
         */
        constexpr UniquePointer()
        {
            pValue = nullptr;
        }

        /**
         * \brief Default Constructor
         */
        constexpr UniquePointer(decltype(nullptr))
        {
            pValue = nullptr;
        }
        
        /**
         * \brief Explicit constructor for creating an Unique Pointer
         */
        explicit UniquePointer(T* pValue) noexcept
        {
            this->pValue = pValue;
        }

        /**
         * \brief Move Constructor
         */
        UniquePointer(UniquePointer<T>&& other) noexcept
        {
            pValue = other.Release();
        }
        
        /**
         * \brief Destructor to safely delete the pointer
         */
        ~UniquePointer()
        {
            delete pValue;
        }

        
        /**
         * \brief Move assignment
         */
        UniquePointer& operator=(UniquePointer<T>&& other) noexcept
        {
            if(this == &other) return *this;

            Reset(other.Release());
            
            return *this;
        }

        std::add_lvalue_reference_t<T> operator*() const
        {
            return *pValue;
        }
        
        /**
         * \brief Allows access to owned pointer through ->
         */
        T* operator->() const noexcept
        {
            return pValue;
        }
        
        /**
         * \brief Allows the use of an UniquePointer as a boolean
         */
        explicit operator bool() const
        {
            return pValue != nullptr; 
        }

        /**
         * \brief Get the pointer
         */
        T* Get()
        {
            return pValue;
        }

        /**
         * \brief Deletes the owned pointer and takes ownership of the passed pointer
         */
        void Reset(T* pNewValue = new T()) noexcept
        {
            if(pValue == pNewValue) return;

            delete pValue;
            pValue = pNewValue;
        }

        /**
         * \brief Forgets the owned pointer without deleting it
         */
        T* Release() noexcept
        {
            T* const pTemp = pValue;
            pValue = new T();
            return pTemp;
        }

        /**
         * \brief Exchanges the owned pointer between two UniquePointer objects
         */
        void Swap(UniquePointer<T>& other)
        {
            T* const pTemp = pValue;
            pValue = other.pValue;
            other.pValue = pTemp;
        }
        
        // Delete these functions because copying is dangerous as it might leave to dangling pointers!
        UniquePointer(const UniquePointer&) = delete;
        UniquePointer& operator=(const UniquePointer&) = delete;
        UniquePointer& operator=(T* pValue) = delete;
        
    private:
        T* pValue;
    };

    /**
     * \brief Specialization for unbounded arrays
     */
    template<class T>
    class UniquePointer<T[]>
    {
    public:
        constexpr UniquePointer()
        {
            pValue = nullptr;
        }

        constexpr UniquePointer(decltype(nullptr))
        {
            pValue = nullptr;
        }

        explicit UniquePointer(T* pValue) noexcept
        {
            this->pValue = pValue;
        }

        UniquePointer(UniquePointer<T[]>&& other) noexcept
        {
            pValue = other.Release();
        }

        ~UniquePointer()
        {
            Reset();
        }

        UniquePointer<T[]>& operator=(UniquePointer<T[]>&& other) noexcept
        {
            Reset(other.Release());
            return *this;
        }

        UniquePointer<T[]>& operator=(decltype(nullptr)) noexcept
        {
            Reset();
            return *this;
        }

        std::add_lvalue_reference_t<T> operator[](ptrdiff_t i) const
        {
            return pValue[i];
        }

        void Reset(T* pArray = new T[0]) noexcept
        {
            if(pArray == pValue) return;
            delete[] pValue;
            pValue = pArray;
        }

        T* Release()
        {
            T* const pTemp = pValue;
            pValue = new T[];
            return pTemp;
        }

        void Swap(UniquePointer<T[]> other) noexcept
        {
            T* const pTemp = pValue;
            pValue = other.pValue;
            other.pValue = pTemp;
        }

        T* Get() const noexcept
        {
           return pValue; 
        }

        // Delete these functions because copying is dangerous as it might leave to dangling pointers!
        UniquePointer(const UniquePointer<T[]>&) = delete;
        UniquePointer& operator=(const UniquePointer<T[]>&) = delete;
        UniquePointer& operator=(T* pValue) = delete;
        
    private:
        T* pValue;
    };
}
