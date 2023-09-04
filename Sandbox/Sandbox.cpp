#include <WSTL.hpp>
#include <chrono>
#include <iostream>

#include "containers/BTree.hpp"

using namespace WSTL;

int main()
{
    BTree<int> tree;

    for(int i = 0; i < 100; ++i)
    {
        tree.Insert(i); 
    }

    std::cout << tree.Size() << std::endl;
    tree.Delete(42);
    std::cout << tree.Size() << std::endl;
    std::cout << tree.Size() << std::endl;

    
    return 0;
}