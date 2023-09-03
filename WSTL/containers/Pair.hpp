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
        Pair(T1 first = T1(), T2 second = T2()) : first(first), second(second) { }

        /**
         * \brief Copy Constructor
         */
        Pair(Pair<T1, T2>& other) : first(other.first), second(other.second) { }

        /**
         * \brief Copy Assignment Operator
         */
        Pair& operator=(const Pair<T1, T2>& other)
        {
            if(*this == other) return *this;
        
            first = other.first;
            second = other.second;
            
            return *this;
        }

        /**
         * \brief Move Constructor
         */
        Pair(Pair<T1, T2>&& other) noexcept
        {
            first = std::move(other.first);
            second = std::move(other.second);
        }

        /**
         * \brief Move Assignment Operator
         */
        Pair& operator=(Pair<T1, T2>&& other) noexcept
        {
            if(*this == other) return *this;

            first = std::move(other.first);
            second = std::move(other.second);
            
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
        
        ~Pair() = default;
    };
}
