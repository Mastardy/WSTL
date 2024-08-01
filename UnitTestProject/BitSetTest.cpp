#include <bitset>
#include <gtest/gtest.h>
#include <WSTL/WSTL.hpp>

using namespace WSTL;

TEST(BitSetTest, Construction)
{
    BitSet<8> a;

    EXPECT_EQ(a.Size(), 8);
    EXPECT_EQ(a.Count(), 0);

    auto bstr = "10101010";
    BitSet<8> b(bstr);

    EXPECT_EQ(b.Size(), 8);
    EXPECT_EQ(b.Count(), 4);
}

TEST(BitSetTest, Set)
{
    BitSet<8> bitSet;

    for (int i = 0; i < 8; i++)
    {
        bitSet.Set(i);
    }

    EXPECT_EQ(bitSet.Count(), 8);
}

TEST(BitSetTest, Reset)
{
    BitSet<8> bitSet;

    for (int i = 0; i < 8; i++)
    {
        bitSet.Set(i);
    }

    for (int i = 0; i < 8; i++)
    {
        bitSet.Reset(i);
    }

    EXPECT_EQ(bitSet.Count(), 0);
}

TEST(BitSetTest, Flip)
{
    BitSet<8> bitSet;

    for (int i = 0; i < 8; i++)
    {
        bitSet.Flip(i);
    }

    BitSet<8> expected("10101010");

    for (int i = 0; i < 8; i++)
    {
        expected.Flip(i);
    }

    EXPECT_EQ(expected.Count(), 4);
    EXPECT_EQ(bitSet.Count(), 8);
}

TEST(BitSetTest, Count)
{
    BitSet<8> bitSet;

    for (int i = 0; i < 8; i++)
    {
        bitSet.Set(i);
        EXPECT_EQ(bitSet.Count(), i + 1);
    }
}

TEST(BitSetTest, Validate)
{
    BitSet<16> bitSet;

    for (int i = 0; i < 16; i++)
    {
        EXPECT_NO_THROW(bitSet.Validate(i));
    }

    EXPECT_THROW(bitSet.Validate(-1), std::out_of_range);
    EXPECT_THROW(bitSet.Validate(16), std::out_of_range);
}

TEST(BitSetTest, Utility)
{
    BitSet<4> a("1010");
    BitSet<4> b("0000");
    BitSet<4> c("1111");

    EXPECT_FALSE(a.All());
    EXPECT_FALSE(b.All());
    EXPECT_TRUE(c.All());

    EXPECT_TRUE(a.Any());
    EXPECT_FALSE(b.Any());
    EXPECT_TRUE(c.Any());

    EXPECT_FALSE(a.None());
    EXPECT_TRUE(b.None());
    EXPECT_FALSE(c.None());
}

TEST(BitSetTest, Operator)
{
    BitSet<8> a("10101010");
    BitSet<8> b("01010101");
    BitSet<8> c = a & b;
    BitSet<8> d = a | b;

    BitSet<8> expectedC("00000000");
    BitSet<8> expectedD("11111111");

    EXPECT_EQ(c, expectedC);
    EXPECT_EQ(d, expectedD);

    BitSet<8> e = BitSet<8>("11011011") ^ BitSet<8>("10010010");
    BitSet<8> expectedE("01001001");

    EXPECT_EQ(e, expectedE);
}

TEST(BitSetTest, Conversion)
{
    BitSet<8> a("abababab", 8, 'a', 'b');
    EXPECT_TRUE(strcmp(a.ToString().c_str(), "10101010") == 0);

    BitSet<8> b("10101010");
    EXPECT_EQ(b.ToULong(), 170);

    BitSet<64> c("1010101010101010101010101010101010101010101010101010101010101010");
    EXPECT_EQ(c.ToULLong(), 0xAAAAAAAAAAAAAAAA);
}
