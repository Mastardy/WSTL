#include <packages/Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn.1.8.1.6/build/native/include/gtest/gtest.h>
#include <packages/Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn.1.8.1.6/build/native/include/gtest/internal/gtest-internal.h>
#include <WSTL/WSTL.hpp>

using namespace WSTL;

TEST(BitSetTest, Construction)
{
    BitSet<8> bitSet;

    EXPECT_EQ(bitSet.Size(), 8);
    EXPECT_EQ(bitSet.Count(), 0);
}
