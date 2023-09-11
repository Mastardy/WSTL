#pragma once
#include <stdexcept>

namespace WSTL
{
    template <typename T>
    struct ListNode
    {
    private:
        typedef ListNode<T> Self;

    public:
        T value;
        Self* pNext;
        Self* pPrev;

        ListNode(T value) : value(std::move(value)), pNext(nullptr), pPrev(nullptr) {}
        
        /**
         * \brief Insert a node after this one
         */
        void Insert(Self* pNode)
        {
            if(this->pNext != nullptr)
            {
                this->pNext->pPrev = pNode;
                pNode->pNext = this->pNext;
            }
            
            pNode->pPrev = this;
            this->pNext = pNode;
        }

        /**
         * \brief Remove this node from the list.
         */
        void Remove()
        {
            if(pNext != nullptr && pPrev != nullptr)
            {
                pNext->pPrev = pPrev;
                pPrev->pNext = pNext;
            }
            else if(pNext != nullptr)
            {
                pNext->pPrev = nullptr;
            }
            else if(pPrev != nullptr)
            {
                pPrev->pNext = nullptr;
            }
            delete this;
        }

        /**
         * \brief Swap two nodes
         */
        static void Swap(Self& a, Self& b)
        {
            Self* pNextTemp = a.pNext;
            a.pNext = b.pNext;
            b.pNext = pNextTemp;

            Self* pTempPrev = a.pPrev;
            a.pPrev = b.pPrev;
            b.pPrev = pTempPrev;
        }

        /**
         * \brief Insert a range of nodes after this one
         */
        void InsertRange(Self* pFirst, Self* pLast)
        {
            if(pFirst == nullptr || pLast == nullptr) return;
            
            pLast->pNext = this->pNext;
            this->pNext->pPrev = pLast;
            
            pFirst->pPrev = this;
            this->pNext = pFirst;
        }
    };

    template <typename T, typename Pointer, typename Reference>
    struct ListIterator
    {
        typedef ListNode<T> Node;
        typedef ListIterator<T, Pointer, Reference> Self;
        
    public:
        Node* pNode;

        ListIterator() noexcept : pNode(nullptr) {}
        ListIterator(const Node* pNode) noexcept : pNode(const_cast<Node*>(pNode)) {} 
        ListIterator(const Self& other) noexcept : pNode(const_cast<Node*>(other.pNode)) {}

        Self& operator=(const Self& other) noexcept
        {
            if(this == &other) return *this;
            
            pNode = other.pNode;
            return *this;
        }

        ListIterator(const Self&& other) = delete;
        Self& operator=(const Self&& other) = delete;
        ~ListIterator() = default;
        
        Self next() const noexcept
        {
            return Self(pNode->pNext);
        }

        Self prev() const noexcept
        {
            return Self(pNode->pPrev);
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
        
        Self& operator--() noexcept
        {
            pNode = pNode->pPrev;
            return *this;
        }

        Self operator--(int) noexcept
        {
            Self temp = *this;
            pNode = pNode->pPrev;
            return temp;
        }

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
    class List
    {
        typedef ListNode<T> Node;
        typedef ListIterator<T, T*, T&> Iterator;
        typedef ListIterator<T, const T*, const T&> ConstIterator;
        typedef List<T> Self;
        
    public:
        /**
         * \brief Default Constructor
         */
        List() : size(0), pHead(nullptr), pTail(nullptr)  {}

        /**
         * \brief Default Fill Constructor
         */
        List(Size amount, const T& value) : size(0), pHead(nullptr), pTail(nullptr)
        {
            for(::Size i = 0; i < amount; i++)
            {
                PushBack(value);
            }
        }
        
        /**
         * \brief Copy Constructor
         */
        List(const Self& other) : size(0), pHead(nullptr), pTail(nullptr)
        {
            for(auto element : other)
            {
                PushBack(element);
            }
        }

        /**
         * \brief Move Constructor
         */
        List(Self&& other) noexcept : size(other.size), pHead(other.pHead), pTail(other.pTail)
        {
            other.size = 0;
            other.pHead = nullptr;
            other.pTail = nullptr;
        }
        
        /**
         * \brief Copy Assignment
         */
        Self& operator=(const Self& other)
        {
            if(this == &other) return *this;
            
            Clear();
            
            for(::Size i = 0; i < other.Size(); i++)
            {
                PushBack(other[i]);
            }
            
            return *this;
        }

        /**
         * \brief Move Assignment
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
         * \brief Default Destructor
         */
        ~List()
        {
            Clear();
        }

        T& operator[](Size index)
        {
            return NodeAt(index)->value;
        }

        const T& operator[](Size index) const
        {
            return NodeAt(index)->value;
        }
        
        /**
         * \brief Assigns a value to the element at the specified index
         */
        void Assign(Size index, const T& value)
        {
            if(index > size || index < 0) throw std::out_of_range("Index out of range");
            else if(index == size) PushBack(value);
            else if(index == 0) PushFront(value);
            else NodeAt(index)->Insert(new Node(value));
        }

        /**
         * \brief Assigns a value to the back of the list
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
                pTail->pNext = pNewNode;
                pNewNode->pPrev = pTail;
                pTail = pNewNode;
            }
            
            ++size;
        }

        /**
         * \brief Assigns a value to the front of the list
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
                pHead->pPrev = pNewNode;
                pNewNode->pNext = pHead;
                pHead = pNewNode;
            }
            size++;
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
                size--;
                return;
            }
            
            pTail = pTail->pPrev;
            delete pTail->pNext;
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
                size--;
                return;
            }
            
            pHead = pHead->pNext;
            delete pHead->pPrev;
            pHead->pPrev = nullptr;
            --size;
        }

        /**
         * \brief Deletes all elements of the list 
         */
        void Clear()
        {
            if(IsEmpty()) return;
            
            Node* pCurrent = pHead;

            while(pCurrent != nullptr)
            {
                Node* pNext = pCurrent->pNext;
                pCurrent->Remove();
                pCurrent = pNext;
            }
            
            pTail = nullptr;
            pHead = nullptr;
            
            size = 0;
        }

        /**
         * \brief Deletes the element at the specified index
         */
        void Remove(Size index)
        {
            if(index >= size || index < 0) throw std::out_of_range("Index out of range");
            if(index == 0) PopFront();
            else if(index == size - 1) PopBack();
            else
            {
                NodeAt(index)->Remove();
                size--;
            }
        }

        /**
         * \brief Gets the front value
         */
        T& Front()
        {
            if(IsEmpty()) throw std::runtime_error("List is empty");
            return pHead->value;
        }

        /**
         * \brief Gets the front value
         */
        const T& Front() const
        {
            if(IsEmpty()) throw std::runtime_error("List is empty");
            return pHead->value;
        }

        /**
         * \brief Gets the back value
         */
        T& Back()
        {
            if(IsEmpty()) throw std::runtime_error("List is empty");
            return pTail->value;
        }

        /**
         * \brief Gets the back value
         */
        const T& Back() const
        {
            if(IsEmpty()) throw std::runtime_error("List is empty");
            return pTail->value;
        }

        /**
         * \brief Returns true if the list is empty
         */
        bool IsEmpty() const
        {
            return size == 0;
        }

        /**
         * \brief Returns the size of the list
         */
        Size Size() const
        {
            return size;
        }

        /**
         * \brief Returns iterator begin
         */
        Iterator begin()
        {
            return Iterator(pHead);
        }

        /**
         * \brief Returns const iterator begin
         */
        ConstIterator begin() const
        {
            return ConstIterator(pHead);
        }

        /**
         * \brief Returns iterator end
         */
        Iterator end()
        {
            return Iterator(nullptr);
        }

        /**
         * \brief Returns const iterator end
         */
        ConstIterator end() const
        {
            return ConstIterator(nullptr);
        }

    protected:
        /**
         * \brief Get Node at the specified index
         */
        Node* NodeAt(::Size index)
        {
            if(index >= size || index < 0) throw std::out_of_range("Index out of range.");
            
            auto pCurrent = pHead;

            for(::Size i = 0; i < index; i++)
            {
                pCurrent = pCurrent->pNext;
            }
                        
            return pCurrent;
        }

        const Node* NodeAt(::Size index) const
        {
            if(index >= size) throw std::out_of_range("Index out of range");
            
            auto pCurrent = pHead;

            for(::Size i = 0; i < index; i++)
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
