#include <iostream>
#include <string>

#include "containers/Map.hpp"

int main()
{
    WSTL::Map<unsigned int, std::string> results;

    results[1] = "Teste";
    results[2] = "Teste2";
    results[3] = "Teste3";
    results[4] = "Teste4";
    results[5] = "Teste5";
    results[6] = "Teste6";
    results[7] = "Teste7";
    results[8] = "Teste8";
    results[9] = "Teste9";
    
    std::cout << results.Size() << std::endl;
    auto keys = results.GetKeys();
    for(const auto& key : keys)
    {
        std::cout << key << std::endl;
    }

    auto values = results.GetValues();
    for(const auto& value : values)
    {
        std::cout << value << std::endl;
    }
    
    return 0;
}
