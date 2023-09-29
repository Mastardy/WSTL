#include <gtest/gtest.h>

#include "WSTL/containers/Containers.hpp"

using namespace WSTL;

TEST(DequeTest, Constructor)
{
    Deque<int> deque;
    EXPECT_EQ(deque.Size(), 0);
}

TEST(DequeTest, Copy)
{
    Deque<int> deque;

    deque.PushBack(21);
    
    Deque<int> dequeCopy(deque);
    EXPECT_EQ(deque[0], dequeCopy[0]);
    EXPECT_EQ(deque.Size(), dequeCopy.Size());

    Deque<int> dequeCopy2 = deque;
    EXPECT_EQ(deque[0], dequeCopy2[0]);
    EXPECT_EQ(dequeCopy[0], dequeCopy2[0]);
    EXPECT_EQ(deque.Size(), dequeCopy2.Size());
}

TEST(DequeTest, Move)
{
    Deque<int> deque;

    deque.PushBack(21);

    Deque<int> dequeMove(std::move(deque));
    EXPECT_EQ(dequeMove[0], 21);
    EXPECT_EQ(deque.Size(), 0);
    EXPECT_EQ(dequeMove.Size(), 1);

    Deque<int> dequeMove2 = std::move(dequeMove);
    EXPECT_EQ(dequeMove2[0], 21);
    EXPECT_EQ(dequeMove.Size(), 0);
    EXPECT_EQ(dequeMove2.Size(), 1);
}

TEST(DequeTest, Size)
{
    Deque<int> deque;

    for(int i = 0; i < 10; ++i)
    {
        deque.PushFront(i);
        EXPECT_EQ(deque.Size(), i + 1);
    }

    for(int i = 0; i < 10; ++i)
    {
        deque.PushBack(i);
        EXPECT_EQ(deque.Size(), i + 11);
    }

    for(int i = 19; i >= 0; --i)
    {
        if(i % 2 == 0)
        {
            deque.PopFront();
            EXPECT_EQ(deque.Size(), i);
        }
        else
        {
            deque.PopBack();
            EXPECT_EQ(deque.Size(), i);
        }
    }
}

TEST(DequeTest, Access)
{
    Deque<int> deque;

    for(int i = 0; i < 10; ++i)
    {
        deque.PushFront(i);
    }

    for(int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(deque[i], 9 - i);
        EXPECT_EQ(deque.At(i), 9 - i);
    }

    EXPECT_EQ(deque.Front(), 9);
    EXPECT_EQ(deque.Back(), 0);
}

TEST(DequeTest, Assign)
{
    Deque<int> deque;

    deque.Assign(25, 21);
    EXPECT_EQ(deque.Size(), 25);
    for(int i = 0; i < 25; ++i)
    {
        EXPECT_EQ(deque[i], 21);
    }
}

TEST(DequeTest, PushBack)
{
    Deque<int> deque;

    for(int i = 0; i < 10; ++i)
    {
        deque.PushBack(i);
        EXPECT_EQ(deque.Back(), i);
    }
}

TEST(DequeTest, PushFront)
{
    Deque<int> deque;

    for(int i = 0; i < 10; ++i)
    {
        deque.PushFront(i);
        EXPECT_EQ(deque.Front(), i);
    }
}

TEST(DequeTest, Insert)
{
    Deque<int> deque;

    for(int i = 0; i < 5; ++i)
    {
        deque.PushBack(i);
    }

    EXPECT_EQ(deque.Size(), 5);
    
    deque.Insert(3, 21);

    EXPECT_EQ(deque.Size(), 6);
    EXPECT_EQ(deque[0], 0);
    EXPECT_EQ(deque[1], 1);
    EXPECT_EQ(deque[2], 2);
    EXPECT_EQ(deque[3], 21);
    EXPECT_EQ(deque[4], 3);
    EXPECT_EQ(deque[5], 4);
}

TEST(DequeTest, PopBack)
{
    Deque<int> deque;

    for(int i = 0; i < 10; ++i)
    {
        deque.PushBack(i);
    }

    for(int i = 9; i >= 0; --i)
    {
        EXPECT_EQ(deque.Back(), i);
        deque.PopBack();
    }
}

TEST(DequeTest, PopFront)
{
    Deque<int> deque;

    for(int i = 0; i < 10; ++i)
    {
        deque.PushFront(i);
    }

    for(int i = 9; i >= 0; --i)
    {
        EXPECT_EQ(deque.Front(), i);
        deque.PopFront();
    }
}

TEST(DequeTest, Erase)
{
    Deque<int> deque;

    for(int i = 0; i < 10; ++i)
    {
        deque.PushFront(i);
    }

    deque.Erase(3);

    EXPECT_EQ(deque.Size(), 9);
    EXPECT_EQ(deque[0], 9);
    EXPECT_EQ(deque[1], 8);
    EXPECT_EQ(deque[2], 7);
    EXPECT_EQ(deque[3], 5);
    EXPECT_EQ(deque[4], 4);
    EXPECT_EQ(deque[5], 3);
    EXPECT_EQ(deque[6], 2);
    EXPECT_EQ(deque[7], 1);
    EXPECT_EQ(deque[8], 0);
}

TEST(DequeTest, Clear)
{
    Deque<int> deque;

    for(int i = 0; i < 10; ++i)
    {
        deque.PushFront(i);
    }

    deque.Clear();
    EXPECT_EQ(deque.Size(), 0);
}