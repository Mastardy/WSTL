#include <gtest/gtest.h>
#include "containers/Map.hpp"

using namespace WSTL;

typedef Map<unsigned int, char> MapType;

TEST(MapTest, Constructor)
{
    const MapType a;
    EXPECT_EQ(a.Size(), 0);
}

TEST(MapTest, Copy)
{
    MapType a;
    a.Insert(1, 'a');
    a.Insert(2, 'b');
    a.Insert(3, 'c');
    
    MapType b(a);
    EXPECT_EQ(b.Size(), 3);
    EXPECT_EQ(b[1], 'a');
    EXPECT_EQ(b[2], 'b');

    MapType c = a;
    
    EXPECT_EQ(c.Size(), 3);
    EXPECT_EQ(c[1], 'a');
    EXPECT_EQ(c[2], 'b');
}

TEST(MapTest, Move)
{
    MapType a;
    a.Insert(1, 'a');
    a.Insert(2, 'b');
    a.Insert(3, 'c');
    
    MapType b(std::move(a));
    EXPECT_EQ(b.Size(), 3);
    EXPECT_EQ(b[1], 'a');
    EXPECT_EQ(b[2], 'b');

    MapType c = std::move(b);
    
    EXPECT_EQ(c.Size(), 3);
    EXPECT_EQ(c[1], 'a');
    EXPECT_EQ(c[2], 'b');
}

TEST(MapTest, Size)
{
    MapType a;
    EXPECT_EQ(a.Size(), 0);
    a.Insert(1, 'a');
    EXPECT_EQ(a.Size(), 1);
    a.Insert(2, 'b');
    EXPECT_EQ(a.Size(), 2);
    a.Insert(3, 'c');
    EXPECT_EQ(a.Size(), 3);
    a.Erase(1);
    EXPECT_EQ(a.Size(), 2);
    a.Erase(2);
    EXPECT_EQ(a.Size(), 1);
    a.Erase(3);
    EXPECT_EQ(a.Size(), 0);
}

TEST(MapTest, Insert)
{
    MapType a;
    a.Insert(1, 'a');
    EXPECT_EQ(a.Size(), 1);
    EXPECT_EQ(a[1], 'a');
    a.Insert(2, 'b');
    EXPECT_EQ(a.Size(), 2);
    EXPECT_EQ(a[2], 'b');
    a.Insert(3, 'c');
    EXPECT_EQ(a.Size(), 3);
    EXPECT_EQ(a[3], 'c');

    a[4] = 'd';
    EXPECT_EQ(a.Size(), 4);
    EXPECT_EQ(a[4], 'd');
}