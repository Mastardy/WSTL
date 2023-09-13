#include "memory/Memory.hpp"

int main()
{
    WSTL::SharedPointer<std::string> sp1(new std::string("Hello"));
    WSTL::SharedPointer<std::string> sp2 = sp1;

    std::cout << *sp1 << std::endl;
    std::cout << *sp2 << std::endl;
    
    sp2.Release();
    
    return 0;
}
