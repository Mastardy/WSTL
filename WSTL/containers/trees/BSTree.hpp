#pragma once
#include <algorithm>

#include "memory/Memory.hpp"

namespace WSTL
{
    template <typename Key, typename Value>
    struct BSTNode
    {
        typedef BSTNode<Key, Value> Node;

        Key key;
        Value value;
        Node* pLeft;
        Node* pRight;

        BSTNode(Key k, Value v) : key(k), value(std::move(v)), pLeft(nullptr), pRight(nullptr) { }
    };

    template <typename Key, typename Value>
    struct BSTIterator
    {
        typedef BSTNode<Key, Value> Node;

        Node* pNode = nullptr;
        Node* pParent = nullptr;
        bool isLeft = false;

        auto operator<=>(const BSTIterator& right)
        {
            return pNode->key <=> right.pNode->key;
        }
    };

    template <typename Key, typename Value>
    class BSTree
    {
        typedef BSTNode<Key, Value> Node;
        typedef BSTIterator<Key, Value> Iterator;

    public:
        BSTree() : pRoot(nullptr)
        {
        }

        /**
         * \brief Inserts a new node into the tree
         */
        void Insert(Key key, Value value)
        {
            if (pRoot == nullptr)
            {
                pRoot = new Node(key, value);
                return;
            }

            InternalInsert(pRoot, key, value);
        }

        /**
         * \brief Searches for a node with the given key
         */
        Node* Search(Key key)
        {
            return InternalSearch(pRoot, key).pNode;
        }

        /**
         * \brief Deletes a node with the given key
         */
        void Delete(Key key)
        {
            auto pSearch = InternalSearch(pRoot, key);
            
            if (pSearch.pNode == nullptr) return;
            if (pSearch.pNode->pLeft == nullptr && pSearch.pNode->pRight == nullptr)
            {
                if(pSearch.isLeft) pSearch.pParent->pLeft = nullptr;
                else pSearch.pParent->pRight = nullptr;
                
                Free(&pSearch.pNode);
            }
            else if (pSearch.pNode->pLeft == nullptr || pSearch.pNode->pRight == nullptr)
            {
                if(pSearch.isLeft) pSearch.pParent->pLeft = pSearch.pNode->pLeft == nullptr ? pSearch.pNode->pRight : pSearch.pNode->pLeft;
                else pSearch.pParent->pRight = pSearch.pNode->pLeft == nullptr ? pSearch.pNode->pRight : pSearch.pNode->pLeft;

                Free(&pSearch.pNode);
            }
            else
            {
                auto pTempParent = pSearch.pNode;
                auto pTemp = pSearch.pNode->pRight;
                while (pTemp->pLeft != nullptr)
                {
                    pTempParent = pTemp;
                    pTemp = pTemp->pLeft;
                }

                if(pSearch.isLeft) pSearch.pParent->pLeft = pTemp;
                else pSearch.pParent->pRight = pTemp;
                pTemp->pLeft = pSearch.pNode->pLeft;
                pTemp->pRight = pSearch.pNode->pRight;

                pTempParent->pLeft = nullptr;
                
                Free(&pSearch.pNode);
            }
        }
        
    protected:
        /**
         * \brief Internal insert function
         */
        Node* InternalInsert(Node* pTemp, Key key, Value value)
        {
            if (pTemp == nullptr) return nullptr;

            if (key < pTemp->key)
            {
                if (pTemp->pLeft != nullptr)
                {
                    return InternalInsert(pTemp->pLeft, key, value);
                }
                else
                {
                    pTemp->pLeft = new Node(key, value);
                    return pTemp->pLeft;
                }
            }
            else if (key > pTemp->key)
            {
                if (pTemp->pRight != nullptr)
                {
                    return InternalInsert(pTemp->pRight, key, value);
                }
                else
                {
                    pTemp->pRight = new Node(key, value);
                    return pTemp->pRight;
                }
            }

            return nullptr;
        }

        Iterator InternalSearch(Node* pTemp, Key key, Node* pParent = nullptr, bool isLeft = false)
        {
            if (pTemp == nullptr) return { nullptr, nullptr, false };
            if (pTemp->key == key) return { pTemp, pParent, isLeft };

            Iterator pLeft = InternalSearch(pTemp->pLeft, key, pTemp, true);
            if (pLeft.pNode != nullptr) return pLeft;
            Iterator pRight = InternalSearch(pTemp->pRight, key, pTemp);
            if (pRight.pNode != nullptr) return pRight;
            return { nullptr, nullptr, false };
        }

    private:
        Node* pRoot;
    };
}
