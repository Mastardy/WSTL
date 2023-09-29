#include <gtest/gtest.h>
#include "WSTL/containers/trees/RBTree.hpp"

using namespace WSTL;

typedef RBTree<unsigned int, char> RedBlackTreeType;
typedef Pair<unsigned int, char> PairType;

TEST(RedBlackTreeTest, DefaultConstructor)
{
    const RedBlackTreeType a;
    EXPECT_EQ(a.Size(), 0);
}

TEST(RedBlackTreeTest, Copy)
{
    RedBlackTreeType a;
    a.Insert(5, 'a');
    a.Insert(10, 'b');

    const RedBlackTreeType b(a);
    EXPECT_EQ(b.Size(), 2);

    const RedBlackTreeType c = a;
    EXPECT_EQ(c.Size(), 2);
}

TEST(RedBlackTreeTest, Move)
{
    RedBlackTreeType a;
    a.Insert(5, 'a');
    a.Insert(10, 'b');

    RedBlackTreeType b(std::move(a));
    EXPECT_EQ(b.Size(), 2);
    EXPECT_EQ(a.Size(), 0);

    const RedBlackTreeType c = std::move(b);
    EXPECT_EQ(c.Size(), 2);
    EXPECT_EQ(b.Size(), 0);
}

TEST(RedBlackTreeTest, Insert)
{
    RedBlackTreeType a;
    a.Insert(5, 'a');
    a.Insert(10, 'b');
    EXPECT_EQ(a.Size(), 2);
    a.Insert(4, 'c');
    EXPECT_EQ(a.Size(), 3);
}

TEST(RedBlackTreeTest, Get)
{
    RedBlackTreeType a;
    a.Insert(5, 'a');
    a.Insert(10, 'b');
    a.Insert(4, 'c');

    EXPECT_EQ(a.Get(5), 'a');
    EXPECT_EQ(a.Get(10), 'b');
    EXPECT_EQ(a.Get(4), 'c');
    EXPECT_EQ(a.Get(1), char());

    EXPECT_EQ(a.GetPair(5), PairType(5, 'a'));
    EXPECT_EQ(a.GetPair(10), PairType(10, 'b'));
    EXPECT_EQ(a.GetPair(4), PairType(4, 'c'));
    EXPECT_EQ(a.GetPair(1), PairType(1, char()));

    EXPECT_EQ(a[5], 'a');
    EXPECT_EQ(a[10], 'b');
    EXPECT_EQ(a[4], 'c');
    EXPECT_EQ(a[1], char());
}

TEST(RedBlackTreeTest, Deletion)
{
    RedBlackTreeType a;
    a.Insert(5, 'a');
    a.Insert(3, 'b');
    a.Insert(7, 'c');

    a.Delete(3);
    EXPECT_EQ(a.Size(), 2);
    EXPECT_EQ(a.Get(3), char());
    EXPECT_EQ(a.Get(5), 'a');
    EXPECT_EQ(a.Get(7), 'c');

    a.Delete(3);
    EXPECT_EQ(a.Size(), 2);
    EXPECT_EQ(a.Get(5), 'a');
    EXPECT_EQ(a.Get(7), 'c');
}

TEST(RedBlackTreeTest, Size)
{
    RedBlackTreeType a;
    EXPECT_EQ(a.Size(), 0);

    a.Insert(1, 'a');
    EXPECT_EQ(a.Size(), 1);

    a.Delete(1);
    EXPECT_EQ(a.Size(), 0);
}

TEST(RedBlackTreeTest, Clear)
{
    RedBlackTreeType a;
    a.Insert(1, 'a');
    a.Insert(2, 'b');
    a.Insert(3, 'c');

    a.Clear();
    EXPECT_EQ(a.Size(), 0);
}