#include <iostream>

#include "utility/Optional.hpp"

using namespace WSTL;

int main()
{
    Optional<float> opt1;

    std::cout << "opt1.HasValue(): " << opt1.HasValue() << std::endl;

    Optional<float> opt2(5.0f);

    std::cout << "opt2.HasValue(): " << opt2.HasValue() << std::endl;

    std::cout << "opt2.Value(): " << opt2.Value() << std::endl;

    Optional<float> opt3(opt2);

    std::cout << "opt3.HasValue(): " << opt3.HasValue() << std::endl;
    
    return 0;
}
