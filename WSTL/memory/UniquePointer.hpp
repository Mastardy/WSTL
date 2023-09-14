#pragma once
#include "Types.hpp"
#include <type_traits>

namespace WSTL
{
    template<class T>
    class UniquePointer
    {
    public:
        /**
         * \brief Default Constructor
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
        explicit UniquePointer(T* pValue) noexcept : pValue(pValue) { }

        /**
         * \brief Move Constructor for converting an UniquePointer of a derived class to an UniquePointer of a base class 
         */
        template<typename U>
        UniquePointer(UniquePointer<U>&& other) noexcept : pValue(other.Release()) { }
        
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

        /**
         * \brief Move Assignment for nullptr (Resets the UniquePointer)
         */
        UniquePointer<T>& operator=(decltype(nullptr)) noexcept
        {
            Reset();
            return *this;
        }
        
        /**
         * \brief Allows access to owned pointer through *
         */
        T& operator*() const
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
        void Reset(T* pNewValue = nullptr) noexcept
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
            pValue = nullptr;
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
        /**
         * \brief Default Constructor
         */
        constexpr UniquePointer()
        {
            pValue = nullptr;
        }

        /**
         * \brief Default Constructor for nullptr
         */
        constexpr UniquePointer(decltype(nullptr))
        {
            pValue = nullptr;
        }

        /**
         * \brief Default Constructor for an initialized pointer
         */
        explicit UniquePointer(T* pValue) noexcept
        {
            this->pValue = pValue;
        }

        /**
         * \brief Move Constructor
         */
        UniquePointer(UniquePointer<T[]>&& other) noexcept
        {
            pValue = other.Release();
        }

        /**
         * \brief Destructor
         */
        ~UniquePointer()
        {
            Reset();
        }

        /**
         * \brief Move Assignment
         */
        UniquePointer<T[]>& operator=(UniquePointer<T[]>&& other) noexcept
        {
            Reset(other.Release());
            return *this;
        }

        /**
         * \brief Move Assignment for nullptr (Resets the UniquePointer)
         */
        UniquePointer<T[]>& operator=(decltype(nullptr)) noexcept
        {
            Reset();
            return *this;
        }

        /**
         * \brief Array style operator for directly using contents of the pointer
         */
        std::add_lvalue_reference_t<T> operator[](ptrdiff_t i) const
        {
            return pValue[i];
        }

        /**
         * \brief Resets the Unique pointer by deleting the existing pointer and replacing it by the provided one
         */
        void Reset(T* pArray = new T[0]) noexcept
        {
            if(pArray == pValue) return;
            delete[] pValue;
            pValue = pArray;
        }

        /**
         * \brief Deletes the existing pointer
         */
        T* Release()
        {
            T* const pTemp = pValue;
            pValue = new T[0];
            return pTemp;
        }

        /**
         * \brief Swaps pointer between this UniquePointer and the provided one
         */
        void Swap(UniquePointer<T[]>& other) noexcept
        {
            T* const pTemp = pValue;
            pValue = other.pValue;
            other.pValue = pTemp;
        }

        /**
         * \brief Gets the raw pointer
         */
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

    /**
     * \brief Makes an Unique Pointer
     */
    template<typename T, typename... Args>
    inline std::enable_if_t<!std::is_array_v<T>, UniquePointer<T>> MakeUnique(Args&&... args)
    {
        return UniquePointer<T>(new T(std::forward<Args>(args)...));
    }

    /**
     * \brief Makes an Unique Pointer for Unbounded Arrays
     */
    template<typename T, typename... Args>
    inline std::enable_if_t<std::is_unbounded_array_v<T>, UniquePointer<T>> MakeUnique(Size n)
    {
        typedef std::remove_extent_t<T> U; // Get the type without the array extent
        return UniquePointer<T>(new U[n]);
    }
}
