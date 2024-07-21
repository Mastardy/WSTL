#pragma once

#include "stdexcept"
#include "Types.hpp"
#include "utility/TypeTraits.hpp"

namespace WSTL
{
    template<Size BitAmount>
    class BitSet
    {
    public:
        typedef ConditionalT<BitAmount <= sizeof(unsigned long) * CHAR_BIT, unsigned long, unsigned long long> Type;
    };
}
