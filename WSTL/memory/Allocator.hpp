#pragma once
#include <string>
#include "WSTL/Types.hpp"

constexpr unsigned SystemAllocatorMinAlignment = alignof(max_align_t);
constexpr unsigned SystemPointerSize = 8;

namespace WSTL
{
    class Allocator
    {
    public:
        template <typename T>
        static T* Allocate(Size amount, int flags = 0)
        {
            return AllocateAligned<T>(amount, SystemAllocatorMinAlignment, 0, flags);
        }

        template <typename T>
        static T* AllocateAligned(Size amount, Size alignment, Size offset = 0, int flags = 0)
        {
            Size adjustedAlignment = alignment > SystemPointerSize ? alignment : SystemPointerSize;

            void* p = new char[amount + adjustedAlignment + SystemPointerSize];
            
            auto pAddress = reinterpret_cast<uintptr_t>(p);
            auto pPlusPointerSize = pAddress + SystemPointerSize;
            auto pAlignedAddress = (pPlusPointerSize + adjustedAlignment - 1) & ~(adjustedAlignment - 1);
            
            auto pAligned = reinterpret_cast<void*>(pAlignedAddress);

            auto pStoredPointer = static_cast<void**>(pAligned) - 1;
            *pStoredPointer = p;
            
            return static_cast<T*>(pAligned);
        }

        template <typename T, class... Args>
        static T* AllocateAndConstruct(Size size, Size amount = 1, Args&&... args)
        {
            T* p = Allocate<T>(amount * size);
            Construct(p, amount, std::forward<Args>(args)...);
            return p;
        }

        template <typename T, class... Args>
        static void Construct(T* pointer, Size amount = 1, Args&&... args)
        {
            for(Size i = 0; i < amount; ++i)
            {
                new (pointer + i) T(std::forward<Args>(args)...);
            }
        }

        template <typename T>
        static void Deallocate(T** pointer, Size = 0)
        {
            if(*pointer == nullptr) return;

            void* pVoid = *pointer;
            
            void* pOriginalAllocation = *(static_cast<void**>(pVoid) - 1);
            delete[] static_cast<char*>(pOriginalAllocation);
            *pointer = nullptr;
        }
        
        template<typename T>
        static void Destruct(T* pointer, Size amount = 1)
        {
            for(Size i = 0; i < amount; ++i)
            {
                (pointer + i)->~T();
            }
        }
       
        template <typename T>
        static void DestructAndDeallocate(T** pointer, Size amount = 1)
        {
            Destruct(*pointer, amount);
            Deallocate(pointer);
        }
    };
}
