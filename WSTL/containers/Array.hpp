#pragma once
#include <stdexcept>

namespace WSTL
{
    template<typename T, Size ArraySize>
    class Array
    {
    public:    
        /**
         * \brief Element Access through []
         */
        T& operator[](Size index)
        {
#if defined(_DEBUG)
            CheckIndexOutOfRange(index);
#endif
            return elements[index];
        }

        /**
         * \brief Element Access through []
         */
        const T& operator[](Size index) const
        {
#if defined(_DEBUG)
            CheckIndexOutOfRange(index);
#endif
            return elements[index];
        }

        /**
         * \brief Returns element at specified index
         */
        T& At(Size index)
        {
            CheckIndexOutOfRange(index);
            return elements[index];
        }

        /**
         * \brief Returns const element at specified index
         */
        const T& At(Size index) const
        {
            CheckIndexOutOfRange(index);
            return elements[index];
        }

        /**
         * \brief Returns first element
         */
        T& Front()
        {
            return elements[0];
        }

        /**
         * \brief Returns first element as a const
         */
        const T& Front() const
        {
            return elements[0];
        }

        /**
         * \brief Returns last element
         */
        T& Back()
        {
            return elements[ArraySize - 1];
        }

        /**
         * \brief Returns last element as a const
         */
        const T& Back() const
        {
            return elements[ArraySize - 1];
        }

        /**
         * \brief Returns pointer to the underlying array
         */
        T* Data()
        {
            return elements;
        }

        /**
         * \brief Returns const pointer to the underlying array
         */
        const T* Data() const
        {
            return elements;
        }

        /**
         * \brief Returns iterator to the beginning of the array
         */
        T* begin()
        {
            return &elements[0];
        }

        /**
         * \brief Returns const iterator to the beginning of the array
         */
        const T* begin() const
        {
            return &elements[0];
        }

        /**
         * \brief Returns iterator to the end of the array
         */
        T* end()
        {
            return &elements[ArraySize - 1];
        }

        /**
         * \brief Returns const iterator to the end of the array
         */
        const T* end() const
        {
            return &elements[ArraySize - 1];
        }

        /**
         * \brief Returns reversed iterator to the end of the array
         */
        T* rbegin()
        {
            return end();
        }

        /**
         * \brief Returns reversed const iterator to the end of the array
         */
        const T* rbegin() const
        {
            return end();
        }

        /**
         * \brief Returns reversed iterator to the end of the array
         */
        T* rend()
        {
            return begin();
        }

        /**
         * \brief Returns reversed const iterator to the end of the array
         */
        const T* rend() const
        {
            return begin();
        }

        /**
         * \brief Returns size of the array
         */
        static constexpr Size Size()
        {
            return ArraySize;
        }

        /**
         * \brief Checks if array is empty
         */
        static constexpr bool IsEmpty()
        {
            return false; // Array can't be empty
        }

        /**
         * \brief Fills array with specified value
         */
        void Fill(const T& value)
        {
            for(::Size i = 0; i < ArraySize; i++)
            {
                elements[i] = value;
            }
        }

        /**
         * \brief Swaps contents of two arrays
         */
        void Swap(Array& other)
        {
            Array temp = other;
            other = *this;
            *this = temp;
        }

        /**
         * \brief Checks if two arrays are equal
         */
        bool operator==(const Array& other) const
        {
            for(::Size i = 0; i < ArraySize; i++)
            {
                if(elements[i] != other.elements[i]) return false;
            }

            return true;
        }

        /**
         * \brief Checks if two arrays are not equal
         */
        bool operator!=(const Array& other) const
        {
            return !(*this == other);
        }
        
        T elements[ArraySize]; // Has to stay public
        
    protected:
        /**
         * \brief Checks if specified index is out of range 
         */
        static inline void CheckIndexOutOfRange(::Size index)
        {
            if(index < 0 || index >= ArraySize) throw std::out_of_range("Index out of range");
        }
    };
}
