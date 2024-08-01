#pragma once
#include "WSTL/Types.hpp"
#include "WSTL/memory/Allocator.hpp"

namespace WSTL
{
    static UI32 Scramble(UI32 key)
    {
        key *= 0xcc9e2d51;
        key = (key << 15) | (key >> 17);
        key *= 0x1b873593;
        return key;
    }

    inline UI32 Hash(const void* data, UI32 length)
    {
        auto key = static_cast<const UI8*>(data);
        UI32 seed = 0;
        UI32 k = 0;

        for(Size i = length >> 2; i; i--)
        {
            memcpy_s(&k, sizeof(UI32), key, sizeof(UI32));
            key += sizeof(UI32);
            seed ^= Scramble(k);
            seed = (seed << 13) | (seed >> 19);
            seed = seed * 5 + 0xe6546b64;
        }

        k = 0;
        for(Size i = length & 3; i; i--)
        {
            k <<= 8;
            k |= *key++;
        }

        seed ^= Scramble(k);
        seed ^= length;
        seed ^= seed >> 16;
        seed *= 0x85ebca6b;
        seed ^= seed >> 13;
        seed *= 0xc2b2ae35;
        seed ^= seed >> 16;
        return seed;
    }
}
