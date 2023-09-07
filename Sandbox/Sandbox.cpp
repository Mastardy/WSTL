#include <iostream>

#include "containers/Set.hpp"

int main()
{
    WSTL::Set<int> set;

    set.Insert(6);
    set.Insert(3);
    set.Insert(8);
    set.Insert(1);
    set.Insert(5);
    set.Insert(7);
    set.Insert(9);
    set.Insert(2);
    set.Insert(4);
    
    auto vec = set.ToVector();
    
    std::cout << "Set: ";
    
    for(Size i = 0; i < vec.Size(); ++i)
    {
        std::cout << vec[i] << " ";
    }
    
    return 0;
}
