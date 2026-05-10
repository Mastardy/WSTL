#include <iostream>

#include "WSTL/WSTL.hpp"

using namespace WSTL;

struct PlayerId {
    UI32   region;
    string tag;

    bool operator==(const PlayerId& o) const {
        return region == o.region && tag == o.tag;
    }

    Size Hash() const {
        Size h1 = WSTL::Hash(&region, sizeof(UI32));
        Size h2 = WSTL::Hash(tag.data(), static_cast<Size>(tag.size()));
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

int main() {
    static_assert(HasCustomHash<PlayerId>::value);

    HashMap<PlayerId, SharedPointer<Vector<string>>> world;
    PlayerId hero{ 1, "ace" };

    world.Insert(hero, MakeShared<Vector<string>>());
    world.Get(hero)->PushBack("torch");

    std::cout << world.Get(hero)->Size() << " item(s)\n";
}