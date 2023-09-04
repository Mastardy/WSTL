#pragma once
#include "Types.hpp"
#include "Utility.hpp"

namespace WSTL
{
    template<typename T>
    struct BTreeNode
    {
        T data;
        BTreeNode<T>* pLeft;
        BTreeNode<T>* pRight;

        BTreeNode(T data = T()) : data(data), pLeft(nullptr), pRight(nullptr) {}
    };

    template<typename T>
    struct BTreeIterator
    {
        typedef BTreeNode<T> Node;

        Node* pNode = nullptr;
        Node* pParent = nullptr;
        bool isLeft = false;

        auto operator<=>(const BTreeIterator&) const = default;
    };

    template<typename T>
    class BTree
    {
        typedef BTreeNode<T> Node;
        typedef BTreeIterator<T> Iterator;
        
    public:
        /**
         * \brief Default Constructor
         */
        BTree() : pRoot(nullptr) { }
        
        /**
         * \brief Insert a value into the tree
         */
        void Insert(T value)
        {
            if(pRoot == nullptr)
            {
                pRoot = new Node(value);
                return;
            }
            
            Node* pNew = InternalGetNewNode();
            pNew->data = value;
        }

        /**
         * \brief Delete a value from the tree
         */
        void Delete(T value)
        {
            Node* pSearch = Search(value);
            if(pSearch == nullptr) return;
            Iterator itLast = InternalGetLast(pRoot, Height());
            if(itLast.pNode == nullptr) return;

            Swap(pSearch->data, itLast.pNode->data);

            if(itLast.isLeft)
            {
                itLast.pParent->pLeft = nullptr;
                Free(&(itLast.pNode));
            }
            else
            {
                itLast.pParent->pRight = nullptr;
                Free(&(itLast.pNode));
            }
        }
        
        /**
         * \brief Search for a value in the tree
         */
        inline Node* Search(T value)
        {
            return InternalSearch(pRoot, value);
        }

        inline Node* SearchLast(T value)
        {
            return InternalSearchLast(pRoot, value);
        }

        /**
         * \brief Get the last node in the tree
         */
        inline Node* GetLast()
        {
            return InternalGetLast(pRoot, Height()).pNode;
        }

        /**
         * \brief Get the height of the tree
         */
        inline int Height()
        {
            return InternalHeight(pRoot);
        }

        Size Size()
        {
            return InternalSize(pRoot);
        }

        bool IsEmpty()
        {
            return pRoot == nullptr;
        }

        void Clear()
        {
            InternalClear(pRoot);
            pRoot = nullptr;
        }

        /**
         * \brief Get the root node of the tree
         */
        Node* GetRoot() { return pRoot; }
        
    protected:
        /**
         * \brief Internal search function
         */
        Node* InternalSearch(Node* pTemp, T value)
        {
            if(pTemp == nullptr) return nullptr;
            if(pTemp->data == value) return pTemp;
            
            Node* pLeft = InternalSearch(pTemp->pLeft, value);
            if(pLeft != nullptr) return pLeft;
            Node* pRight = InternalSearch(pTemp->pRight, value);
            if(pRight != nullptr) return pRight;
            return nullptr;
        }

        /**
         * \brief Internal search function
         */
        Node* InternalSearchLast(Node* pTemp, T value)
        {
            if(pTemp == nullptr) return nullptr;
            if(pTemp->data == value) return pTemp;

            Node* pRight = InternalSearchLast(pTemp->pRight, value);
            if(pRight != nullptr) return pRight;
            Node* pLeft = InternalSearchLast(pTemp->pLeft, value);
            if(pLeft != nullptr) return pLeft;
            
            return nullptr;
        }

        /**
         * \brief Internal function to get the last node in the tree
         */
        Iterator InternalGetLast(Node* pTemp, int height)
        {
            if(pTemp == nullptr) return { };
            if(height == 1)
            {
                if(pTemp->pRight != nullptr)
                {
                    return { pTemp->pRight, pTemp, false };
                }
                else if(pTemp->pLeft != nullptr)
                {
                    return { pTemp->pLeft, pTemp, true };
                }

                return { };
            }

            auto pRight = InternalGetLast(pTemp->pRight, height - 1);
            if(pRight.pNode != nullptr) return pRight;
            auto pLeft = InternalGetLast(pTemp->pLeft, height - 1);
            if(pLeft.pNode != nullptr) return pLeft;
            
            return { };
        }

        /**
         * \brief Internal function to get a new node in the tree
         */
        Node* InternalGetNewNode()
        {
            const int height = Height();
            
            Node* pTemp = pRoot;
            int i = 0;
            while(pTemp->pRight != nullptr)
            {
                pTemp = pTemp->pRight;
                ++i;
            }
            if(height == i)
            {
                pTemp = pRoot;
                while(pTemp->pLeft != nullptr) pTemp = pTemp->pLeft;
                pTemp->pLeft = new Node();
                return pTemp->pLeft;
            }

            return InternalGetNewNodeRecursive(pRoot, height);
        }

        /**
         * \brief Internal function to get a new node in the tree
         */
        Node* InternalGetNewNodeRecursive(Node* pTemp, int height)
        {
            if(pTemp == nullptr) return nullptr;
            
            if(height == 1 && pTemp != nullptr)
            {
                if(pTemp->pLeft == nullptr)
                {
                    pTemp->pLeft = new Node();
                    return pTemp->pLeft;
                }
                else if(pTemp->pRight == nullptr)
                {
                    pTemp->pRight = new Node();
                    return pTemp->pRight;
                }
            }

            Node* pLeft = InternalGetNewNodeRecursive(pTemp->pLeft, height - 1);
            if(pLeft != nullptr) return pLeft;
            Node* pRight = InternalGetNewNodeRecursive(pTemp->pRight, height - 1);
            if(pRight != nullptr) return pRight;

            return nullptr;
        }

        /**
         * \brief Internal function to get the height of the tree
         */
        int InternalHeight(Node* pTemp)
        {
            if(pTemp->pLeft == nullptr) return 0;
            return 1 + InternalHeight(pTemp->pLeft);
        }

        /**
         * \brief Internal function to get the size of the tree
         */
        ::Size InternalSize(Node* pTemp)
        {
            if(pTemp == nullptr) return 0;
            return 1 + InternalSize(pTemp->pLeft) + InternalSize(pTemp->pRight);
        }

        /**
         * \brief Internal function to clear the tree
         */
        void InternalClear(Node* pTemp)
        {
            if(pTemp == nullptr) return;
            if(pTemp->pLeft != nullptr) InternalClear(pTemp->pLeft);
            if(pTemp->pRight != nullptr) InternalClear(pTemp->pRight);
            Free(&pTemp);
        }
        
    private:
        Node* pRoot;
    };
}
