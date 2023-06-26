#include <cassert>
#include <WSTL.hpp>

using namespace WSTL;

struct B
{
    virtual ~B() = default;
    virtual void Bar() { std::cout << "B::Bar" << std::endl; }
};

struct D : B
{
    D() { std::cout << "D::D" << std::endl; }
    ~D() override { std::cout << "D::~D" << std::endl; }
    
    void Bar() override { std::cout << "D::Bar" << std::endl; }
};

UniquePointer<D> PassThrough(UniquePointer<D> p)
{
    p->Bar();
    return p;
}

std::unique_ptr<D> pass_through(std::unique_ptr<D> p)
{
    p->Bar();
    return p;
}

void CloseFile(std::FILE* fp)
{
    std::fclose(fp);
}

struct List
{
    struct Node
    {
        int data;
        UniquePointer<Node> next;
    };

    UniquePointer<Node> head;

    ~List()
    {
        while(head)
        {
            auto next = std::move(head->next);
            head = std::move(next);
        }
    }

    void Push(int data)
    {
        head = UniquePointer<Node>(new Node {data, std::move(head)});
    }
};

int main()
{
    std::cout << "1. Unique Ownership Semantics Demo\n";
    {
        auto p = MakeUnique<D>();
        UniquePointer<D> q = PassThrough(std::move(p));

        assert(!p);
    }

    std::cout << "2. Runtime Polymorphism Demo\n";
    {
        UniquePointer<B> p = MakeUnique<D>();
        p->Bar();
    }

    std::cout << "3. Array from Unique Pointer Demo\n";
    {
        UniquePointer<D[]> p = MakeUnique<D[]>(3);
    }

    std::cout << "4. Linked List Demo\n";
    {
        List list;
        for(int i = 0; i != 1'000'00; i++)
        {
            list.Push(i);
        }
        std::cout << "Added 1'000'000 integers to the list\n";
    }
    
    return 0;
}
