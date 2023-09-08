#pragma once

#include <exception>

#include "Types.hpp"
#include "Utility.hpp"

namespace WSTL
{
    template<typename T>
    struct BinaryHeapNode
    {
    private:
        typedef BinaryHeapNode<T> Self;
    public:
        T data;
        Self *pLeft;
        Self *pRight;
        Self *pParent;

        BinaryHeapNode(const T &data) : data(data), pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}

        BinaryHeapNode(const Self& other) : data(other.data), pLeft(nullptr), pRight(nullptr), pParent(nullptr)
        {
            if (other.pLeft)
            {
                pLeft = new Self(*other.pLeft);
                pLeft->pParent = this;
            }
            if (other.pRight)
            {
                pRight = new Self(*other.pRight);
                pRight->pParent = this;
            }
        }

        BinaryHeapNode(Self&& other) noexcept : data(other.data), pLeft(other.pLeft), pRight(other.pRight), pParent(other.pParent)
        {
            other.pLeft = nullptr;
            other.pRight = nullptr;
            other.pParent = nullptr;
        }

        BinaryHeapNode& operator=(const Self& other)
        {
            if(this == &other) return *this;

            data = other.data;
            if (other.pLeft)
            {
                pLeft = new Self(*other.pLeft);
                pLeft->pParent = this;
            }
            if (other.pRight)
            {
                pRight = new Self(*other.pRight);
                pRight->pParent = this;
            }

            return *this;
        }

        BinaryHeapNode& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;

            data = other.data;
            other.data = nullptr;
            
            return *this;
        }
        
        ~BinaryHeapNode() = default;
    };

    template<typename Value>
    class BinaryHeap
    {
        typedef BinaryHeapNode<Value> Node;
        typedef BinaryHeap<Value> Self;
    public:
        /**
         * \brief Default constructor
         */
        BinaryHeap() : pRoot(nullptr) {}

        /**
         * \brief Copy constructor
         */
        BinaryHeap(const Self& other) : pRoot(Node(*other.pRoot)) { }

        /**
         * \brief Move constructor
         */
        BinaryHeap(Self&& other) noexcept : pRoot(other.pRoot)
        {
            other.pRoot = nullptr;
        }

        /**
         * \brief Destructor
         */
        ~BinaryHeap()
        {
            Clear();
        }

        /**
         * \brief Copy assignment operator
         */
        Self& operator=(const Self& other)
        {
            if (this == &other) return *this;

            Clear();
            pRoot = new Node(*other.pRoot);

            return *this;
        }

        /**
         * \brief Move assignment operator
         */
        Self& operator=(Self&& other) noexcept
        {
            if(this == &other) return *this;

            Clear();
            pRoot = other.pRoot;
            other.pRoot = nullptr;

            return *this;
        }

        /**
         * \brief Clears the heap by deleting all nodes
         */
        void Clear()
        {
            InternalClear(pRoot);
        }

        /**
         * \brief Inserts a new value into the heap
         */
        void Insert(const Value& data)
        {
            if(pRoot == nullptr)
            {
                pRoot = new Node(data);
                return;
            }
            
            InternalHeapifyUp(InternalInsert(pRoot, data));
        }

        /**
         * \brief Extracts the top value from the heap
         */
        Value Extract()
        {
            if(pRoot == nullptr) throw std::exception("Heap is empty");
            
            Value temp = pRoot->data;
            
            InternalDeleteTop();
            InternalHeapifyDown();
            
            return temp;
        }

        /**
         * \brief Returns the top value from the heap
         */
        ::Size Size()
        {
            return InternalSize(pRoot);
        }

    protected:
        /**
         * \brief Clears the heap by deleting all nodes
         */
        void InternalClear(Node* pTemp)
        {
            if(pTemp == nullptr) return;
            InternalClear(pTemp->pLeft);
            InternalClear(pTemp->pRight);
            Free(&pTemp);
        }

        /**
         * \brief Inserts a new value into the heap 
         */
        Node* InternalInsert(Node* pTemp, const Value& data)
        {
            if(pTemp == nullptr) return nullptr;
            
            if(pTemp->pLeft == nullptr)
            {
                pTemp->pLeft = new Node(data);
                pTemp->pLeft->pParent = pTemp;
                return pTemp->pLeft;
            }
            else if(pTemp->pRight == nullptr)
            {
                pTemp->pRight = new Node(data);
                pTemp->pRight->pParent = pTemp;
                return pTemp->pRight;
            }
            else
            {
                const ::Size leftSize = InternalSize(pTemp->pLeft);
                const ::Size rightSize = InternalSize(pTemp->pRight);
                if(leftSize <= rightSize)
                {
                    return InternalInsert(pTemp->pLeft, data);
                }
                else
                {
                    return InternalInsert(pTemp->pRight, data);
                }
            }
        }

        /**
         * \brief Heapifies the heap upwards
         */
        void InternalHeapifyUp(Node* pTemp)
        {
            if(pTemp == nullptr) return;
            if(pTemp->pParent == nullptr) return;

            if(pTemp->data > pTemp->pParent->data)
            {
                Swap(pTemp->data, pTemp->pParent->data);
                InternalHeapifyUp(pTemp->pParent);
            }
        }

        /**
         * \brief Heapifies the heap downwards
         */
        void InternalHeapifyDown()
        {
            if(pRoot == nullptr) return;

            Node* pTemp = pRoot;
            
            while(pTemp->pLeft != nullptr || pTemp->pRight != nullptr)
            {
                Node* pReplacement =
                    pTemp->pLeft == nullptr ? pTemp->pRight :
                    pTemp->pRight == nullptr ? pTemp->pLeft :
                    pTemp->pLeft->data > pTemp->pRight->data ? pTemp->pLeft : pTemp->pRight;

                if(pReplacement->data <= pTemp->data) break;
                
                Swap(pReplacement->data, pTemp->data);
                pTemp = pReplacement;
            }
        }

        /**
         * \brief Deletes the top value from the heap by replacing it with the last value
         */
        void InternalDeleteTop()
        {
            Node* pReplacement = InternalFindReplacement(pRoot);
            if(pReplacement == nullptr) return;

            if(pReplacement == pRoot)
            {
                Free(&pRoot);
                return;
            }
            
            if(pReplacement->pParent->pLeft == pReplacement)
            {
                pReplacement->pParent->pLeft = nullptr;
            }
            else
            {
                pReplacement->pParent->pRight = nullptr;
            }

            Swap(pRoot->data, pReplacement->data);
            Free(&pReplacement);
        }

        /**
         * \brief Finds the last node for replacing the top value
         */
        Node* InternalFindReplacement(Node* pTemp)
        {
            if(pTemp == nullptr) return nullptr;
            if(pTemp->pLeft == nullptr && pTemp->pRight == nullptr) return pTemp;

            const ::Size leftSize = InternalSize(pTemp->pLeft);
            const ::Size rightSize = InternalSize(pTemp->pRight);

            if(leftSize > rightSize)
            {
                return InternalFindReplacement(pTemp->pLeft);
            }
            else
            {
                return InternalFindReplacement(pTemp->pRight);
            }
        }

        /**
         * \brief Returns the size of the heap
         */
        ::Size InternalSize(Node* pTemp)
        {
            if(pTemp == nullptr) return 0;
            return 1 + InternalSize(pTemp->pLeft) + InternalSize(pTemp->pRight);
        }
        
    private:
        Node* pRoot;
    };
}
