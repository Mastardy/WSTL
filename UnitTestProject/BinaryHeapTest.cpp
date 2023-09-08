#include <gtest/gtest.h>

#include "containers/trees/BinaryHeap.hpp"

using namespace WSTL;

TEST(BinaryHeapTest, Insert)
{
    BinaryHeap<int> heap;
    heap.Insert(1);
    heap.Insert(2);
    heap.Insert(3);

    EXPECT_EQ(heap.Size(), 3);
    EXPECT_EQ(heap.Extract(), 3);
    EXPECT_EQ(heap.Extract(), 2);
    EXPECT_EQ(heap.Extract(), 1);
}

TEST(BinaryHeapTest, Extract)
{
    BinaryHeap<int> heap;
    heap.Insert(1);
    heap.Insert(2);
    heap.Insert(3);

    EXPECT_EQ(heap.Extract(), 3);
    EXPECT_EQ(heap.Extract(), 2);
    EXPECT_EQ(heap.Extract(), 1);
}

TEST(BinaryHeapTest, Size)
{
    BinaryHeap<int> heap;
    heap.Insert(1);
    heap.Insert(2);
    heap.Insert(3);

    EXPECT_EQ(heap.Size(), 3);
    EXPECT_EQ(heap.Extract(), 3);
    EXPECT_EQ(heap.Size(), 2);
    EXPECT_EQ(heap.Extract(), 2);
    EXPECT_EQ(heap.Size(), 1);
    EXPECT_EQ(heap.Extract(), 1);
    EXPECT_EQ(heap.Size(), 0);
}

TEST(BinaryHeapTest, Copy)
{
    BinaryHeap<int> heap;
    heap.Insert(1);
    heap.Insert(2);
    heap.Insert(3);

    BinaryHeap<int> heap2(heap);

    EXPECT_EQ(heap2.Size(), 3);
    EXPECT_EQ(heap2.Extract(), 3);
    EXPECT_EQ(heap2.Extract(), 2);
    EXPECT_EQ(heap2.Extract(), 1);
    EXPECT_EQ(heap.Size(), 3);

    BinaryHeap<int> heap3 = heap;

    EXPECT_EQ(heap3.Size(), 3);
    EXPECT_EQ(heap3.Extract(), 3);
    EXPECT_EQ(heap3.Extract(), 2);
    EXPECT_EQ(heap3.Extract(), 1);
    EXPECT_EQ(heap.Size(), 3);
}

TEST(BinaryHeapTest, Move)
{
    BinaryHeap<int> heap;
    heap.Insert(1);
    heap.Insert(2);
    heap.Insert(3);

    BinaryHeap<int> heap2(std::move(heap));

    EXPECT_EQ(heap2.Size(), 3);
    EXPECT_EQ(heap.Size(), 0);

    BinaryHeap<int> heap3 = std::move(heap2);

    EXPECT_EQ(heap3.Size(), 3);
    EXPECT_EQ(heap3.Extract(), 3);
    EXPECT_EQ(heap3.Extract(), 2);
    EXPECT_EQ(heap3.Extract(), 1);
    EXPECT_EQ(heap2.Size(), 0);
}

TEST(BinaryHeapTest, Clear)
{
    BinaryHeap<int> heap;
    heap.Insert(1);
    heap.Insert(2);
    heap.Insert(3);
    
    heap.Clear();

    EXPECT_EQ(heap.Size(), 0);
}