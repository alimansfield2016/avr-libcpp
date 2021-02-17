#pragma once

// #include <avr/pgmspace.h>
#include <cstddef>
#include <type_traits>
#include <initializer_list>

#define PROGMEM __attribute__((__progmem__))

#define eeprom_read_byte(addr)   \
(__extension__({                \
    uint16_t __addr16 = (uint16_t)(addr); \
    uint8_t __result;           \
    __asm__ __volatile__        \
    (                           \
        "lpm" "\n\t"            \
        "mov %0, r0" "\n\t"     \
        : "=r" (__result)       \
        : "z" (__addr16)        \
        : "r0"                  \
    );                          \
    __result;                   \
}))

namespace AVR
{
	template<class T>
	class eeprom_ptr final {
	public:
		using type = T;
		using value_type = std::remove_const_t<type>;
		using const_type = const value_type;
		using pointer = const_type*;
		using size_type = int;

	private:
		pointer m_ptr;
	public:
		constexpr eeprom_ptr(const type *ptr) : m_ptr{ptr}
		{}
		constexpr eeprom_ptr(const eeprom_ptr&) = default;

		// operator &
		// operator ++
		// operator ++(int)
		// operator --
		// operator --(int)
		// operator +
		// operator -
		// operator +=
		// operator -=
		// operator []
		// operator *
		// operator ==
		// operator !=

		constexpr eeprom_ptr &assign(eeprom_ptr _ptr) { m_ptr = _ptr.get(); return *this; }
		// constexpr operator pointer() const { return m_ptr; }
		constexpr operator bool() const { return !!m_ptr; }

		constexpr pointer ptr() const { return m_ptr; }
		constexpr pointer get() const { return ptr(); }
		constexpr pointer operator&() const { return ptr(); }
		constexpr pointer operator->() const { return ptr(); }

		constexpr eeprom_ptr &operator++() { m_ptr++; return *this; }
		constexpr eeprom_ptr operator++(int) { return m_ptr++; }
		constexpr eeprom_ptr &operator--() { m_ptr--; return *this; }
		constexpr eeprom_ptr operator--(int) { return m_ptr--; }

		constexpr eeprom_ptr operator+(size_type s) const { return m_ptr+s; }
		constexpr eeprom_ptr operator-(size_type s) const { return m_ptr-s; }
		constexpr eeprom_ptr operator+=(size_type s) { m_ptr+=s; return *this; }
		constexpr eeprom_ptr operator-=(size_type s) { m_ptr-=s; return *this; }

		constexpr value_type operator[](size_type idx) const { return *(*this + idx); }
		constexpr value_type operator*() const 
		{
			constexpr size_t size = sizeof(value_type);
			uint8_t buf[size];
			
			for(uint8_t i = 0; i < size; i++){
				buf[i] = eeprom_read_byte(reinterpret_cast<const uint8_t*>(m_ptr)+i);
			}

			return *static_cast<value_type*>(static_cast<void*>(buf));
		}

		template<class U>
		constexpr bool operator==(eeprom_ptr<U> other) const { return ptr() == other.ptr(); }
		template<class U>
		constexpr bool operator!=(eeprom_ptr<U> other) const { return ptr() != other.ptr(); }

	};

} // namespace AVR