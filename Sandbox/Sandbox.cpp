#include <iostream>

#include "WSTL/memory/Allocator.hpp"

using namespace WSTL;

struct Test
{
    int x, y;
    char c;
    double d;
    std::string str;
};

int main()
{
    auto test = Allocator::AllocateAndConstruct<Test>(sizeof(Test), 4);
    for(int i = 0; i < 4; i++)
    {
        test[i].x = i;
        test[i].y = i;
        test[i].c = 'a' + i;
        test[i].d = i;
        test[i].str = "Hello World!";
        std::cout << test[i].x << " " << test[i].y << " " << test[i].c << " " << test[i].d << " " << test[i].str << std::endl;
    }
    
    Allocator::DestructAndDeallocate(&test);
    
    return 0;
}
