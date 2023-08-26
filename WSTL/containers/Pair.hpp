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
        Pair() : first(T1()), second(T2()) { }

        /**
         * \brief Default Constructor
         */
        Pair(T1 first) : first(first), second(T2()) { }

        /**
         * \brief Default Constructor
         */
        Pair(T2 second) : first(T1()), second(second) { }
    
        /**
         * \brief Default Constructor
         */
        Pair(T1 first, T2 second) : first(first), second(second) { }

        /**
         * \brief Copy Constructor
         */
        Pair(Pair<T1, T2>& other) : first(other.first), second(other.second) { }

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
            std::swap(first, other.first);
            std::swap(second, other.second);
        }
     
        /**
         * \brief Spaceship Operator
         */
        auto operator<=>(const Pair& p2) const = default;

        Pair(Pair<T1, T2>&& other) = delete;
        Pair& operator=(const Pair<T1, T2>&& other) = delete;
        ~Pair() = default;
    };
}
