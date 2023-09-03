#pragma once

#include <filesystem>
#include <stdexcept>

#include "memory/Memory.hpp"
#include "Types.hpp"

namespace WSTL
{
    template <typename T, Size FixedSize>
    class FixedVector
    {
    public:
        /**
         * \brief Default Constructor
         */
        FixedVector()
        {
            Initialize();
        }

        /**
         * \brief Constructor with default values for count elements
         */
        FixedVector(Size count, const T& value) : FixedVector()
        {
            if(count < 0 || count > FixedSize) throw std::out_of_range("Count Out Of Range.");

            for(::Size i = 0; i < count; i++)
            {
                pBegin[i] = value;
            }
        }

        /**
         * \brief Copy Construcor
         */
        FixedVector(const FixedVector<T, FixedSize>& other) : FixedVector()
        {
            for(::Size i = 0; i < FixedSize; i++)
            {
                pBegin[i] = other.pBegin[i];
            }
        }

        /**
         * \brief Move Constructor
         */
        FixedVector(FixedVector<T, FixedSize>&& other) noexcept : FixedVector()
        {
            if(this == &other) return;
            
            pBegin = other.pBegin;
            pEnd = other.pEnd;

            other.pBegin = nullptr;
            other.pEnd = nullptr;
        }

        /**
         * \brief Constructor with initializer list
         */
        FixedVector(std::initializer_list<T> init) : FixedVector()
        {
            if(init.size() > FixedSize) throw std::out_of_range("Initializer List Size Out Of Range.");

            for(::Size i = 0; i < init.size(); i++)
            {
                pBegin[i] = *(init.begin() + i);
                ++pEnd;
            }
        }

        /**
         * \brief Destructor
         */
        ~FixedVector()
        {
            Destroy();
        }

        /**
         * \brief Copy Assignment Operator
         */
        FixedVector<T, FixedSize>& operator=(const FixedVector<T, FixedSize>& other)
        {
            if(this == &other) return *this;

            for(::Size i = 0; i < FixedSize; i++)
            {
                pBegin[i] = other.pBegin[i];
            }

            return *this;
        }

        /**
         * \brief Move Assignment Operator
         */
        FixedVector<T, FixedSize>& operator=(FixedVector<T, FixedSize>&& other) noexcept
        {
            if(this == &other) return *this;

            Destroy();

            pBegin = other.pBegin;
            pEnd = other.pEnd;

            other.pBegin = nullptr;
            other.pEnd = nullptr;

            return *this;
        }

        /**
         * \brief Assignment Operator with initializer list
         */
        FixedVector<T, FixedSize>& operator=(std::initializer_list<T> init)
        {
            if(init.size() > FixedSize) throw std::out_of_range("Initializer List Size Out Of Range.");

            for(::Size i = 0; i < init.size(); i++)
            {
                pBegin[i] = *(init.begin() + i);
                ++pEnd;
            }

            return *this;
        }

        /**
         * \brief Returns the underlying array
         */
        inline T* Data() noexcept
        {
            return pBegin;
        }

        
        /**
         * \brief Returns the underlying array as const
         */
        inline const T* Data() const noexcept
        {
            return pBegin;
        }

        /**
         * \brief Returns the element at the specified index
         */
        inline T& operator[](Size index)
        {
            CheckIndexOutOfRange(index);
            return pBegin[index];
        }

        /**
         * \brief Returns the element at the specified index as const
         */
        inline const T& operator[](Size index) const
        {
            CheckIndexOutOfRange(index);
            return pBegin[index];
        }

        /**
         * \brief Returns the element at the specified index
         */
        inline T& At(Size index)
        {
            return operator[](index);
        }

        /**
         * \brief Returns the element at the specified index as const
         */
        inline const T& At(Size index) const
        {
            return operator[](index);
        }

        /**
         * \brief Returns the first element of the vector
         */
        inline T& Front()
        {
            return operator[](0);
        }

        /**
         * \brief Returns the first element of the vector as const
         */
        inline const T& Front() const
        {
            return operator[](0);
        }

        /**
         * \brief Returns the last element of the vector
         */
        inline T& Back()
        {
            return operator[](Size() - 1);
        }

        /**
         * \brief Returns the last element of the vector as const
         */
        inline const T& Back() const
        {
            return operator[](Size() - 1);
        }

        /**
         * \brief Adds an element to the end of the vector with the specified value
         */
        void PushBack(const T& value)
        {
            CheckForCapacity();

            *pEnd = value;
            ++pEnd;
        }

        /**
         * \brief Adds an element to the end of the vector
         */
        T& PushBack()
        {
            CheckForCapacity();

            ++pEnd;
            return Back();
        }

        /**
         * \brief Adds an uninitialized element to the end of the vector
         */
        void* PushBackUninitialized()
        {
            CheckForCapacity();

            return pEnd++;
        }

        /**
         * \brief Adds an element to the end of the vector by moving the specified value
         */
        void PushBack(T&& value)
        {
            CheckForCapacity();

            *pEnd = std::move(value);
            ++pEnd;
        }

        /**
         * \brief Removes the last element of the vector
         */
        void PopBack()
        {
            if(pEnd == pBegin) return;
            Destruct(pEnd--);
        }

        /**
         * \brief Constructs an element in-place at the end of the vector using the specified arguments 
         */
        template<class... Args>
        T& EmplaceBack(Args&&... args)
        {
            CheckForCapacity();

            *pEnd = T(std::forward<Args>(args)...);
            ++pEnd;
            return Back();
        }

        /**
         * \brief Constructs an element in-place at the specified position of the vector using the specified arguments 
         */
        template<class... Args>
        T* Emplace(Size index, Args&&... args)
        {
            CheckIndexOutOfRange(index);
            CheckForCapacity();

            auto pEmplace = pBegin + index;
            MoveContentsForward(pEmplace);
            *pEmplace = T(std::forward<Args>(args)...);
            ++pEnd;
            return pEmplace;
        }

        /**
         * \brief Inserts an element at the specified position of the vector
         */
        T* Insert(Size index, const T& value)
        {
            CheckIndexOutOfRange(index);
            CheckForCapacity();

            auto pInsert = pBegin + index;
            MoveContentsForward(pInsert);
            *pInsert = value;
            ++pEnd;
            return pInsert;
        }

        /**
         * \brief Inserts an element at the specified position of the vector by moving the specified value
         */
        T* Insert(Size index, T&& value)
        {
            CheckIndexOutOfRange(index);
            CheckForCapacity();

            auto pInsert = pBegin + index;
            MoveContentsForward(pInsert);
            *pInsert = std::move(value);
            ++pEnd;
            return pInsert;
        }

        /**
         * \brief Inserts count elements with the specified value at the specified position of the vector
         */
        T* Insert(Size index, Size count, const T& value)
        {
            CheckIndexOutOfRange(index);
            CheckForCapacity(count);

            auto insertPos = pBegin + index;
            MoveContentsForward(insertPos, count);
            for(auto pTemp = insertPos; pTemp != pBegin + index + count; ++pTemp)
            {
                *pTemp = value;
            }
            pEnd += count;
            return insertPos;
        }

        /**
         * \brief Inserts the elements of the initializer list at the specified position of the vector
         */
        T* Insert(Size index, std::initializer_list<T> init)
        {
            auto count = init.size();
            
            CheckIndexOutOfRange(index);
            CheckForCapacity(count);

            auto insertPos = pBegin + index;
            MoveContentsForward(insertPos, count);
            pEnd += count;
            for(::Size i = 0; i < count; i++)
            {
                *insertPos = *(init.begin() + i);
                ++insertPos;
            }
            return pBegin + index;
        }

        /**
         * \brief Removes the element at the specified position of the vector
         */
        T* Erase(Size index)
        {
            CheckIndexOutOfRange(index);

            MoveContentsBackward(pBegin + index);

            Destruct(--pEnd);
            return pBegin + index;
        }

        /**
         * \brief Removes the elements in the specified range of the vector
         */
        T* Erase(const T* first, const T* last)
        {
            CheckRangeOutOfRange(first, last);

            auto count = last - first;

            MoveContentsBackward(first, count);
            Destruct(pEnd - count, pEnd);
            pEnd -= count;

            return const_cast<T*>(first);
        }

        /**
         * \brief Removes all elements from the vector
         */
        void Clear() noexcept
        {
            Destroy();
            Initialize();
        }

        /**
         * \brief Swaps the contents of the vector with the specified vector
         */
        void Swap(FixedVector<T, FixedSize>& other) noexcept
        {
            std::swap(pBegin, other.pBegin);
            std::swap(pEnd, other.pEnd);
        }

        /**
         * \brief Returns whether the vector is empty
         */
        inline bool IsEmpty() const noexcept
        {
            return Size() == 0;
        }

        /**
         * \brief Returns wether the vector is full
         */
        inline bool IsFull() const noexcept
        {
            return Size() == FixedSize;
        }
        
        /**
         * \brief Returns the number of elements in the vector
         */      
        inline Size Size() const noexcept
        {
            return pEnd - pBegin;
        }

        /**
         * \brief Returns iterator begin
         */
        T* begin()
        {
            return pBegin;
        }

        /**
         * \brief Returns const iterator begin
         */
        const T* begin() const
        {
            return pBegin;
        }

        /**
         * \brief Returns iterator end
         */
        T* end()
        {
            return pEnd;
        }

        /**
         * \brief Returns const iterator end
         */
        T* end() const
        {
            return pEnd;
        }
        
    protected:
        /**
         * \brief Initializes the underlying array
         */
        void Initialize()
        {
            pBegin = new T[FixedSize];            
            pEnd = pBegin;
        }

        /**
         * \brief Deletes underlying array
         */
        void Destroy()
        {
            FreeArray(pBegin);
        }

        /**
         * \brief Checks if the specified index is out of vector range
         */
        static void CheckIndexOutOfRange(::Size index)
        {
            if(index < 0 || index >= FixedSize) throw std::out_of_range("Index Out Of Range.");
        }

        /**
         * \brief Checks if the specified range is out of vector range
         */
        inline void CheckRangeOutOfRange(const T* first, const T* last) const
        {
            if(first > last) throw std::invalid_argument("`first` is greater than `last`");
            if(first < pBegin || first > pEnd || last > pEnd ) throw std::out_of_range("Index out of Range");
        }

        /**
         * \brief Checks if the vector has enough capacity to add the specified number of elements
         */
        static void CheckForCapacity(::Size count = 1)
        {
            if(Size() + count > FixedSize) throw std::out_of_range("Vector is Full.");
        }

        /**
         * \brief Moves the contents of the vector backward, starting from the specified position
         */
        void MoveContentsBackward(const T* pos, Size count = 1)
        {
            for(T* mover = pBegin + (pos - pBegin); mover != pEnd - count; ++mover)
            {
                *mover = *(mover + count);
            }
        }
        
        /**
         * \brief Moves the contents of the vector forward, starting from the specified position
         */
        void MoveContentsForward(const T* pos, Size count = 1)
        {
            for(auto mover = pEnd + count - 1; mover != pos; --mover)
            {
                *mover = *(mover - count);
            }
        }
        
    private:
        T* pBegin;
        T* pEnd;
    };
}
