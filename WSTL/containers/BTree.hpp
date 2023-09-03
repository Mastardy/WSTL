#pragma once
#include "Queue.hpp"

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

    class BTreeIterator
    {
        
    };

    template<typename T>
    class BTree
    {
        typedef BTreeNode<T> Node;
        
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
            
            Queue<Node*> queue;
            queue.Push(pRoot);

            while(!queue.IsEmpty())
            {
                Node* pTemp = queue.Pop();

                if(pTemp->pLeft == nullptr)
                {
                    pTemp->pLeft = new Node(value);
                    return;
                }
                else
                {
                    queue.Push(pTemp->pLeft);
                }

                if(pTemp->pRight == nullptr)
                {
                    pTemp->pRight = new Node(value);
                    return;
                }
                else
                {
                    queue.Push(pTemp->pRight);
                }
            }
        }

        /**
         * \brief Delete a value from the tree
         */
        void Delete(T value)
        {
            Node* pSearch = Search(value);

            
        }

        /**
         * \brief 
         */
        void DeleteAll(T value)
        {
            
        }
        
        /**
         * \brief 
         */
        Node* Search(T value)
        {
            Queue<Node*> queue;
            queue.Push(pRoot);

            while(!queue.IsEmpty())
            {
                Node* pTemp = queue.Front();
                queue.Pop();

                if(pTemp->data == value) return pTemp;
                if(pTemp->pLeft != nullptr) queue.Push(pTemp->pLeft);
                if(pTemp->pRight != nullptr) queue.Push(pTemp->pRight);
            }

            return nullptr;
        }
    
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

        Node* GetRoot() { return pRoot; }
        
    private:
        Node* pRoot;
    };
}
