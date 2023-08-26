﻿#pragma once
#include "memory/Memory.hpp"

namespace WSTL
{
    template <typename T>
    class Vector
    {
    public:
        /**
         * \brief Default Constructor
         */
        Vector() : pBegin(nullptr), pEnd(nullptr), capacity(0) { }

        /**
         * \brief Constructor with a start size
         */
        explicit Vector(size_t count)
        {
            Initialize(count);
        }

        /**
         * \brief Constructor with a start size and default value
         */
        Vector(size_t count, const T& value) : Vector(count)
        {
            for (size_t i = 0; i < count; i++)
            {
                pBegin[i] = value;
            }
        }

        /**
         * \brief Copy Constructor
         */
        Vector(const Vector<T>& other) : Vector(other.Size())
        {
            for (size_t i = 0; i < other.Size(); i++)
            {
                pBegin[i] = other[i];
            }
        }

        /**
         * \brief Move Constructor
         */
        Vector(Vector<T>&& other) noexcept
        {
            pBegin = other.pBegin;
            pEnd = other.pEnd;
            capacity = other.capacity;

            other.pBegin = nullptr;
            other.pEnd = nullptr;
            other.capacity = 0;
        }

        /**
         * \brief Constructor with an initializer list
         */
        Vector(std::initializer_list<T> init) : Vector(init.size())
        {
            for(size_t i = 0; i < init.size(); i++)
            {
                pBegin[i] = *(init.begin() + i);
            }
        }

        /**
         * \brief Default Destructor
         */
        ~Vector()
        {
            Destroy();
        }

        /**
         * \brief Copy Assignment Operator
         */
        Vector<T>& operator=(const Vector<T>& other)
        {
            if(this == &other) return *this;

            Destroy();            
            Initialize(other.Size());

            for(size_t i = 0; i < other.Size(); i++)
            {
                pBegin[i] = other[i];
            }
            
            return *this;
        }

        /**
         * \brief Move Assignment Operator
         */
        Vector<T>& operator=(Vector<T>&& other) noexcept
        {
            if(this == &other) return *this;

            Swap(other);

            return *this;
        }

        /**
         * \brief Assignment Operator with an initializer list
         */
        Vector<T>& operator=(std::initializer_list<T> init)
        {
            Destroy();
            Initialize(init.size());

            for(size_t i = 0; i < init.size(); i++)
            {
                pBegin[i] = *(init.begin() + i);
            }

            return *this;
        }

        /**
         * \brief Assigns a new size to the vector, replacing its current contents
         */
        void Assign(size_t count, const T& value)
        {
            Destroy();
            Initialize(count);
            for(size_t i = 0; i < count; i++)
            {
                pBegin[i] = value;
            }
        }

        /**
         * \brief Replaces the contents with `value`, in the range `[first, last]`
         */
        void Assign(T* first, T* last, const T& value)
        {
            CheckIndexOutOfRange(first, last);
            if(first == last) return;

            while(first != last && first != pEnd)
            {
                *first = value;
                ++first;
            }
        }

        /**
         * \brief Assigns new contents, replacing its current contents, from an initializer list
         */
        void Assign(std::initializer_list<T> init)
        {
            Destroy();
            Initialize(init.size());

            for(size_t i = 0; i < init.size(); i++)
            {
                pBegin[i] = *(init.begin() + i);
            }
        }

        /**
         * \brief Returns a pointer to the underlying container
         */
        T* Data() noexcept
        {
            return pBegin;
        }

        /**
         * \brief Returns a const pointer to the underlying container
         */
        const T* Data() const noexcept
        {
            return pBegin;
        }

        /**
         * \brief Returns the element at the specified index
         */
        T& operator[](size_t index)
        {
            if(index < 0 || index >= Size()) throw std::out_of_range("Index Out Of Range");

            return pBegin[index];
        }

        /**
         * \brief Returns the element as const at the specified index
         */
        const T& operator[](size_t index) const
        {
            CheckIndexOutOfRange(index);

            return pBegin[index];
        }

        /**
         * \brief Returns the element at the specified index
         */
        T& At(size_t index)
        {
            return (*this)[index];
        }

        /**
         * \brief Returns the element as const at the specified index
         */
        const T& At(size_t index) const
        {
            return (*this)[index];
        }

        /**
         * \brief Returns the first element of the vector
         */
        inline T& Front()
        {
            return (*this)[0];
        }

        /**
         * \brief Returns the first element of the vector as const
         */
        inline const T& Front() const
        {
            return (*this)[0];
        }

        /**
         * \brief Returns the last element of the vector
         */
        inline T& Back()
        {
            return (*this)[Size() - 1];
        }

        /**
         * \brief Returns the last element of the vector as const
         */
        inline const T& Back() const
        {
            return (*this)[Size() - 1];
        }

        /**
         * \brief Adds a new element at the end of the vector by copying value
         */
        void PushBack(const T& value)
        {
            CheckForCapacity();

            *pEnd = value;
            ++pEnd;
        }

        /**
         * \brief Adds a new element at the end of the vector and returns a reference to it
         */
        T& PushBack()
        {
            CheckForCapacity();

            auto& value = *pEnd;
            ++pEnd;
            return value;
        }

        /**
         * \brief Adds a new uninitialized element at the end of the vector and returns a pointer to it
         */
        void* PushBackUninitialized()
        {
            CheckForCapacity();

            return pEnd++;
        }

        /**
         * \brief Adds a new element at the end of the vector by moving the given value
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
         * \brief Adds a new element at the end of the vector by constructing it in-place using the provided arguments
         */
        template<class... Args> 
        T& EmplaceBack(Args&&... args)
        {
            CheckForCapacity();

            *pEnd = T(std::forward<Args>(args)...);
            auto emplacePos = pEnd++;
            return *emplacePos;
        }

        /**
         * \brief Adds a new element in the specified position of the vector by constructing it in-place using the
         * provided arguments
         */
        template<class... Args>
        T* Emplace(size_t index, Args&&... args)
        {
            CheckIndexOutOfRange(index);
            CheckForCapacity();

            auto emplacePos = pBegin + index;
            MoveContentsForward(emplacePos);
            *emplacePos = T(std::forward<Args>(args)...);
            return emplacePos;
        }

        /**
         * \brief Inserts a new element at the specified position of the vector by copying value
         */
        T* Insert(size_t index, const T& value)
        {
            CheckIndexOutOfRange(index);
            CheckForCapacity();

            auto insertPos = pBegin + index;
            MoveContentsForward(insertPos);
            *insertPos = value;
            return insertPos;            
        }

        /**
         * \brief Inserts a new element at the specified position of the vector by moving the given value
         */
        T* Insert(size_t index, T&& value)
        {
            CheckIndexOutOfRange(index);
            CheckForCapacity();

            auto insertPos = pBegin + index;
            MoveContentsForward(insertPos);
            *insertPos = std::move(value);
            return insertPos;
        }

        /**
         * \brief Inserts the specified number of elements at the specified position of the vector by copying value
         */
        T* Insert(size_t index, size_t count, const T& value)
        {
            CheckIndexOutOfRange(index);
            CheckForCapacity(count);

            auto insertPos = pBegin + index;
            MoveContentsForward(insertPos, count);
            for(; insertPos != pBegin + index + count; ++insertPos)
            {
                *insertPos = value;
            }
            return pBegin + index;
        }

        /**
         * \brief Inserts the elements of the initializer list at the specified position of the vector
         */
        T* Insert(size_t index, std::initializer_list<T> init)
        {
            size_t count = init.size();
            
            CheckIndexOutOfRange(index);
            CheckForCapacity(count);

            auto insertPos = pBegin + index;
            MoveContentsForward(insertPos, count);
            for(size_t i = 0; i < count; i++)
            {
                *insertPos = *(init.begin() + i);
                ++insertPos;
            }
            return pBegin + index;
        }

        /**
         * \brief Erases the element at the specified position of the vector
         */
        T* Erase(size_t index)
        {
            CheckIndexOutOfRange(index);

            MoveContentsBackward(pBegin + index);
            Destruct(pEnd - 1);
            --pEnd;
            return pBegin + index;
        }

        /**
         * \brief Erases the elements in the specified range of the vector
         */
        T* Erase(const T* first, const T* last)
        {
            CheckIndexOutOfRange(first, last);

            size_t count = last - first;
                        
            MoveContentsBackward(first, count);
            Destruct(pEnd - count, pEnd);
            pEnd -= count;

            return pBegin + (first - pBegin);
        }

        /**
         * \brief Clears the contents of the vector
         */
        void Clear() noexcept
        {
            Destroy();
            Initialize();
        }

        /**
         * \brief Swaps the contents of the vector with the contents of the other vector
         */
        void Swap(Vector<T>& other) noexcept
        {
            std::swap(pBegin, other.pBegin);
            std::swap(pEnd, other.pEnd);
            std::swap(capacity, other.capacity);
        }

        /**
         * \brief Resizes the vector to the specified size
         */
        void Resize(size_t count)
        {
            if(count == Size()) return;
            else if(count < Size())
            {
                Destruct(pBegin + count, pEnd);
                pEnd = pBegin + count;
            }
            else
            {
                CheckForCapacity(count);
                for(size_t i = Size(); i < count; ++i)
                {
                    pBegin[i] = T();
                }
                pEnd = pBegin + count;
            }
        }

        /**
         * \brief Resizes the vector to the specified size and fills the new elements with the specified value
         */
        void Resize(size_t count, const T& value)
        {
            if(count == Size()) return;
            else if(count < Size())
            {
                Destruct(pBegin + count, pEnd);
                pEnd = pBegin + count;
            }
            else
            {
                CheckForCapacity(count);
                for(size_t i = Size(); i < count; ++i)
                {
                    pBegin[i] = value; 
                }
                pEnd = pBegin + count;
            }
        }

        /**
         * \brief Increases the capacity of the vector to accommodate at least the specified number of elements.
         */
        void SetCapacity(size_t count)
        {
            T* const newBegin = new T[count];
            T* newEnd = newBegin + Size();

            for(size_t i = 0; i < Size(); i++)
            {
                newBegin[i] = pBegin[i];
            }

            Destruct(pBegin, pEnd);
            FreeArray(pBegin);

            pBegin = newBegin;
            pEnd = newEnd;
            capacity = count;
        }

        /**
         * \brief Reduces the capacity of the vector to fit its current size
         */
        void ShrinkToFit()
        {
            SetCapacity(Size());
        }

        /**
         * \brief Returns whether the vector is empty
         */
        inline bool IsEmpty() const noexcept
        {
            return Size() == 0;
        }

        /**
         * \brief Returns the number of elements in the vector
         */
        inline size_t Size() const noexcept
        {
            return pEnd - pBegin;
        }

        /**
         * \brief Returns the current capacity of the vector
         */
        inline size_t Capacity() const noexcept
        {
            return capacity;
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
         * \brief Initializes the vector members with the specified size
         */
        void Initialize(size_t count = 0)
        {
            if(count == 0)
            {
                pBegin = nullptr;
                pEnd = nullptr;
                capacity = 0;
            }
            
            pBegin = new T[count]();
            pEnd = pBegin + count;
            capacity = count;
        }

        /**
         * \brief Destroys vector contents and frees the allocated memory
         */
        void Destroy()
        {
            Destruct(pBegin, pEnd);
            FreeArray(pBegin);
            pEnd = nullptr;
            capacity = 0;
        }

        /**
         * \brief Checks if the vector has enough capacity to add a new element
         */
        void CheckForCapacity(size_t count = 1)
        {
            if(capacity == 0)
            {
                SetCapacity(1);
            }
            else if(Size() + count - 1 >= capacity)
            {
                SetCapacity(Size() * 2 + count);
            }
        }
        
        /**
         * \brief Checks if the specified index is out of range
         */
        inline void CheckIndexOutOfRange(size_t index) const
        {
            if(index < 0 || index >= Size()) throw std::out_of_range("Index out of Range");
        }

        /**
         * \brief Checks if the specified range is out of vector range
         */
        inline void CheckIndexOutOfRange(const T* first, const T* last) const
        {
            if(first > last) throw std::invalid_argument("`first` is greater than `last`");
            if(first < pBegin || first > pEnd || last > pEnd ) throw std::out_of_range("Index out of Range");
        }
        
        void MoveContentsBackward(const T* pos, size_t count = 1)
        {
            int i = pos - pBegin;
            for(T* mover = pBegin + i; mover != pEnd - count; ++mover)
            {
                *mover = *(mover + count);
            }
        }
        
        /**
         * \brief Moves the contents of the vector forward, starting from the specified position
         */
        void MoveContentsForward(const T* pos, size_t count = 1)
        {
            for(auto mover = pEnd + count - 1; mover != pos; --mover)
            {
                *mover = *(mover - count);
            }
            pEnd += count;
        }

    private:
        T* pBegin;
        T* pEnd;
        size_t capacity;
    };
}