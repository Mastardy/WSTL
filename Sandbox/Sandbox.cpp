#include <WSTL.hpp>

#include <chrono>

using namespace WSTL;

int main()
{
    List<int> a(40000, 5);

    auto start = std::chrono::high_resolution_clock::now();
    
    for(size_t i = 0; i < a.Size(); i++)
    {
        a[i] = 4;
    }
    
    auto duration = duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

    std::cout << "Accessing through array: " << duration.count() << "ms\n";
    
    start = std::chrono::high_resolution_clock::now();

    for (int& i : a)
    {
        i = 3;
    }

    duration = duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

    std::cout << "Accessing through iterator: " << duration.count() << "ms\n";
    
    return 0;
}
