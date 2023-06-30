#pragma once
#include <stdexcept>

namespace WSTL
{
    template<typename T, size_t ArraySize>
    class Array
    {
    public:    
        /**
         * \brief Element Access through []
         */
        T& operator[](size_t index)
        {
            if(index < 0 || index >= ArraySize) throw std::out_of_range("Index out of range");
            return elements[index];
        }

        /**
         * \brief Element Access through []
         */
        const T& operator[](size_t index) const
        {
            if(index < 0 || index >= ArraySize) throw std::out_of_range("Index out of range");
            return elements[index];
        }

        T& At(size_t index)
        {
            if(index < 0 || index >= ArraySize) throw std::out_of_range("Index out of range");
            return elements[index];
        }

        const T& At(size_t index) const
        {
            if(index < 0 || index >= ArraySize) throw std::out_of_range("Index out of range");
            return elements[index];
        }

        T& Front()
        {
            return elements[0];
        }

        const T& Front() const
        {
            return elements[0];
        }

        T& Back()
        {
            return elements[ArraySize - 1];
        }

        const T& Back() const
        {
            return elements[ArraySize - 1];
        }

        T* Data()
        {
            return elements;
        }

        const T* Data() const
        {
            return elements;
        }

        T* begin()
        {
            return &elements[0];
        }

        const T* begin() const
        {
            return &elements[0];
        }

        T* rbegin()
        {
            return &elements[ArraySize - 1];
        }

        const T* rbegin() const
        {
            return &elements[ArraySize - 1];
        }

        T* rend()
        {
            return &elements[0];
        }

        const T* rend() const
        {
            return &elements[0];
        }

        T* end()
        {
            return &elements[ArraySize - 1];
        }

        const T* end() const
        {
            return &elements[ArraySize - 1];
        }

        static constexpr size_t Size()
        {
            return ArraySize;
        }

        static constexpr bool IsEmpty()
        {
            return ArraySize == 0;
        }

        void Fill(const T& value)
        {
            for(size_t i = 0; i < ArraySize; i++)
            {
                elements[i] = value;
            }
        }

        void Swap(Array& other)
        {
            Array temp = other;
            other = *this;
            *this = temp;
        }

        bool operator==(const Array& other) const
        {
            for(size_t i = 0; i < ArraySize; i++)
            {
                if(elements[i] != other.elements[i]) return false;
            }

            return true;
        }

        bool operator!=(const Array& other) const
        {
            return !(*this == other);
        }
        
        T elements[ArraySize];
    };
}
