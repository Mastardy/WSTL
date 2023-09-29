#pragma once
#include <stdexcept>

#include "WSTL/Types.hpp"

namespace WSTL
{
    template <typename T>
    struct SListNode
    {
    private:
        typedef SListNode<T> Self;
        
    public:
        T value;
        SListNode<T>* pNext;

        SListNode(T value) : value(value), pNext(nullptr)
        {
        }

        /** 
         *  \brief Inserts a new node after this node.
         */
        void Insert(Self* pNode)
        {
            if(this->pNext != nullptr)
            {
                pNode->pNext = this->pNext;
            }
            this->pNext = pNode;
        }

        /**
         * \brief Removes the node after this node.
         */
        void Remove()
        {
            if(pNext != nullptr)
            {
                const Self* pTemp = pNext;
                pNext = pNext->pNext;
                delete pTemp;
            }
            else 
            {
                throw std::runtime_error("Cannot remove node after nullptr.");
            }
        }

        /**
         * \brief Swaps the values of two nodes.
         */
        static void Swap(Self& lhs, Self& rhs)
        {
            Self temp = lhs;
            lhs = rhs;
            rhs = temp;
        }

        /**
         * \brief Insert a range of nodes after this node.
         */
        void InsertRange(Self* pFirst, Self* pLast)
        {
            if(pFirst == nullptr || pLast == nullptr) return;
            
            pLast->pNext = this->pNext;
            this->pNext = pFirst;
        }
    };

    template<typename T, typename Pointer = T*, typename Reference = T&>
    struct SListIterator
    {
    private:
        typedef SListNode<T> Node;
        typedef SListIterator<T, Pointer, Reference> Self;
        
    public:
        Node* pNode;

        SListIterator() noexcept : pNode(nullptr) {}
        SListIterator(const Node* pNode) noexcept : pNode(const_cast<Node*>(pNode)) {}
        SListIterator(const Self& other) noexcept : pNode(other.pNode) {}

        Self& operator=(const Self& other) noexcept
        {
            if(this != &other) pNode = other.pNode;
            return *this;
        }

        SListIterator(const Self&& other) = delete;
        Self& operator=(const Self&& other) = delete;
        ~SListIterator() = default;

        Self next() const noexcept
        {
            return pNode->pNext;
        }

        Reference operator*() const noexcept
        {
            return pNode->value;
        }

        Pointer operator->() const noexcept
        {
            return &pNode->value;
        }

        Self& operator++() noexcept
        {
            pNode = pNode->pNext;
            return *this;
        }

        Self operator++(int) noexcept
        {
            Self temp = *this;
            pNode = pNode->pNext;
            return temp;
        }

        Self& operator--() noexcept = delete;
        Self operator--(int) noexcept = delete;

        bool operator==(const Self& other) const noexcept
        {
            return pNode == other.pNode;
        }

        bool operator!=(const Self& other) const noexcept
        {
            return pNode != other.pNode;
        }
    };

    template<typename T>
    class SList
    {
        typedef SListNode<T> Node;
        typedef SListIterator<T> Iterator;
        typedef SListIterator<T, const T*, const T&> ConstIterator;
        typedef SList<T> Self;

    public:
        /**
         * \brief Default constructor
         */
        SList() : size(0), pHead(nullptr), pTail(nullptr) {}

        /**
         * \brief Constructs a list with a given amount of elements
         */
        SList(Size amount, const T& value) : size(0), pHead(nullptr), pTail(nullptr)
        {
            for(::Size i = 0; i < amount; ++i)
            {
                PushFront(value);
            }
        }

        /**
         * \brief Copy constructor
         */
        SList(const Self& other) : size(0), pHead(nullptr), pTail(nullptr)
        {
            for(auto element : other)
            {
                PushBack(element);
            }
        }

        /**
         * \brief Move constructor
         */
        SList(Self&& other) noexcept : size(other.size), pHead(other.pHead), pTail(other.pTail)
        {
            other.size = 0;
            other.pHead = nullptr;
            other.pTail = nullptr;
        }

        /**
         * \brief Copy assignment operator
         */
        Self& operator=(const Self& other)
        {
            if(this == &other) return *this;

            Clear();

            for(auto element : other)
            {
                PushBack(element);
            }

            return *this;
        }

        /**
         * \brief Move assignment operator
         */
        Self& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;

            Clear();

            size = other.size;
            pHead = other.pHead;
            pTail = other.pTail;

            other.size = 0;
            other.pHead = nullptr;
            other.pTail = nullptr;

            return *this;
        }

        /**
         * \brief Destructor
         */
        ~SList()
        {
            Clear();
        }

        /**
         * \brief Returns the value at the given index
         */
        T& operator[](Size index)
        {
            return NodeAt(index)->value;
        }

        /**
         * \brief Returns the value at the given index as a const
         */
        const T& operator[](Size index) const
        {
            return NodeAt(index)->value;
        }

        /**
         * \brief Inserts a value at the given index
         */
        void Assign(Size index, const T& value)
        {
            if(index > size || index < 0) throw std::out_of_range("Index out of range.");
            else if(index == size) PushBack(value);
            else if(index == 0) PushFront(value);
            else NodeAt(index)->Insert(new Node(value));
        }

        /**
         * \brief Inserts a value at the end of the list
         */
        void PushBack(const T& value)
        {
            auto pNewNode = new Node(value);

            if(IsEmpty())
            {
                pHead = pNewNode;
                pTail = pNewNode;
            }
            else
            {
                pTail = pTail->pNext = pNewNode;
            }

            ++size;
        }

        /**
         * \brief Inserts a value at the front of the list
         */
        void PushFront(const T& value)
        {
            auto pNewNode = new Node(value);
            
            if(IsEmpty())
            {
                pHead = pNewNode;
                pTail = pNewNode;
            }
            else
            {
                pNewNode->pNext = pHead;
                pHead = pNewNode;
            }

            ++size;
        }

        /**
         * \brief Deletes the last element of the list
         */
        void PopBack()
        {
            if(IsEmpty()) return;

            if(size == 1)
            {
                delete pHead;
                pHead = nullptr;
                pTail = nullptr;
                size = 0;
                return;
            }

            Node* pTemp = pHead;
            while(pTemp->pNext != pTail)
            {
                pTemp = pTemp->pNext;
            }
            
            delete pTemp->pNext;
            pTail = pTemp;
            pTail->pNext = nullptr;
            --size;
        }

        /**
         * \brief Deletes the first element of the list
         */
        void PopFront()
        {
            if(IsEmpty()) return;

            if(size == 1)
            {
                delete pHead;
                pHead = nullptr;
                pTail = nullptr;
                size = 0;
                return;
            }

            const Node* pTemp = pHead;
            pHead = pHead->pNext;
            delete pTemp;
            --size;
        }

        /**
         * \brief Clears the list by deleting all elements
         */
        void Clear()
        {
            if(IsEmpty()) return;

            Node* pCurrent = pHead;

            while(pCurrent != nullptr)
            {
                Node* pTemp = pCurrent->pNext;
                delete pCurrent;
                pCurrent = pTemp;
            }

            pTail = nullptr;
            pHead = nullptr;
            size = 0;
        }

        /**
         * \brief Removes the element at the given index
         */
        void Remove(Size index)
        {
            if(index >= size || index < 0) throw std::out_of_range("Index out of range");
            if(index == 0) PopFront();
            else if(index == size - 1) PopBack();
            else
            {
                NodeAt(index - 1)->Remove();
                size--;
            }
        }

        /**
         * \brief Returns the value at the front
         */
        T& Front()
        {
            if(IsEmpty()) throw std::out_of_range("List is empty.");
            return pHead->value;
        }

        /**
         * \brief Returns the value at the front as a const
         */
        const T& Front() const
        {
            if(IsEmpty()) throw std::out_of_range("List is empty.");
            return pHead->value;
        }

        /**
         * \brief Returns the value at the back
         */
        T& Back()
        {
            if(IsEmpty()) throw std::out_of_range("List is empty.");
            return pTail->value;
        }

        /**
         * \brief Returns the value at the back as a const
         */
        const T& Back() const
        {
            if(IsEmpty()) throw std::out_of_range("List is empty.");
            return pTail->value;
        }

        /**
         * \brief Returns whether the list is empty or not
         */
        bool IsEmpty() const noexcept
        {
            return size == 0;
        }

        /**
         * \brief Returns the size of the list
         */
        Size Size() const noexcept
        {
            return size;
        }

        Iterator begin() noexcept
        {
            return Iterator(pHead);
        }

        ConstIterator begin() const noexcept
        {
            return ConstIterator(pHead);
        }

        Iterator end() noexcept
        {
            return Iterator(nullptr);
        }

        ConstIterator end() const noexcept
        {
            return ConstIterator(nullptr);
        }

    protected:
        /**
         * \brief Returns the node at the given index
         */
        Node* NodeAt(::Size index)
        {
            if(index >= size || index < 0) throw std::out_of_range("Index out of range.");

            auto pCurrent = pHead;
            for(::Size i = 0; i < index; ++i)
            {
                pCurrent = pCurrent->pNext;
            }

            return pCurrent;
        }

        /**
         * \brief Returns the node at the given index as a const
         */
        const Node* NodeAt(::Size index) const
        {
            if(index >= size || index < 0) throw std::out_of_range("Index out of range.");

            auto pCurrent = pHead;
            for(::Size i = 0; i < index; ++i)
            {
                pCurrent = pCurrent->pNext;
            }

            return pCurrent;
        }
        
    private:
        ::Size size;
        Node* pHead;
        Node* pTail;
    };
}
