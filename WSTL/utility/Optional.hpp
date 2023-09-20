#pragma once

namespace WSTL
{
    template<typename T>
    struct Optional
    {
    public:
        /**
         * \brief Default constructor
         */
        Optional() : hasValue(false) {}
        
        /**
         * \brief Constructor from value
         */
        Optional(const T& value) : hasValue(true), value(value) {}

        /**
         * \brief Copy constructor
         */
        Optional(const Optional& other) : hasValue(other.hasValue), value(other.value) {}

        /**
         * \brief Move constructor
         */
        Optional(Optional&& other) noexcept : hasValue(other.hasValue), value(other.value) {}

        /**
         * \brief Copy assignment operator
         */
        Optional& operator=(const Optional& other)
        {
            if(this == &other) return *this;

            hasValue = other.hasValue;
            value = other.value;

            return *this;
        }

        /**
         * \brief Move assignment operator
         */
        Optional& operator=(Optional&& other) noexcept
        {
            if(this == &other) return *this;

            hasValue = other.hasValue;
            value = other.value;
            other.hasValue = false;
            other.value = T();

            return *this;
        }

        /**
         * \brief Destructor
         */
        ~Optional() = default;

        /**
         * \brief Checks if the optional has a value
         */
        bool HasValue() const
        {
            return hasValue;
        }

        /**
         * \brief Gets the value
         */
        T& Value()
        {
            return value;
        }

        /**
         * \brief Gets the value as const
         */
        const T& Value() const
        {
            return value;
        }

        /**
         * \brief Gets the reference to the value
         */
        T& operator*()
        {
            return value;
        }

        /**
         * \brief Gets the reference to the value as const
         */
        const T& operator*() const
        {
            return value;
        }

        /**
         * \brief Gets the pointer to the value
         */
        T* operator->()
        {
            return &value;
        }

        /**
         * \brief Gets the pointer to the value as const
         */        
        const T* operator->() const
        {
            return &value;
        }

        /**
         * \brief Resets the optional
         */
        void Reset()
        {
            hasValue = false;
            value = T();
        }

        /**
         * \brief Resets the optional with a value
         */
        void Reset(const T& value)
        {
            hasValue = true;
            this->value = value;
        }

        /**
         * \brief Resets the optional with a value
         */
        void Reset(T&& value)
        {
            hasValue = true;
            this->value = value;
        }
        
    private:
        bool hasValue;
        T value;            
    };
}