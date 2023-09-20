#pragma once

namespace WSTL
{
    template<typename T>
    struct Optional
    {
    public:
        Optional() : hasValue(false) {}
        Optional(const T& value) : hasValue(true), value(value) {}
        Optional(const Optional& other) : hasValue(other.hasValue), value(other.value) {}
        Optional(Optional&& other) noexcept : hasValue(other.hasValue), value(other.value) {}

        Optional& operator=(const Optional& other)
        {
            if(this == &other) return *this;

            hasValue = other.hasValue;
            value = other.value;

            return *this;
        }

        Optional& operator=(Optional&& other) noexcept
        {
            if(this == &other) return *this;

            hasValue = other.hasValue;
            value = other.value;
            other.hasValue = false;
            other.value = T();

            return *this;
        }

        ~Optional() = default;

        bool HasValue() const
        {
            return hasValue;
        }

        T& Value()
        {
            return value;
        }
        
        const T& Value() const
        {
            return value;
        }

        T& operator*()
        {
            return value;
        }
        
        const T& operator*() const
        {
            return value;
        }

        T* operator->()
        {
            return &value;
        }
        
        const T* operator->() const
        {
            return &value;
        }

        void Reset()
        {
            hasValue = false;
            value = T();
        }

        void Reset(const T& value)
        {
            hasValue = true;
            this->value = value;
        }

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