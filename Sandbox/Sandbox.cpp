#include <WSTL.hpp>

using namespace WSTL;

int main()
{
    List<int> a(10, 5);

    for(size_t i = 0; i < a.Size(); i++)
    {
        std::cout << a[i] << std::endl;
    }
    
    return 0;
}
