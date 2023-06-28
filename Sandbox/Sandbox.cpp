#include <iostream>
#include <WSTL.hpp>

using namespace WSTL;

int main()
{
    Array<char, 3> array = {'a', 'b', 'c'};
        
    std::cout << array[2];

    for(const auto element : array)
    {
        std::cout << element;
    }
    
    return 0;
}
