#include <iostream>

#include "WSTL/WSTL.hpp"

using namespace WSTL;

struct TestStruct
{
    int a;
    std::string b;

    bool operator==(const TestStruct& other) const
    {
        return a == other.a && b == other.b;
    }

    ::Size Hash() const
    {
        var hashA = WSTL::Hash(&a, sizeof(int));
        var hashB = WSTL::Hash(b.data(), static_cast<::Size>(b.size()));
        return hashA ^ (hashB + 0x9e3779b9 + (hashA << 6) + (hashA >> 2));
    }
};

int main()
{    
    var map = HashMap<TestStruct, string>();

    std::cout << HasCustomHash<TestStruct>::value << '\n';
    
    var a = TestStruct{1, "key1"};
    var b = TestStruct{1, "key1"};
    
    map.Insert(a, "value1");
    std::cout << map.Get(b) << '\n';
    
    map.Get(a) = "value2";
    std::cout << map.Get(b) << '\n';
    
    return 0;
}
