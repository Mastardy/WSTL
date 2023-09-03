#include <iostream>
#include <WSTL.hpp>

#include "containers/BTree.hpp"

using namespace WSTL;

int main()
{
    BTree<int> tree;
    constexpr int max = 1000;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < max; ++i)
    {
        tree.Insert(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Insert took " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    tree.Search(max - 1);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Search took " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    tree.InternalSearch(tree.GetRoot(), max - 1);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Internal Search took " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns" << std::endl;
    
    return 0;
}