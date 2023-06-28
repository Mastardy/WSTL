#pragma once

namespace WSTL
{
    template<typename T1, typename T2>
    class Pair
    {
    public:
        T1 first;
        T2 second;

        /**
         * \brief Default Constructor
         */
        Pair()
        {
            first = T1();
            second = T2();
        }

        /**
         * \brief Default Constructor
         */
        Pair(T1 first)
        {
            this->first = first;
            second = T2();
        }

        /**
         * \brief Default Constructor
         */
        Pair(T2 second)
        {
            first = T1();
            this->second = second;
        }
    
        /**
         * \brief Default Constructor
         */
        Pair(T1 first, T2 second)
        {
            this->first = first;
            this->second = second;
        }

        /**
         * \brief Copy Constructor
         */
        Pair(Pair<T1, T2>& other)
        {
            first = other.first;
            second = other.second;
        }

        /**
         * \brief Copy Assignment
         */
        Pair& operator=(const Pair<T1, T2>& other)
        {
            if(other == *this) return *this;
        
            first = other.first;
            second = other.second;
            return *this;
        }

        /**
         * \brief Swaps the values of the pairs
         */
        void Swap(Pair& other)
        {
            Pair temp = other;
            other = *this;
            *this = temp;
        }
    
        auto operator<=>(const Pair& p2) const = default;
    };
}