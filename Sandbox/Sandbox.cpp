#include <string>

#include "containers/trees/BSTree.hpp"

int main()
{
    WSTL::BSTree<unsigned int, std::string> tree;

    tree.Insert(15, "A");
    tree.Insert(24, "A");
    tree.Insert(19, "A");
    tree.Insert(28, "A");
    tree.Insert(25, "A");
    tree.Insert(8, "A");
    tree.Insert(5, "A");
    tree.Insert(2, "A");
    tree.Insert(6, "A");
    tree.Insert(13, "A");
    tree.Insert(12, "A"); 

    tree.Delete(24);
    
    return 0;
}
