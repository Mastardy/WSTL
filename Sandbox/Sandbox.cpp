#include <WSTL/utility/Hash.hpp>

#include "WSTL/WSTL.hpp"

using namespace WSTL;

#include <print>

int main()
{    
    auto a = "Hello, World!";
    auto ah = Hash(a, strlen(a));
    std::println("(Hex) A: {:#010x}", ah);
    std::println("(Bin) A: {:#034b}", ah);

    auto b = 512;
    auto bh = Hash(&b, sizeof(b));
    std::println("(Hex) B: {:#010x}", bh);
    std::println("(Bin) B: {:#034b}", bh);

    auto c = 3.14159f;
    auto ch = Hash(&c, sizeof(c));
    std::println("(Hex) C: {:#010x}", ch);
    std::println("(Bin) C: {:#034b}", ch);

    auto d = 2.71828;
    auto dh = Hash(&d, sizeof(d));
    std::println("(Hex) D: {:#010x}", dh);
    std::println("(Bin) D: {:#034b}", dh);

    auto e = true;
    auto eh = Hash(&e, sizeof(e));
    std::println("(Hex) E: {:#010x}", eh);
    std::println("(Bin) E: {:#034b}", eh);

    auto f = 'A';
    auto fh = Hash(&f, sizeof(f));
    std::println("(Hex) F: {:#010x}", fh);
    std::println("(Bin) F: {:#034b}", fh);
    
    struct G
    {
        int a;
        float b;
        double c;
        bool d;
        char e;
    };
    auto g = G{ 1, 2.0f, 3.0, true, 'A' };
    auto gh = Hash(&g, sizeof(g));
    std::println("(Hex) G: {:#010x}", gh);
    std::println("(Bin) G: {:#034b}", gh);
    
    return 0;
}
