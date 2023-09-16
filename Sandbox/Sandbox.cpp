#include "memory/Memory.hpp"

int main()
{
    WSTL::SharedPointer<int> sp1(new int(5));
    WSTL::WeakPointer<int> wp1(sp1);

    if(!wp1.Expired())
    {
        auto temp = wp1.Lock();

        *temp = 10;
    }

    sp1.Reset();

    if(!wp1.Expired())
    {
        auto temp = wp1.Lock();

        *temp = 1;
    }
    
    return 0;
}
