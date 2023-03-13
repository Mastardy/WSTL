#include <iostream>
#include <WSTL.hpp>

using namespace WSTL;

struct Shape
{
    int width;
    int height;
};

int main()
{
    {
        const UniquePointer<Shape[]> ptr(new Shape[5]);

        for(int i = 0; i < 5; i++)
        {
            std::cout << i << ": ";
            ptr[i].width = i;
            ptr[i].height = i * 3;

            std::cout << ptr[i].width * ptr[i].height << "\n";
        }
    }
    
    return 0;
}
