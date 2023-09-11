#pragma once
#include "List.hpp"
#include "fixed/FixedVector.hpp"

namespace WSTL
{
    template<typename T, Size SubArraySize = 5>
    class Deque
    {
        typedef Deque<T, SubArraySize> Self;
    public:
        /**
         * \brief Default constructor
         */
        Deque()
        {
            container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
            container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
        }

        /**
         * \brief Copy constructor
         */
        Deque(const Self& other)
        {
            container = other.container;
            frontIndex = other.frontIndex;
            backIndex = other.backIndex;
        }

        /**
         * \brief Move constructor
         */
        Deque(Self&& other) noexcept
        {
            container = std::move(other.container);
            frontIndex = other.frontIndex;
            backIndex = other.backIndex;

            other.container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
            other.container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
            other.frontIndex = SubArraySize - 1;
            other.backIndex = 0;           
        }

        /**
         * \brief Destructor
         */
        ~Deque()
        {
            container.Clear();
        }

        /**
         * \brief Copy assignment operator
         */
        Self& operator=(const Self& other)
        {
            if(this == &other) return *this;
            container = other.container;
            frontIndex = other.frontIndex;
            backIndex = other.backIndex;
            return *this;
        }

        /**
         * \brief Move assignment operator
         */
        Self& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;

            container = std::move(other.container);
            frontIndex = other.frontIndex;
            backIndex = other.backIndex;

            other.Clear();
            
            return *this;
        }

        /**
         * \brief Returns the size of the deque
         */
        ::Size Size() const
        {
            return container.Size() * SubArraySize - (frontIndex + 1) - (SubArraySize - backIndex);
        }

        /**
         * \brief Returns if the deque is empty
         */
        bool IsEmpty() const
        {
            if(container.Size() == 2) return frontIndex == SubArraySize - 1 && backIndex == 0;
            
            return false;
        }

        /**
         * \brief Returns the element at the specified index
         */
        T& operator[](::Size index)
        {
            const ::Size convertedIndex = index + frontIndex + 1;
            return container[convertedIndex / SubArraySize][convertedIndex % SubArraySize];
        }

        /**
         * \brief Returns the element at the specified index as a const
         */
        const T& operator[](::Size index) const
        {
            const ::Size convertedIndex = index + frontIndex + 1;
            return container[convertedIndex / SubArraySize][convertedIndex % SubArraySize];
        }

        /**
         * \brief Returns the element at the specified index
         */
        T& At(::Size index)
        {
            return (*this)[index];
        }

        /**
         * \brief Returns the element at the specified index as a const
         */
        const T& At(::Size index) const
        {
            return (*this)[index];
        }
        
        /**
         * \brief Returns the element at the front of the deque
         */
        T& Front()
        {
            return (*this)[0];
        }

        /**
         * \brief Returns the element at the front of the deque as a const
         */
        const T& Front() const
        {
            return (*this)[0];
        }
        
        /**
         * \brief Returns the element at the back of the deque
         */
        T& Back()
        {
            return (*this)[Size() - 1];
        }

        /**
         * \brief Returns the element at the back of the deque as a const
         */
        const T& Back() const
        {
            return (*this)[Size() - 1];
        }
        
        
        /**
         * \brief Assigns x amount of value to deque
         */
        void Assign(::Size amount, const T& value)
        {
            Clear();
            for(::Size i = 0; i < amount; ++i)
            {
                PushBack(value);
            }
        }

        /**
         * \brief Inserts an element at the back of the deque
         */
        void PushBack(T value)
        {
            container[container.Size() - 1][backIndex] = value;

            if (backIndex == SubArraySize - 1)
            {
                container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
                backIndex = 0;
            }
            else
            {
                ++backIndex;
            }
        }

        /**
         * \brief Inserts an element at the front of the deque
         */
        void PushFront(T value)
        {
            container[0][frontIndex] = value;

            if(frontIndex == 0)
            {
                container.PushFront(FixedVector<T, SubArraySize>(SubArraySize, T()));
                frontIndex = SubArraySize - 1;
            }
            else
            {
                --frontIndex;
            }
        }

        /**
         * \brief Inserts an element at the specified index
         */
        void Insert(::Size index, T value)
        {
            const ::Size size = Size();
            
            if(index == 0)
            {
                PushFront(value);
            }
            else if(index == size)
            {
                PushBack(value);
            }
            else
            {
                const ::Size convertedIndex = index + frontIndex + 1;
                const ::Size convertedSize = size + frontIndex + 1;
                
                for(::Size i = convertedSize; i >= convertedIndex; --i)
                {
                    if(i % SubArraySize == 0 && i / SubArraySize == container.Size())
                    {
                        container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
                        backIndex = 0;
                    }
                    container[(i + 1) / SubArraySize][(i + 1) % SubArraySize] = container[i / SubArraySize][i % SubArraySize];
                }
                container[convertedIndex / SubArraySize][convertedIndex % SubArraySize] = value;
                backIndex = (backIndex + 1) % SubArraySize;
            }
        }

        /**
         * \brief Deletes the last element of the deque
         */
        void PopBack()
        {
            const ::Size size = Size();
            if(size == 0) return;
            if(size == 1)
            {
                Clear();
                return;
            }

            if(backIndex == 0)
            {
                container.PopBack();
                backIndex = SubArraySize - 1;
            }
            else
            {
                --backIndex;
            }
        }

        /**
         * \brief Deletes the first element of the deque
         */
        void PopFront()
        {
            const ::Size size = Size();
            if(size == 0) return;
            if(size == 1)
            {
                Clear();
                return;
            }

            if(frontIndex == SubArraySize - 1)
            {
                container.PopFront();
                frontIndex = 0;
            }
            else
            {
                ++frontIndex;
            }
        }

        /**
         * \brief Deletes the element at the specified index
         */
        void Erase(::Size index)
        {
            const ::Size size = Size();
            if(size == 0) return;
            if(size == 1)
            {
                Clear();
                return;
            }

            if(index == 0)
            {
                PopFront();
            }
            else if(index == Size() - 1)
            {
                PopBack();
            }
            else
            {
                const ::Size convertedIndex = index + frontIndex + 1;
                const ::Size convertedSize = size + frontIndex + 1;

                for(::Size i = convertedIndex; i < convertedSize; ++i)
                {
                    container[i / SubArraySize][i % SubArraySize] = container[(i + 1) / SubArraySize][(i + 1) % SubArraySize];
                }

                --backIndex;

                if(backIndex == SubArraySize - 1) container.PopBack();
            }
        }

        /**
         * \brief Deletes all elements of the deque
         */
        void Clear()
        {
            container.Clear();
            container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
            container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
            frontIndex = SubArraySize - 1;
            backIndex = 0;
        }

        /**
         * \brief Swaps the contents of two deques
         */
        void Swap(Self& other)
        {
            container.Swap(other.container);
            std::swap(frontIndex, other.frontIndex);
            std::swap(backIndex, other.backIndex);
        }
        
    private:
        List<FixedVector<T, SubArraySize>> container;
        ::Size frontIndex = SubArraySize - 1;
        ::Size backIndex = 0;
    };
}
