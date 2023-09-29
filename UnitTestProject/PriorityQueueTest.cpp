#include <gtest/gtest.h>

#include "WSTL/containers/PriorityQueue.hpp"

using namespace WSTL;

TEST(PriorityQueueTest, Push)
{
    PriorityQueue<int> queue;
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);

    EXPECT_EQ(queue.Size(), 3);
    EXPECT_EQ(queue.Pop(), 3);
    EXPECT_EQ(queue.Pop(), 2);
    EXPECT_EQ(queue.Pop(), 1);
}

TEST(PriorityQueueTest, Pop)
{
    PriorityQueue<int> queue;
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);

    EXPECT_EQ(queue.Pop(), 3);
    EXPECT_EQ(queue.Pop(), 2);
    EXPECT_EQ(queue.Pop(), 1);
}

TEST(PriorityQueueTest, Size)
{
    PriorityQueue<int> queue;
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);

    EXPECT_EQ(queue.Size(), 3);
    EXPECT_EQ(queue.Pop(), 3);
    EXPECT_EQ(queue.Size(), 2);
    EXPECT_EQ(queue.Pop(), 2);
    EXPECT_EQ(queue.Size(), 1);
    EXPECT_EQ(queue.Pop(), 1);
    EXPECT_EQ(queue.Size(), 0);
}

TEST(PriorityQueueTest, Copy)
{
    PriorityQueue<int> queue;
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);

    PriorityQueue<int> queue2(queue);

    EXPECT_EQ(queue2.Size(), 3);
    EXPECT_EQ(queue2.Pop(), 3);
    EXPECT_EQ(queue2.Pop(), 2);
    EXPECT_EQ(queue2.Pop(), 1);
}

TEST(PriorityQueueTest, Move)
{
    PriorityQueue<int> queue;
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);

    PriorityQueue<int> queue2(std::move(queue));

    EXPECT_EQ(queue2.Size(), 3);
    EXPECT_EQ(queue2.Pop(), 3);
    EXPECT_EQ(queue2.Pop(), 2);
    EXPECT_EQ(queue2.Pop(), 1);
}

TEST(PriorityQueueTest, Assignment)
{
    PriorityQueue<int> queue;
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);

    PriorityQueue<int> queue2 = queue;

    EXPECT_EQ(queue2.Size(), 3);
    EXPECT_EQ(queue2.Pop(), 3);
    EXPECT_EQ(queue2.Pop(), 2);
    EXPECT_EQ(queue2.Pop(), 1);
}

TEST(PriorityQueueTest, AssignmentMove)
{
    PriorityQueue<int> queue;
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);

    PriorityQueue<int> queue2 = std::move(queue);

    EXPECT_EQ(queue2.Size(), 3);
    EXPECT_EQ(queue2.Pop(), 3);
    EXPECT_EQ(queue2.Pop(), 2);
    EXPECT_EQ(queue2.Pop(), 1);
}

TEST(PriorityQueueTest, InitializerList)
{
    PriorityQueue<int> queue = { 1, 2, 3 };

    EXPECT_EQ(queue.Size(), 3);
    EXPECT_EQ(queue.Pop(), 3);
    EXPECT_EQ(queue.Pop(), 2);
    EXPECT_EQ(queue.Pop(), 1);
}

TEST(PriorityQueueTest, Emplace)
{
    PriorityQueue<std::string> queue;
    queue.Emplace("Hello");
    queue.Emplace("World");
    queue.Emplace("!");

    EXPECT_EQ(queue.Size(), 3);
    EXPECT_EQ(queue.Pop(), "World");
    EXPECT_EQ(queue.Pop(), "Hello");
    EXPECT_EQ(queue.Pop(), "!");
}

TEST(PriorityQueueTest, Clear)
{
    PriorityQueue<int> queue;
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);

    queue.Clear();

    EXPECT_EQ(queue.Size(), 0);
}