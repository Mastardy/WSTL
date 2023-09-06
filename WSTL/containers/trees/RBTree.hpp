#pragma once
#include <algorithm>

#include "containers/Pair.hpp"
#include "memory/Memory.hpp"

namespace WSTL
{
    template<typename Key, typename Value>
    struct RBTNode
    {
        typedef RBTNode<Key, Value> Node;
        
        Key key;
        Value value;
        Node* pParent;
        Node* pLeft;
        Node* pRight;
        bool isBlack;

        RBTNode(const Key& key, Value value, bool isBlack = true) : key(key), value(std::move(value)), pParent(nullptr), pLeft(nullptr), pRight(nullptr), isBlack(isBlack) { }
    };
    
    template<typename Key, typename Value>
    class RBTree
    {
        typedef RBTNode<Key, Value> Node;

    public:
        RBTree() : pRoot(nullptr) { }

        RBTree(const RBTree& other) : pRoot(nullptr)
        {
            if(other.pRoot == nullptr) return;
            pRoot = new Node(other.pRoot->key, other.pRoot->value, other.pRoot->isBlack);
            InternalCopy(other.pRoot);
        }

        RBTree(RBTree&& other) noexcept
        {
            if(other.pRoot == nullptr) return;
            pRoot = other.pRoot;
            other.pRoot = nullptr;
        }

        RBTree& operator=(const RBTree& other) 
        {
            if(this == &other) return *this;

            if(other.pRoot == nullptr)
            {
                Clear();
                return *this;
            }
            
            InternalCopy(other.pRoot);
            return *this;
        }

        RBTree& operator=(RBTree&& other) noexcept
        {
            if(this == &other) return *this;

            Clear();
            pRoot = other.pRoot;
            other.pRoot = nullptr;
            
            return *this;
        }

        ~RBTree()
        {
            Clear();
        }

        Value operator[](const Key& key)
        {
            return GetValue(key);
        }
        
        void Insert(const Key& key, const Value& value)
        {
            if(pRoot == nullptr)
            {
                pRoot = new Node(key, value);
                return;
            }
            InternalCheckViolation(InternalInsert(pRoot, key, value));
        }

        inline Value Get(const Key& key)
        {
            return GetValue(key);
        }
        
        Pair<Key, Value> GetPair(const Key& key)
        {
            Node* pTemp = InternalSearch(pRoot, key);
            if(pTemp == nullptr) return Pair<Key, Value>(key, Value());
            return Pair<Key, Value>(key, InternalSearch(pRoot, key)->value);
        }

        Value GetValue(const Key& key)
        {
            Node* pTemp = InternalSearch(pRoot, key);
            return pTemp == nullptr ? Value() : pTemp->value;
        }

        void Delete(const Key& key)
        {
            auto pDelete = InternalSearch(pRoot, key);
            if(pDelete == nullptr) return;
            bool isBlack = pDelete->isBlack;
            if(pDelete->pRight != nullptr && pDelete->pLeft != nullptr)
            {
                auto pSuccessor = InternalFindMin(pDelete->pRight);
                isBlack = pSuccessor->isBlack;

                auto pSuccessorChild = pSuccessor->pRight;
                InternalTransplant(pSuccessor, pSuccessorChild);
                pDelete->pRight->pParent = pSuccessor;
                pSuccessor->pRight = pDelete->pRight;

                InternalTransplant(pDelete, pSuccessor);
                pSuccessor->pLeft = pDelete->pLeft;

                InternalCheckViolationDelete(pSuccessorChild, isBlack);
            }
            else if(pDelete->pRight != nullptr)
            {
                InternalTransplant(pDelete, pDelete->pRight);
                InternalCheckViolationDelete(pDelete->pRight, isBlack);
            }
            else if(pDelete->pLeft != nullptr)
            {
                InternalTransplant(pDelete, pDelete->pLeft);
                InternalCheckViolationDelete(pDelete->pLeft, isBlack);
            }
            else
            {
                if(pDelete->pParent == nullptr)
                {
                    Free(&pDelete);
                    pRoot = nullptr;
                    return;
                }
                if(pDelete->pParent->pRight == pDelete) pDelete->pParent->pRight = nullptr;
                else pDelete->pParent->pLeft = nullptr;
            }
            Free(&pDelete);
        }

        inline bool IsEmpty() const
        {
            return pRoot == nullptr;
        }

        inline Size Size() const
        {
            return InternalSize(pRoot);
        }
        
        inline void Clear()
        {
            InternalClear(pRoot);
            pRoot = nullptr;
        }
        
    protected:
        Node* InternalInsert(Node* pTemp, const Key& key, const Value& value)
        {
            if(pTemp == nullptr) return nullptr;

            if(key < pTemp->key)
            {
                if(pTemp->pLeft != nullptr) return InternalInsert(pTemp->pLeft, key, value);
                
                pTemp->pLeft = new Node(key, value, false);
                pTemp->pLeft->pParent = pTemp;
                return pTemp->pLeft;
            }
            else if(key > pTemp->key)
            {
                if(pTemp->pRight != nullptr) return InternalInsert(pTemp->pRight, key, value);

                pTemp->pRight = new Node(key, value, false);
                pTemp->pRight->pParent = pTemp;
                return pTemp->pRight;
            }

            return nullptr;
        }

        Node* InternalSearch(Node* pTemp, const Key& key)
        {
            if(pTemp == nullptr) return nullptr;

            if(key < pTemp->key)
            {
                if(pTemp->pLeft != nullptr) return InternalSearch(pTemp->pLeft, key);
                
            }
            else if(key > pTemp->key)
            {
                if(pTemp->pRight != nullptr) return InternalSearch(pTemp->pRight, key);
            }
            else
            {
                return pTemp;
            }
            
            return nullptr;
        }

        Node* InternalFindMin(Node* pTemp) const
        {
            if(pTemp == nullptr) return nullptr;
            if(pTemp->pLeft == nullptr) return pTemp;
            return InternalFindMin(pTemp->pLeft);
        }

        Node* InternalFindMax(Node* pTemp) const
        {
            if(pTemp == nullptr) return nullptr;
            if(pTemp->pRight == nullptr) return pTemp;
            return InternalFindMax(pTemp->pRight);
        }

        ::Size InternalSize(Node* pTemp) const
        {
            if(pTemp == nullptr) return 0;
            return InternalSize(pTemp->pLeft) + InternalSize(pTemp->pRight) + 1;
        }
        
        void InternalCheckViolation(Node* pTemp)
        {            
            while(!pTemp->pParent->isBlack)
            {
                Node* pParent = pTemp->pParent;
                if(pParent == nullptr) break;
                
                Node* pGrandParent = pParent->pParent;
                if(pGrandParent == nullptr) break;

                auto isUncleLeft = pParent != pGrandParent->pLeft;
                auto pUncle =  isUncleLeft ? pGrandParent->pLeft : pGrandParent->pRight;
                if(pUncle == nullptr) pUncle = new Node(Key(), Value(), true);

                if(!pUncle->isBlack)
                {
                    pParent->isBlack = true;
                    pUncle->isBlack = true;
                    pGrandParent->isBlack = false;
                    pTemp = pGrandParent;
                    if(pTemp->pParent == nullptr) break;
                }
                else
                {
                    if(isUncleLeft)
                    {
                        if(pTemp == pParent->pLeft)
                        {
                            pTemp = pParent;
                            InternalRotateRight(pTemp);
                            if(pTemp->pParent == nullptr) break;
                            pParent = pTemp->pParent;
                            pGrandParent = pParent->pParent;
                        }
                        pParent->isBlack = true;
                        pGrandParent->isBlack = false;
                        InternalRotateLeft(pGrandParent);
                    }
                    else
                    {
                        if(pTemp == pParent->pLeft)
                        {
                            pTemp = pParent;
                            InternalRotateLeft(pTemp);
                            if(pTemp->pParent == nullptr) break;
                            pParent = pTemp->pParent;
                            pGrandParent = pParent->pParent;
                        }
                        pParent->isBlack = true;
                        pGrandParent->isBlack = false;
                        InternalRotateRight(pGrandParent);
                    }
                }
            }
            pRoot->isBlack = true;
        }

        void InternalCheckViolationDelete(Node* pTemp, bool isBlack)
        {
            if(pTemp == nullptr) return;
            if(!isBlack) return;

            while(pTemp != pRoot && pTemp->isBlack)
            {
                if(pTemp == pTemp->pParent->pLeft)
                {
                    Node* pBrother = pTemp->pParent->pRight;
                    if(!pBrother->isBlack)
                    {
                        pBrother->isBlack = true;
                        pTemp->pParent->isBlack = false;
                        InternalRotateLeft(pTemp->pParent);
                        pBrother = pTemp->pParent->pRight;
                    }
                    if(pBrother->pLeft->isBlack && pBrother->pRight->isBlack)
                    {
                        pBrother->isBlack = false;
                        pTemp = pTemp->pParent;
                    }
                    else
                    {
                        if(pBrother->pRight->isBlack)
                        {
                            pBrother->pLeft->isBlack = true;
                            pBrother->isBlack = false;
                            InternalRotateRight(pBrother);
                            pBrother = pTemp->pParent->pRight;
                        }
                        pBrother->isBlack = pTemp->pParent->isBlack;
                        pTemp->pParent->isBlack = true;
                        pBrother->pRight->isBlack = true;
                        InternalRotateLeft(pTemp->pParent);
                        pTemp = pRoot;                        
                    }
                }
            }

            pTemp->isBlack = true;
        }
        
        void InternalTransplant(Node* pTarget, Node* pSource)
        {
            if(pTarget == nullptr) return;

            if(pTarget->pParent == nullptr)
            {                
                pRoot = pSource;
            }
            else if(pTarget == pTarget->pParent->pLeft)
            {
                pTarget->pParent->pLeft = pSource;
            }
            else
            {
                pTarget->pParent->pRight = pSource;
            }

            if(pSource != nullptr) pSource->pParent = pTarget->pParent;
        }

        void InternalRotateLeft(Node* pParent)
        {
            if(pParent == nullptr) return;
            if(pParent->pRight == nullptr) return;

            Node* pChild = pParent->pRight;

            pParent->pRight = pChild->pLeft;
            if(pParent->pRight != nullptr)
                pParent->pRight->pParent = pParent;
            
            pChild->pParent = pParent->pParent;
            if(pParent->pParent == nullptr)
                pRoot = pChild;
            else if(pParent == pParent->pParent->pLeft)
            {
                pParent->pParent->pLeft = pChild;
            }
            else
            {
                pParent->pParent->pRight = pChild;
            }
            
            pChild->pLeft = pParent;
            pParent->pParent = pChild;
        }

        void InternalRotateRight(Node* pParent)
        {
            if(pParent == nullptr) return;
            if(pParent->pLeft == nullptr) return;

            Node* pChild = pParent->pLeft;

            pParent->pLeft = pChild->pRight;
            if(pParent->pLeft != nullptr)
                pParent->pLeft->pParent = pParent;
            
            pChild->pParent = pParent->pParent;
            if(pParent->pParent == nullptr)
                pRoot = pChild;
            else if(pParent == pParent->pParent->pLeft)
            {
                pParent->pParent->pLeft = pChild;
            }
            else
            {
                pParent->pParent->pRight = pChild;
            }
            
            pChild->pRight = pParent;
            pParent->pParent = pChild;
        }

        void InternalCopy(Node* pOther)
        {
            if(pOther == nullptr) return;
            Node* pTemp = InternalSearch(pRoot, pOther->key);
            if(pOther->pLeft != nullptr)
            {
                pTemp->pLeft = new Node(pOther->pLeft->key, pOther->pLeft->value, pOther->pLeft->isBlack);
                pTemp->pLeft->pParent = pTemp;
                InternalCopy(pOther->pLeft);
            }
            if(pOther->pRight != nullptr)
            {
                pTemp->pRight = new Node(pOther->pRight->key, pOther->pRight->value, pOther->pRight->isBlack);
                pTemp->pRight->pParent = pTemp;
                InternalCopy(pOther->pRight);
            }
        }
        
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

    template<typename Key, typename Value>
    using RedBlackTree = RBTree<Key, Value>;
}
