#pragma once

#include "stdexcept"
#include "WSTL/Defines.hpp"
#include "WSTL/Types.hpp"
#include "WSTL/utility/TypeTraits.hpp"

namespace WSTL
{
    template<::Size BitAmount>
    class BitSet
    {
    public:
        template<class StringElement, class StringTraits = std::char_traits<StringElement>, class StringAlloc = std::allocator<StringElement>>
        using String = std::basic_string<StringElement, StringTraits, StringAlloc>;
        typedef ConditionalT<BitAmount <= sizeof(unsigned long) * CHAR_BIT, unsigned long, unsigned long long> ArrayType;

        class BitReference
        {
            friend BitSet<BitAmount>;

        private:
            constexpr BitReference() noexcept : pBitSet(nullptr), pos(0) {}

            constexpr BitReference(BitSet<BitAmount>& bitset, const ::Size pos) noexcept : pBitSet(&bitset), pos(pos) {}
            
        public:
            constexpr BitReference(const BitReference& other) noexcept : pBitSet(other.pBitSet), pos(other.pos) {}
            constexpr BitReference(BitReference&& other) = delete;
            
            constexpr BitReference& operator=(const bool val) noexcept
            {
                pBitSet->SetUnchecked(pos, val);
                return *this;
            }

            constexpr BitReference& operator=(const BitReference& other) noexcept
            {
                if(this == &other) return *this;
                *this = static_cast<bool>(other);
                return *this;
            }
            constexpr BitReference& operator=(BitReference&& other) = delete;

            [[nodiscard]]
            constexpr bool operator~() const noexcept
            {
                return !pBitSet->Subscript(pos);
            }
            
            constexpr operator bool() const noexcept
            {
                return pBitSet->Subscript(pos);
            }
            
            constexpr ~BitReference() noexcept = default;

            constexpr BitReference& flip() noexcept
            {
                pBitSet->FlipUnchecked(pos);
                return *this;
            }

        private:
            BitSet<BitAmount>* pBitSet;
            ::Size pos;
        };
        
        static constexpr bool NeedsMask = BitAmount < CHAR_BIT * sizeof(unsigned long long);
        static constexpr unsigned long long Mask = (1ULL << (NeedsMask ? BitAmount : 0)) - 1ULL;

        /**
         * @brief Default Constructor
         */
        constexpr BitSet() noexcept : bits() {}

        /**
         * @brief Default Value Constructor
         */
        constexpr BitSet(unsigned long long val) noexcept : bits{static_cast<ArrayType>(NeedsMask ? val & Mask : val)} {}

        /**
         * @brief Constructor that generates a bitset from a string
         */
        template<class Traits, class Elem>
        constexpr void Construct(const Elem* const ptr, ::Size count, const Elem zero, const Elem one)
        {
            if(count > BitAmount)
            {
                for(::Size i = BitAmount; i < count; i++)
                {
                    const var c = ptr[i];
                    if(!Traits::eq(one, c) && !Traits::eq(zero, c)) throw std::invalid_argument("Invalid Bitset Character");
                }
                
                count = BitAmount;
            }

            ::Size wordPos = 0;
            if(count != 0)
            {
                ::Size bitsInWord = 0;
                var last = ptr + count;
                ArrayType thisWord = 0;

                do
                {
                    last--;
                    const var c = *last;
                    thisWord |= static_cast<ArrayType>(Traits::eq(one, c)) << bitsInWord;
                    if(!Traits::eq(one, c) && !Traits::eq(zero, c)) throw std::invalid_argument("Invalid Bitset Character");

                    if(++bitsInWord == BitsPerWord)
                    {
                        bits[wordPos] = thisWord;
                        wordPos++;
                        thisWord = 0;
                        bitsInWord = 0;
                    }
                } while(ptr != last);
                
                if(bitsInWord != 0)
                {
                    bits[wordPos] = thisWord;
                    wordPos++;
                }
            }

            for(; wordPos <= Words; wordPos++)
            {
                bits[wordPos] = 0;
            }
        }

        /**
         * @brief Default Constructor that generates a bitset from a string
         */
        template<class Elem, class Traits, class Alloc>
        constexpr explicit BitSet(const String<Elem, Traits, Alloc>& str,
            typename String<Elem, Traits, Alloc>::size_type pos = 0,
            typename String<Elem, Traits, Alloc>::size_type count = String<Elem, Traits, Alloc>::npos,
            Elem zero = static_cast<Elem>('0'),
            Elem one = static_cast<Elem>('1'))
        {
            Validate(str.size());
            
            if(str.size() - pos < count) count = str.size() - pos;

            Construct<Traits>(str.data() + pos, count, zero, one);
        }

        /**
         * @brief Default Constructor that generates a bitset from a string
         */
        template <class Elem>
        constexpr explicit BitSet(const Elem* str,
            typename String<Elem>::size_type count = String<Elem>::npos,
            Elem zero = static_cast<Elem>('0'),
            Elem one = static_cast<Elem>('1'))
        {
            if(count == String<Elem>::npos) count = std::char_traits<Elem>::length(str);

            Construct<std::char_traits<Elem>>(str, count, zero, one);
        }

        /**
         * @brief Returns if the bit at the specified position is Set (1)
         */
        [[nodiscard]]
        constexpr bool operator[](::Size pos) const
        {
            return pos > BitAmount ? (Validate(pos), false) : Subscript(pos);
        }

        /**
         * @brief Returns a bit reference to the specified position
         */
        [[nodiscard]]
        constexpr BitReference operator[](const ::Size pos) noexcept
        {
            Validate(pos);
            return BitReference(*this, pos);
        }

        /**
         * @brief Runs a bitwise AND operation on the two BitSets
         */
        constexpr BitSet& operator &=(const BitSet& other) noexcept
        {
            for(::Size i = 0; i <= Words; i++) bits[i] &= other.bits[i];
            return *this;
        }

        /**
         * @brief Runs a bitwise OR operation on the two BitSets
         */
        constexpr BitSet& operator |=(const BitSet& other) noexcept
        {
            for(::Size i = 0; i <= Words; i++) bits[i] |= other.bits[i];
            return *this;
        }

        /**
         * @brief Runs a bitwise XOR operation on the two BitSets
         */
        constexpr BitSet& operator ^=(const BitSet& other) noexcept
        {
            for(::Size i = 0; i <= Words; i++) bits[i] ^= other.bits[i];
            return *this;
        }

        /**
         * @brief Shifts the BitSet to the left by the specified amount
         */
        constexpr BitSet& operator<<=(::Size shift) noexcept
        {
            const var wordShift = static_cast<PtrDiff>(shift / BitsPerWord);
            if(wordShift != 0)
            {
                for(var i = Words; 0 <= i; i--)
                {
                    bits[i] = wordShift <= i ? bits[i - wordShift] : 0;
                }
            }

            if((shift %= BitsPerWord) != 0)
            {
                for(var i = Words; 0 < i; i--)
                {
                    bits[i] = (bits[i] << shift) | bits[i - 1] >> (BitsPerWord - shift);
                }
                
                bits[0] <<= shift;
            }
            
            Trim();
            return *this;
        }

        /**
         * @brief Shifts the BitSet to the right by the specified amount
         */
        constexpr BitSet& operator>>=(::Size shift) noexcept
        {
            const var wordShift = static_cast<PtrDiff>(shift / BitsPerWord);
            if(wordShift != 0)
            {
                for(PtrDiff i = 0; i <= Words; i++)
                {
                    bits[i] = wordShift <= Words - i ? bits[i + wordShift] : 0;
                }
            }

            if((shift %= BitsPerWord) != 0)
            {
                for(PtrDiff i = 0; i < Words; i++)
                {
                    bits[i] = (bits[i] >> shift) | bits[i + 1] << (BitsPerWord - shift);
                }
                
                bits[Words] >>= shift;
            }
            
            return *this;
        }

        [[nodiscard]]
        constexpr BitSet operator~() const noexcept
        {
            var tmp = *this;
            tmp.Flip();
            return tmp;
        }

        [[nodiscard]]
        constexpr BitSet operator&(const BitSet& other) noexcept
        {
            var tmp = *this;
            tmp &= other;
            return tmp;
        }

        [[nodiscard]]
        constexpr BitSet operator|(const BitSet& other) const
        {
            var tmp = *this;
            tmp |= other;
            return tmp;
        }

        [[nodiscard]]
        constexpr BitSet operator^(const BitSet& other) const
        {
            var tmp = *this;
            tmp ^= other;
            return tmp;
        }
        
        [[nodiscard]]
        constexpr BitSet operator<<(const ::Size shift) const noexcept
        {
            var tmp = *this;
            tmp <<= shift;
            return tmp;
        }

        [[nodiscard]]
        constexpr BitSet operator>>(const ::Size shift) const noexcept
        {
            var tmp = *this;
            tmp >>= shift;
            return tmp;
        }
        
        [[nodiscard]]
        constexpr bool operator==(const BitSet& other) const noexcept
        {
            if (__builtin_is_constant_evaluated())
            {
                for(::Size i = 0; i < Words; i++)
                {
                    if(bits[i] != other.bits[i]) return false;
                }
                return true;
            }
            else
            {
                return memcmp(&bits[0], &other.bits[0], sizeof(bits)) == 0;
            }
        }

        /**
         * @brief Sets all bits to 1
         */
        constexpr BitSet& Set() noexcept
        {
            if (__builtin_is_constant_evaluated())
            {
                for(var& bit : bits) bit = static_cast<ArrayType>(-1);
            }
            else
            {
                memset(&bits, 0xFF, sizeof(bits));
            }
            
            Trim();
            return *this;
        }

        /**
         * @brief Sets the bit at the specified position to the specified value
         */
        constexpr BitSet& Set(const ::Size pos, bool val = true)
        {
            Validate(pos);
            return SetUnchecked(pos, val);
        }

        /**
         * @brief Sets all bits to 0
         */
        constexpr BitSet& Reset() noexcept
        {
            if (__builtin_is_constant_evaluated())
            {
                for(var& bit : bits) bit = 0;
            }
            else
            {
                memset(&bits, 0, sizeof(bits));
            }
            return *this;
        }

        /**
         * @brief Set bit at the specified position to 0
         */
        constexpr BitSet& Reset(const ::Size pos) noexcept
        {
            return Set(pos, false);
        }

        /**
         * @brief Flips all bits
         */
        constexpr BitSet& Flip() noexcept
        {
            for(::Size i = 0; i <= Words; i++) bits[i] = ~bits[i];

            Trim();
            return *this;
        }

        /**
         * @brief Flips the bit at the specified position
         */
        constexpr BitSet& Flip(const ::Size pos) noexcept
        {
            Validate(pos);
            return FlipUnchecked(pos);
        }

        /**
         * @brief Returns true if at least one bit is Set (1)
         */
        [[nodiscard]]
        constexpr bool Any() noexcept
        {
            for(::Size i = 0; i <= Words; i++)
            {
                if(bits[i] != 0) return true;
            }

            return false;
        }

        /**
         * @brief Returns true if no bits are Set (1)
         */
        [[nodiscard]]
        constexpr bool None() noexcept
        {
            return !Any();
        }

        /**
         * @brief Returns true if all bits are Set (1)
         */
        [[nodiscard]]
        constexpr bool All() noexcept
        {
            if(BitAmount == 0) return true;

            constexpr bool noPadding = BitAmount % BitsPerWord == 0;
            for(::Size i = 0; i < Words + noPadding; i++)
            {
                if(bits[i] != ~static_cast<ArrayType>(0)) return false;
            }
            
            return noPadding || bits[Words] == (ArrayType{1} << (BitAmount % BitsPerWord)) - 1;
        }

        /**
         * @brief Returns the number of bits that are Set (1)
         */
        [[nodiscard]]
        constexpr ::Size Count() noexcept
        {
            ::Size count = 0;
            for(::Size i = 0; i <= Words; i++)
            {
                for(::Size j = 0; j < BitsPerWord; j++)
                {
                    count += (bits[i] >> j) & 1;
                }
            }
            return count;
        }

        /**
         * @brief Returns the size of the BitSet
         */
        [[nodiscard]]
        static constexpr ::Size Size() noexcept
        {
            return BitAmount;
        }
        
        /**
         * @brief Validates the index. Throws std::out_of_range if index is out of range
         */
        static constexpr void Validate(const ::Size pos)
        {
            if(pos >= BitAmount)
                throw std::out_of_range("BitSet<BitAmount>::Validate: Index out of range");
        }

        /**
         * @brief Checks if the bit at the specified position is Set (1)
         */
        constexpr bool Subscript(::Size pos) const
        {
            return (bits[pos / BitsPerWord] & (ArrayType{1} << (pos % BitsPerWord))) != 0;
        }

        /**
         * @brief Returns the word at the specified position
         */
        [[nodiscard]]
        constexpr ArrayType GetWord(::Size pos) const
        {
            return bits[pos];
        }

        [[nodiscard]]
        constexpr UL ToULong() const noexcept(BitAmount < 32)
        {
            if constexpr (BitAmount == 0)
            {
                return 0;
            }
            else if constexpr (BitAmount < 32)
            {
                return static_cast<unsigned long>(bits[0]);
            }
            else
            {
                if constexpr (BitAmount > 32)
                {
                    for(::Size i = 1; i <= Words; i++)
                    {
                        if(bits[i] != 0) throw std::overflow_error("BitSet<BitAmount>::ToULong: Overflow");
                    }
                }

                if(bits[0] > ULONG_MAX) throw std::overflow_error("BitSet<BitAmount>::ToULong: Overflow");

                return static_cast<unsigned long>(bits[0]);
            }
        }

        [[nodiscard]]
        constexpr ULL ToULLong() const noexcept(BitAmount < 64)
        {
            if constexpr(BitAmount == 0) return 0;
            else
            {
                for(::Size i = 1; i <= Words; i++)
                {
                    if(bits[i] != 0) throw std::overflow_error("BitSet<BitAmount>::ToULLong: Overflow");
                }
            }
            
            return bits[0];
        }
        
        /**
         * @brief Returns the string representation of the BitSet
         */
        [[nodiscard]]
        std::string ToString() const
        {
            std::string str;
            str.reserve(BitAmount);
            for(::Size i = 0; i < BitAmount; i++)
            {
                str.push_back(Subscript(i) ? '1' : '0');
            }
            return str;
        }
        
    private:
        static constexpr PtrDiff BitsPerWord = CHAR_BIT * sizeof(ArrayType);
        static constexpr PtrDiff Words = BitAmount == 0 ? 0 : (BitAmount - 1) / BitsPerWord;

        constexpr void Trim() noexcept
        {
            if(BitAmount != 0 && BitAmount % BitsPerWord == 0) return;
            bits[Words] &= (ArrayType{1} << (BitAmount % BitsPerWord)) - 1;
        }
        
        constexpr BitSet& SetUnchecked(const ::Size pos, const bool val) noexcept
        {
            var& SelectedWord = bits[pos / BitsPerWord];
            const var bit = ArrayType{1} << (pos % BitsPerWord);
            
            if(val) SelectedWord |= bit;
            else SelectedWord &= ~bit;
            
            return *this;
        }

        constexpr BitSet& FlipUnchecked(const ::Size pos) noexcept
        {
            bits[pos / BitsPerWord] ^= ArrayType{1} << (pos % BitsPerWord);
            return *this;
        }
        
        ArrayType bits[Words + 1];
    };
}