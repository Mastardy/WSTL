#include <gtest/gtest.h>
#include "../WSTL/WSTL.hpp"

using namespace WSTL;

TEST(ArrayTest, ConstructionAndElementAccess)
{
    Array<int, 3> array = {1, 2, 3};

    EXPECT_EQ(array[0], 1);
    EXPECT_EQ(array[1], 2);
    EXPECT_EQ(array[2], 3);
}

TEST(ArrayTest, Size)
{
    constexpr Array<int, 3> array = {1, 2, 3};

    EXPECT_EQ(array.Size(), 3);
}

TEST(ArrayTest, FrontAndBack)
{
    Array<int, 3> array = {1, 2, 3};

    EXPECT_EQ(array.Front(), 1);
    EXPECT_EQ(array.Back(), 3);
}

TEST(ArrayTest, Iterators)
{
    Array<int, 3> array = {1, 2, 3};

    EXPECT_EQ(*array.begin(), 1);
    EXPECT_EQ(*array.end(), 3);
}

TEST(ArrayTest, ConstIterators)
{
    constexpr Array<int, 3> array = {1, 2, 3};

    EXPECT_EQ(*array.begin(), 1);
    EXPECT_EQ(*array.end(), 3);
}

TEST(ArrayTest, Data)
{
    Array<int, 3> array = {1, 2, 3};

    EXPECT_EQ(*array.Data(), 1);
}

TEST(ArrayTest, Fill)
{
    Array<int, 3> array = {1, 2, 3};

    array.Fill(4);

    EXPECT_EQ(array[0], 4);
    EXPECT_EQ(array[1], 4);
    EXPECT_EQ(array[2], 4);
}

TEST(ArrayTest, Swap)
{
    Array<int, 3> array1 = {1, 2, 3};
    Array<int, 3> array2 = {4, 5, 6};

    array1.Swap(array2);

    EXPECT_EQ(array1[0], 4);
    EXPECT_EQ(array1[1], 5);
    EXPECT_EQ(array1[2], 6);

    EXPECT_EQ(array2[0], 1);
    EXPECT_EQ(array2[1], 2);
    EXPECT_EQ(array2[2], 3);
}

TEST(ArrayTest, Comparison)
{
    constexpr Array<int, 3> array1 = {1, 2, 3};
    constexpr Array<int, 3> array2 = {1, 2, 3};
    constexpr Array<int, 3> array3 = {4, 5, 6};

    EXPECT_TRUE(array1 == array2);
    EXPECT_FALSE(array1 == array3);
    EXPECT_FALSE(array2 == array3);

    EXPECT_FALSE(array1 != array2);
    EXPECT_TRUE(array1 != array3);
    EXPECT_TRUE(array2 != array3);
}

TEST(ArrayTest, Copy)
{
    Array<int, 3> array1 = {1, 2, 3};
    Array<int, 3> array2 = array1;

    EXPECT_EQ(array1[0], 1);
    EXPECT_EQ(array1[1], 2);
    EXPECT_EQ(array1[2], 3);

    EXPECT_EQ(array2[0], 1);
    EXPECT_EQ(array2[1], 2);
    EXPECT_EQ(array2[2], 3);
}


TEST(ArrayTest, Assignment)
{
    Array<int, 3> array2 = {4, 5, 6};

    Array<int, 3> array1 = array2;

    EXPECT_EQ(array1[0], 4);
    EXPECT_EQ(array1[1], 5);
    EXPECT_EQ(array1[2], 6);

    EXPECT_EQ(array2[0], 4);
    EXPECT_EQ(array2[1], 5);
    EXPECT_EQ(array2[2], 6);
}

TEST(ArrayTest, At)
{
    Array<int, 3> array = {1, 2, 3};

    EXPECT_EQ(array.At(0), 1);
    EXPECT_EQ(array.At(1), 2);
    EXPECT_EQ(array.At(2), 3);

    EXPECT_THROW(array.At(3), std::out_of_range);
}

TEST(ArrayTest, ConstAt)
{
    constexpr Array<int, 3> array = {1, 2, 3};

    EXPECT_EQ(array.At(0), 1);
    EXPECT_EQ(array.At(1), 2);
    EXPECT_EQ(array.At(2), 3);

    EXPECT_THROW(array.At(3), std::out_of_range);
}

TEST(ArrayTest, OperatorSquareBrackets)
{
    Array<int, 3> array = {1, 2, 3};

    EXPECT_EQ(array[0], 1 );
    EXPECT_EQ(array[1], 2);
    EXPECT_EQ(array[2], 3);
}