#include <iostream>
#include <WSTL.h>

using namespace WSTL;

int main()
{
    {
        UniquePointer<int> ptr;
        UniquePointer<int> ptr2;

        ptr = UniquePointer<int>(new int {101});
        
        std::cout << "ptr: ";
        std::cout << (ptr ? "Not Null" : "Null") << '\n';
        std::cout << "ptr2: ";
        std::cout << (ptr2 ? "Not Null" : "Null") << '\n';

        ptr2 = std::move(ptr);
        
        std::cout << "ptr: ";
        std::cout << (ptr ? "Not Null" : "Null") << '\n';
        std::cout << "ptr2: ";
        std::cout << (ptr2 ? "Not Null" : "Null") << '\n';

        ptr2 = 3;
        
        std::cout << *ptr2;
    }
    
    return 0;
}
