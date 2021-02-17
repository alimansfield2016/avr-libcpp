#pragma once

#include <stdint.h>

#include <type_traits>

namespace AVR
{
	template<typename Width>
	class Register{
		public:
			using T = Width;
			using VT = volatile T;
			explicit constexpr Register(uintptr_t addr) : m_addr(addr){}

			constexpr VT& reg() const { return *reinterpret_cast<VT*>(m_addr); }

			operator VT&() const { return reg(); }
			T operator = (T other) const { return reg() = other; }
			constexpr uintptr_t operator & () const { return m_addr; }
			constexpr uintptr_t addr() const { return m_addr; }
			constexpr uintptr_t io_addr() const { return m_addr-0x20; }
		protected:
		private:
		const uintptr_t m_addr;
	};
	using Register8 = Register<uint8_t>;
	using Register16 = Register<uint16_t>;

	template<typename Width> 
	class Bitmask
	{
		public:
		using T = Width;
		template<class U, class ...Rest> requires(std::is_integral_v<U> && (std::is_integral_v<Rest> && ... ))
		explicit constexpr Bitmask(U bit, Rest...r) : m_mask{Bitmask{bit} | Bitmask{r...}} {}
		explicit constexpr Bitmask(int bit) : m_mask{static_cast<T>(1<<bit)} {}
		
		//remove implicit copy
		Bitmask(Bitmask&) = default;
		Bitmask(const Bitmask&) = default;

		constexpr operator T() const { return m_mask; }

		constexpr Bitmask operator | (const Bitmask &other) const { return Bitmask(m_mask|other.m_mask, S{}); }
		constexpr Bitmask operator & (const Bitmask &other) const { return Bitmask(m_mask&other.m_mask, S{}); }

		private:
		//private struct S to force private constructor
		struct S{};
		constexpr Bitmask(unsigned long value, S) : m_mask(value){};
		const T m_mask;

	};

	using BV = Bitmask<uint8_t>;
	using BV8 = BV;
	using BV16 = Bitmask<uint16_t>;
} // namespace AVR
