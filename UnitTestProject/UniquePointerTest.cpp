#include <gtest/gtest.h>
#include <type_traits>
#include "../WSTL/WSTL.hpp"

using namespace WSTL;

TEST(UniquePointerTest, CreateAndAccess)
{
    UniquePointer<int> ptr(new int(42));

    EXPECT_EQ(*ptr, 42);
    EXPECT_NE(ptr.Get(), nullptr);

    UniquePointer<std::string> strPtr(new std::string("Hello World!"));

    EXPECT_EQ(*strPtr, "Hello World!");
    EXPECT_NE(strPtr.Get(), nullptr);
}

TEST(UniquePointerTest, Move)
{
    UniquePointer<int> sourcePtr(new int(42));
    
    UniquePointer<int> destPtr(std::move(sourcePtr));
    EXPECT_EQ(*destPtr, 42);
    EXPECT_EQ(sourcePtr.Get(), nullptr);

    UniquePointer<int> newSourcePtr(new int(99));
    destPtr = std::move(newSourcePtr);
    EXPECT_EQ(*destPtr, 99);
    EXPECT_EQ(newSourcePtr.Get(), nullptr);
}

TEST(UniquePointerTest, ResetAndRelease) {
    UniquePointer<int> ptr(new int(42));

    ptr.Reset(new int(99));
    EXPECT_EQ(*ptr, 99);

    const int* releasedPtr = ptr.Release();
    EXPECT_EQ(*releasedPtr, 99);
    EXPECT_EQ(ptr.Get(), nullptr);
    delete releasedPtr; // Clean up the released pointer
}

TEST(UniquePointerTest, DefaultConstructor) {
    UniquePointer<int> ptr;
    EXPECT_EQ(ptr.Get(), nullptr);
}

TEST(UniquePointerTest, CopyOperations) {
    EXPECT_FALSE(std::is_copy_constructible<UniquePointer<int>>::value);
    EXPECT_FALSE(std::is_copy_assignable<UniquePointer<int>>::value);
}

TEST(UniquePointerTest, MemberAccessOperators) {
    const UniquePointer<UniquePointer<int>> ptr(new UniquePointer<int>(new int (42)));

    EXPECT_EQ(*(ptr->Get()), 42);
    EXPECT_EQ(*(*ptr), 42);
}

TEST(UniquePointerTest, ResetWithNullptr) {
    UniquePointer<int> ptr(new int(42));

    ptr.Reset();
    EXPECT_EQ(ptr.Get(), nullptr);
}

TEST(UniquePointerTest, ReleaseWithoutDeletion) {
    UniquePointer<int> ptr(new int(42));

    const int* releasedPtr = ptr.Release();
    EXPECT_EQ(*releasedPtr, 42);
    EXPECT_EQ(ptr.Get(), nullptr);
}

TEST(UniquePointerTest, ArrayType) {
    const UniquePointer<int[]> ptr(new int[5]{1, 2, 3, 4, 5});

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(ptr[i], i + 1);
    }
}

struct B
{
    virtual ~B() = default;
    virtual void Bar() { }
};

struct D : B
{
    D() { }
    ~D() override { }
    
    void Bar() override { }
};

UniquePointer<D> PassThrough(UniquePointer<D> p)
{
    p->Bar();
    return p;
}

TEST(UniquePointerTest, UniqueOwnershipSemantics)
{
    UniquePointer<D> p = MakeUnique<D>();
    UniquePointer<D> q = PassThrough(std::move(p));

    EXPECT_EQ(p.Get(), nullptr);
}

TEST(UniquePointerTest, RuntimePolymorphism)
{
    const UniquePointer<B> p = MakeUnique<D>();
    p->Bar();
}

TEST(UniquePointerTest, ArrayFromUniquePointer)
{
    const UniquePointer<D[]> p = MakeUnique<D[]>(3);
}