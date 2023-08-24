#pragma once
#include <stdexcept>

namespace WSTL
{
    template <typename T>
    struct ListNode
    {
        T value;
        ListNode* pNext;
        ListNode* pPrev;

        ListNode(const T& value) : value(value), pNext(nullptr), pPrev(nullptr) {}
        
        /**
         * \brief Insert a node after this one
         */
        void Insert(ListNode* pNode)
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
        static void Swap(ListNode& a, ListNode& b)
        {
            ListNode* pNextTemp = a.pNext;
            a.pNext = b.pNext;
            b.pNext = pNextTemp;

            ListNode* pTempPrev = a.pPrev;
            a.pPrev = b.pPrev;
            b.pPrev = pTempPrev;
        }

        /**
         * \brief Insert a range of nodes after this one
         */
        void InsertRange(ListNode* pFirst, ListNode* pLast)
        {
            pLast->pNext = this->pNext;
            this->pNext->pPrev = pLast;
            
            pFirst->pPrev = this;
            this->pNext = pFirst;
        }
    };

    template <typename T, typename Pointer, typename Reference>
    struct ListIterator
    {
    public:
        ListNode<T>* pNode;

        ListIterator() noexcept : pNode(nullptr) {}
        ListIterator(const ListNode<T>* pNode) noexcept : pNode(const_cast<ListNode<T>*>(pNode)) {} 
        ListIterator(const ListIterator& other) noexcept : pNode(const_cast<ListNode<T>*>(other.pNode)) {}

        ListIterator<T, Pointer, Reference>& operator=(const ListIterator<T, Pointer, Reference>& other) noexcept
        {
            if(this == &other) return *this;
            
            pNode = other.pNode;
            return *this;
        }

        ListIterator(const ListIterator<T, Pointer, Reference>&& other) = delete;
        ListIterator<T, Pointer, Reference>& operator=(const ListIterator<T, Pointer, Reference>&& other) = delete;
        ~ListIterator() = default;
        
        ListIterator<T, Pointer, Reference> next() const noexcept
        {
            return ListIterator<T, Pointer, Reference>(pNode->pNext);
        }

        ListIterator<T, Pointer, Reference> prev() const noexcept
        {
            return ListIterator<T, Pointer, Reference>(pNode->pPrev);
        }

        Reference operator*() const noexcept
        {
            return pNode->value;
        }

        Pointer operator->() const noexcept
        {
            return &pNode->value;
        }

        ListIterator<T, Pointer, Reference>& operator++() noexcept
        {
            pNode = pNode->pNext;
            return *this;
        }

        ListIterator<T, Pointer, Reference> operator++(int) noexcept
        {
            ListIterator<T, Pointer, Reference> temp = *this;
            pNode = pNode->pNext;
            return temp;
        }
        
        ListIterator<T, Pointer, Reference>& operator--() noexcept
        {
            pNode = pNode->pPrev;
            return *this;
        }

        ListIterator<T, Pointer, Reference> operator--(int) noexcept
        {
            ListIterator<T, Pointer, Reference> temp = *this;
            pNode = pNode->pPrev;
            return temp;
        }

        bool operator==(const ListIterator<T, Pointer, Reference>& other) const noexcept
        {
            return pNode == other.pNode;
        }

        bool operator!=(const ListIterator<T, Pointer, Reference>& other) const noexcept
        {
            return pNode != other.pNode;
        }
    };
    
    template<typename T>
    class List
    {
    public:
        /**
         * \brief Default Constructor
         */
        List() : size(0), pHead(nullptr), pTail(nullptr)  {}

        /**
         * \brief Default Fill Constructor
         */
        List(size_t amount, const T& value) : size(0), pHead(nullptr), pTail(nullptr)
        {
            for(size_t i = 0; i < amount; i++)
            {
                PushBack(value);
            }
        }
        
        /**
         * \brief Copy Constructor
         */
        List(const List<T>& other) : size(0), pHead(nullptr), pTail(nullptr)
        {
            for(auto element : other)
            {
                PushBack(element);
            }
        }

        /**
         * \brief Copy Assignment
         */
        List& operator=(const List<T>& other)
        {
            if(this == &other) return *this;
            
            Clear();
            
            for(size_t i = 0; i < other.Size(); i++)
            {
                PushBack(other[i]);
            }
            
            return *this;
        }

        /**
         * \brief Default Destructor
         */
        ~List()
        {
            Clear();
        }

        /**
         * \brief Assigns a value to the element at the specified index
         */
        void Assign(size_t index, const T& value)
        {
            if(index > size) throw std::out_of_range("Index out of range");
            else if(index == size) PushBack(value);
            else if(index == 0) PushFront(value);
            else NodeAt(index)->Insert(new ListNode<T>(value));
        }

        /**
         * \brief Assigns a value to the back of the list
         */
        void PushBack(const T& value)
        {
            auto pNewNode = new ListNode<T>(value);

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
            
            size++;
        }

        /**
         * \brief Assigns a value to the front of the list
         */
        void PushFront(const T& value)
        {
            auto pNewNode = new ListNode<T>(value);

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
            size--;
        }

        /**
         * \brief Deletes the first element of the list
         */
        void PopFront()
        {
            if(IsEmpty()) return;

            if(size == 1)
            {
                delete pTail;
                pHead = nullptr;
                pTail = nullptr;
                size--;
                return;
            }
            
            pHead = pHead->pNext;
            delete pHead->pPrev;
            pHead->pPrev = nullptr;
            size--;
        }

        /**
         * \brief Deletes all elements of the list 
         */
        void Clear()
        {
            if(IsEmpty()) return;
            
            ListNode<T>* pCurrent = pHead;

            while(pCurrent != nullptr)
            {
                ListNode<T>* pNext = pCurrent->pNext;
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
        void Remove(size_t index)
        {
            if(index >= size) throw std::out_of_range("Index out of range");
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
            if(IsEmpty()) throw std::out_of_range("List is empty");
            return pHead->value;
        }

        /**
         * \brief Gets the front value
         */
        const T& Front() const
        {
            if(IsEmpty()) throw std::out_of_range("List is empty");
            return pHead->value;
        }

        /**
         * \brief Gets the back value
         */
        T& Back()
        {
            if(IsEmpty()) throw std::out_of_range("List is empty");
            return pTail->value;
        }

        /**
         * \brief Gets the back value
         */
        const T& Back() const
        {
            if(IsEmpty()) throw std::out_of_range("List is empty");
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
        size_t Size() const
        {
            return size;
        }

        /**
         * \brief Returns iterator begin
         */
        ListIterator<T, T*, T&> begin()
        {
            return ListIterator<T, T*, T&>(pHead);
        }

        /**
         * \brief Returns const iterator begin
         */
        ListIterator<T, const T*, const T&> begin() const
        {
            return ListIterator<T, const T*, const T&>(pHead);
        }

        /**
         * \brief Returns iterator end
         */
        ListIterator<T, T*, T&> end()
        {
            return ListIterator<T, T*, T&>(nullptr);
        }

        /**
         * \brief Returns const iterator end
         */
        ListIterator<T, const T*, const T&> end() const
        {
            return ListIterator<T, const T*, const T&>(nullptr);
        }

        List& operator=(List&& other) = delete;
        List(List&& other) = delete;

    protected:
        /**
         * \brief Get Node at the specified index
         */
        ListNode<T>* NodeAt(size_t index)
        {
            if(index >= size) throw std::out_of_range("Index out of range");
            
            auto pCurrent = pHead;

            for(size_t i = 0; i < index; i++)
            {
                pCurrent = pCurrent->pNext;
            }
                        
            return pCurrent;
        }
        
    private:
        size_t size;
        ListNode<T>* pHead;
        ListNode<T>* pTail;
    };
}
