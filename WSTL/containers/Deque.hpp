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
        Deque()
        {
            container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
            container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
        }

        Deque(const Self& other)
        {
            container = other.container;
            frontIndex = other.frontIndex;
            backIndex = other.backIndex;
        }

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

        ~Deque()
        {
            container.Clear();
        }

        Self& operator=(const Self& other)
        {
            if(this == &other) return *this;
            container = other.container;
            frontIndex = other.frontIndex;
            backIndex = other.backIndex;
            return *this;
        }

        Self& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;

            container = std::move(other.container);
            frontIndex = other.frontIndex;
            backIndex = other.backIndex;

            other.Clear();
            
            return *this;
        }
        
        ::Size Size() const
        {
            return container.Size() * SubArraySize - (frontIndex + 1) - (SubArraySize - backIndex);
        }

        bool IsEmpty() const
        {
            if(container.Size() == 2) return frontIndex == SubArraySize - 1 && backIndex == 0;
            
            return false;
        }

        T& operator[](::Size index)
        {
            const ::Size convertedIndex = index + frontIndex + 1;
            return container[convertedIndex / SubArraySize][convertedIndex % SubArraySize];
        }

        T& At(::Size index)
        {
            return (*this)[index];
        }

        T& Front()
        {
            return (*this)[0];
        }

        T& Back()
        {
            return (*this)[Size() - 1];
        }

        void Assign(::Size amount, const T& value)
        {
            Clear();
            for(::Size i = 0; i < amount; ++i)
            {
                PushBack(value);
            }
        }
        
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

        template<class... Args>
        void EmplaceBack(Args&&... args)
        {
            if (backIndex == SubArraySize - 1)
            {
                container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
                backIndex = 0;
            }
            else
            {
                ++backIndex;
            }

            container[container.Size() - 1][backIndex] = T(std::forward<Args>(args)...);
        }

        template<class... Args>
        void EmplaceFront(Args&&... args)
        {
            if (frontIndex == 0)
            {
                container.PushFront(FixedVector<T, SubArraySize>(SubArraySize, T()));
                frontIndex = SubArraySize - 1;
            }
            else
            {
                --frontIndex;
            }

            container[0][frontIndex] = T(std::forward<Args>(args)...);
        }

        template<class... Args>
        void Emplace(::Size index, Args&&... args)
        {
            if (index == 0)
            {
                EmplaceFront(std::forward<Args>(args)...);
            }
            else if (index == Size())
            {
                EmplaceBack(std::forward<Args>(args)...);
            }
            else
            {
                const ::Size convertedIndex = index + frontIndex + 1;

                for (::Size i = Size(); i >= convertedIndex; --i)
                {
                    if (i % SubArraySize == 0)
                    {
                        container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
                    }
                    container[i / SubArraySize][i % SubArraySize] = container[(i + 1) / SubArraySize][(i + 1) % SubArraySize];
                }

                container[convertedIndex / SubArraySize][convertedIndex % SubArraySize] = T(std::forward<Args>(args)...);
            }
        }

        void Insert(::Size index, T value)
        {
            if(index == 0)
            {
                PushFront(value);
            }
            else if(index == Size())
            {
                PushBack(value);
            }
            else
            {
                const ::Size convertedIndex = index + frontIndex + 1;

                for(::Size i = Size(); i >= convertedIndex; --i)
                {
                    if(i % SubArraySize == 0)
                    {
                        container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
                    }
                    container[i / SubArraySize][i % SubArraySize] = container[(i + 1) / SubArraySize][(i + 1) % SubArraySize];
                }

                container[convertedIndex / SubArraySize][convertedIndex % SubArraySize] = value;
            }
        }
        
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

                for(::Size i = convertedIndex; i < Size(); ++i)
                {
                    container[i / SubArraySize][i % SubArraySize] = container[(i + 1) / SubArraySize][(i + 1) % SubArraySize];
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
        }

        void Clear()
        {
            container.Clear();
            container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
            container.PushBack(FixedVector<T, SubArraySize>(SubArraySize, T()));
            frontIndex = SubArraySize - 1;
            backIndex = 0;
        }

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
