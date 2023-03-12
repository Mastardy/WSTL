#pragma once
#include <iostream>

namespace WSTL
{
    template<typename T>
    class UniquePointer
    {
    public:
        /**
         * \brief Default constructor
         */
        UniquePointer()
        {
            this->pValue = nullptr;
        }

        /**
         * \brief Default Constructor
         */
        UniquePointer(std::nullptr_t)
        {
            this->pValue = nullptr;
        }
        
        /**
         * \brief Explicit constructor for creating an Unique Pointer
         * \param pValue Allocated object
         */
        explicit UniquePointer(T* pValue)
        {
            this->pValue = pValue;
        }

        /**
         * \brief Move Constructor
         */
        UniquePointer(UniquePointer&& other) noexcept
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
        UniquePointer& operator=(UniquePointer&& other) noexcept
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
}
