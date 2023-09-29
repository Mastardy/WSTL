#include <gtest/gtest.h>
#include "WSTL/containers/Map.hpp"

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

TEST(MapTest, Accessor)
{
    MapType a;
    a.Insert(1, 'a');
    a.Insert(2, 'b');
    a.Insert(3, 'c');

    EXPECT_EQ(a[1], 'a');
    EXPECT_EQ(a[2], 'b');
    EXPECT_EQ(a[3], 'c');
    EXPECT_EQ(a[4], '\0');

    std::cout << a[4] << std::endl;

    EXPECT_EQ(a.At(1), 'a');
    EXPECT_EQ(a.At(2), 'b');
    EXPECT_EQ(a.At(3), 'c');
    EXPECT_EQ(a.At(4), '\0');
}

TEST(MapTest, Insert)
{
    MapType a;
    a.Insert(1, 'a');
    a.Insert(2, 'b');
    a.Insert(3, 'c');
    a.Insert(1, 'd');

    EXPECT_EQ(a.Size(), 3);
    EXPECT_EQ(a[1], 'a');
    EXPECT_EQ(a[2], 'b');
    EXPECT_EQ(a[3], 'c');
}

TEST(MapTest, Delete)
{
    MapType a;
    a.Insert(1, 'a');
    a.Insert(2, 'b');
    a.Insert(3, 'c');
    EXPECT_EQ(a.Size(), 3);
    a.Delete(1);
    EXPECT_EQ(a.Size(), 2);

    EXPECT_EQ(a.Contains(1), false);
}

TEST(MapTest, Clear)
{
    MapType a;
    a.Insert(1, 'a');
    a.Insert(2, 'b');
    a.Insert(3, 'c');
    EXPECT_EQ(a.Size(), 3);
    a.Clear();
    EXPECT_EQ(a.Size(), 0);
}

TEST(MapTest, Contains)
{
    MapType a;
    a.Insert(1, 'a');
    a.Insert(2, 'b');
    a.Insert(3, 'c');
    EXPECT_EQ(a.Contains(1), true);
    EXPECT_EQ(a.Contains(2), true);
    EXPECT_EQ(a.Contains(3), true);
    EXPECT_EQ(a.Contains(4), false);
}

TEST(MapTest, Get)
{
    MapType a;
    a.Insert(1, 'a');
    a.Insert(2, 'b');
    a.Insert(3, 'c');
    EXPECT_EQ(a.Get(1), 'a');
    EXPECT_EQ(a.Get(2), 'b');
    EXPECT_EQ(a.Get(3), 'c');
    EXPECT_THROW(a.Get(4) == '\0', std::out_of_range);
}

TEST(MapTest, GetKeys)
{
    MapType a;
    a.Insert(1, 'a');
    a.Insert(2, 'b');
    a.Insert(3, 'c');

    auto keys = a.GetKeys();

    EXPECT_EQ(keys.Size(), 3);
    EXPECT_EQ(keys[0], 1);
    EXPECT_EQ(keys[1], 2);
    EXPECT_EQ(keys[2], 3);
}

TEST(MapTest, GetValues)
{
    MapType a;
    a.Insert(1, 'a');
    a.Insert(2, 'b');
    a.Insert(3, 'c');

    auto values = a.GetValues();

    EXPECT_EQ(values.Size(), 3);
    EXPECT_EQ(values[0], 'a');
    EXPECT_EQ(values[1], 'b');
    EXPECT_EQ(values[2], 'c');
}