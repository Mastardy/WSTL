#include <iostream>
#include <string>

#include "containers/Map.hpp"

int main()
{
    using namespace WSTL;

    Map<int, char> a;
    a.Insert(1, 'a');
    a.Insert(2, 'b');
    a.Insert(3, 'c');

    Map<int, char> b(a);    
    Map<int, char> c = a;
    
    return 0;
}
