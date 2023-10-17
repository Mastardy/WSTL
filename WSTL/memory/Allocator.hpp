#pragma once
#include <string>
#include "WSTL/Types.hpp"

namespace WSTL
{
    class Allocator
    {
    public:
        explicit Allocator(std::string name = "Allocator");
        Allocator(const Allocator& other);
        Allocator(const Allocator& other, std::string name);
        Allocator(Allocator&& other) = delete;

        Allocator& operator=(const Allocator& other);
        Allocator& operator=(Allocator&& other) = delete;

        ~Allocator() = default;
        
        void* Allocate(Size amount, int flags = 0);
        void* AllocateAligned(Size amount, Size alignment, int flags = 0);
        void Deallocate(void* pointer, Size amount);

        const char* GetName() const;
        void SetName(const char* name);

    protected:
        const char* pName;
    };
}
