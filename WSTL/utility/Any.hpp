#pragma once
#include "WSTL/memory/Memory.hpp"

namespace WSTL
{
    class Any
    {
    private:
        struct Base
        {
            Base() = default;
            Base(const Base&) = default;
            Base(Base&&) = default;
            Base& operator=(const Base&) = default;
            Base& operator=(Base&&) = default;
            virtual ~Base() = default;
            virtual Base* Clone() const = 0;
        };

        template<typename T>
        struct Derived : public Base
        {
            T data;

            Derived(const T& data) : data(data) {}

            Base* Clone() const override
            {
                return new Derived<T>(data);
            }
        };
    
    public:
        /**
         * \brief Default constructor
         */
        Any() : data(nullptr) {}

        /**
         * \brief Constructor from nullptr
         */
        Any(decltype(nullptr)) : data(nullptr) {}

        /**
         * \brief Move constructor
         */
        Any(Any&& other) noexcept
        {
            data = other.data->Clone();
            other.data = nullptr;
        }

        /**
         * \brief Constructor from any type
         */
        template<typename T>
        Any(const T& data)
        {
            this->data = new Derived<T>(data);
        }

        /**
         * \brief Copy constructor
         */
        Any(const Any& other)
        {
            data = other.data ? other.data->Clone() : nullptr;
        }

        /**
         * \brief Copy assignment operator
         */
        Any& operator=(const Any& other)
        {
            if(this == &other) return *this;

            delete data;
            data = other.data ? other.data->Clone() : nullptr;
        
            return *this;
        }

        /**
         * \brief Move assignment operator
         */
        Any& operator=(Any&& other) noexcept
        {
            if(this == &other) return *this;

            data = other.data;
            Free(&other.data);
            
            return *this;
        }

        /**
         * \brief Destructor
         */
        ~Any()
        {
            Free(&data);
        }

        /**
         * \brief Get the data as a specific type
         */
        template<typename T>
        T& Cast()
        {
            auto* derived = dynamic_cast<Derived<T>*>(data);
            if(derived == nullptr) throw std::bad_cast();
            return derived->data;
        }

        /**
         * \brief Get the data as a specific type as const
         */
        template<typename T>
        const T& Cast() const
        {
            auto* derived = dynamic_cast<Derived<T>*>(data);
            if(derived == nullptr) throw std::bad_cast();
            return derived->data;
        }
        
        /**
         * \brief Returns whether the data is empty
         */
        bool IsEmpty() const
        {
            return data == nullptr;
        }
    
    private:
        Base* data;
    };
}