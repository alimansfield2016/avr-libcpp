#pragma once

// #include <avr/pgmspace.h>
#include <cstddef>
#include <type_traits>
#include <initializer_list>

#define PROGMEM __attribute__((__progmem__))

#define __pgm_read_byte(addr)   \
(__extension__({                \
    uint16_t __addr16 = (uint16_t)(addr); \
    uint8_t __result;           \
    __asm__ __volatile__        \
    (                           \
        "lpm %0, Z	\n\t"       \
        : "=r" (__result)       \
        : "z" (__addr16)        \
    );                          \
    __result;                   \
}))

#define __pgm_read_word(addr)         \
(__extension__({                            \
    uint16_t __addr16 = (uint16_t)(addr);   \
    uint16_t __result;                      \
    __asm__ __volatile__                    \
    (                                       \
        "lpm %A0, Z+"   "\n\t"              \
        "lpm %B0, Z"    "\n\t"              \
        : "=r" (__result)					\
		: "z" (__addr16)  					\
    );                                      \
    __result;                               \
}))


#define __pgm_read_data(addr, buf, len)		\
(__extension__({							\
	uint16_t __addr16 = (uint16_t)(addr);	\
	uint16_t __buf16 = (uint16_t)(buf);		\
	uint8_t __len8 = (uint8_t)(len);		\
	uint8_t __data;							\
	__asm__ __volatile__					\
	(										\
		"L_dl1%=:		\n\t"				\
		"lpm %0, Z+		\n\t"				\
		"st %a1+, %0	\n\t"				\
		"dec %2			\n\t"				\
		"brne L_dl1%=	\n\t"				\
		: 	"=r" (__data), 					\
			"=e" (__buf16),					\
			"=r" (__len8)					\
		:	"z" (__addr16),					\
			"1" (__buf16),					\
			"2" (__len8)					\
	);										\
}))											\

#define P(s) AVR::pgm_string((__extension__({static const char __c[] PROGMEM = (s); &__c[0];})))

namespace AVR
{ 

	inline uint8_t pgm_read_byte(const uint8_t *addr)
	{
		return __pgm_read_byte(addr);
	}
	inline uint16_t pgm_read_word(const uint16_t *addr)
	{
		return __pgm_read_word(addr);
	}
	inline void pgm_read(const void *addr, void *buf, std::size_t s)
	{
		if(s)
			__pgm_read_data(addr, buf, s);
	}

	template<std::size_t s> 
	inline void pgm_read(const uint8_t *addr, void *buf)
	{
		for(std::size_t i = 0; i < s; i++)
			static_cast<uint8_t*>(buf)[i] = pgm_read_byte(addr+i);
	}

	template<>
	inline void pgm_read<0>(const uint8_t*, void*){}

	template<>
	inline void pgm_read<2>(const uint8_t *addr, void *buf)
	{
		static_cast<uint16_t*>(buf)[0] = pgm_read_word(reinterpret_cast<const uint16_t*>(addr));
	}

	template<class T>
	class pgm_ptr final {
	public:
		using type = T;
		using value_type = std::remove_const_t<type>;
		using const_type = const value_type;
		using pointer = const_type*;
		using size_type = int;

		constexpr explicit pgm_ptr(const type *ptr) : m_ptr{ptr}
		{}
		constexpr pgm_ptr(const pgm_ptr&) = default;

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

		constexpr pgm_ptr &assign(pgm_ptr _ptr) { m_ptr = _ptr.get(); return *this; }
		constexpr pgm_ptr &assign(const type  *_ptr) { m_ptr = _ptr; return *this; }
		constexpr pgm_ptr &operator=(const type  *_ptr) { m_ptr = _ptr; return *this; }
		constexpr operator bool() const { return !!m_ptr; }

		constexpr pointer ptr() const { return m_ptr; }
		constexpr pointer get() const { return ptr(); }
		constexpr pointer operator&() const { return ptr(); }
		constexpr pointer operator->() const { return ptr(); }

		constexpr pgm_ptr &operator++() { m_ptr++; return *this; }
		constexpr pgm_ptr operator++(int) { return pgm_ptr{m_ptr++}; }
		constexpr pgm_ptr &operator--() { m_ptr--; return *this; }
		constexpr pgm_ptr operator--(int) { return pgm_ptr{m_ptr--}; }

		constexpr pgm_ptr operator+(size_type s) const { return pgm_ptr{m_ptr+s}; }
		constexpr pgm_ptr operator-(size_type s) const { return pgm_ptr{m_ptr-s}; }
		constexpr pgm_ptr operator+=(size_type s) { m_ptr+=s; return *this; }
		constexpr pgm_ptr operator-=(size_type s) { m_ptr-=s; return *this; }

		constexpr value_type operator[](size_type idx) const { return *(*this + idx); }
		constexpr value_type operator*() const 
		{
			constexpr size_t size = sizeof(value_type);
			uint8_t buf[size];
			pgm_read<size>(reinterpret_cast<const uint8_t*>(m_ptr), buf);
			
			return *static_cast<value_type*>(static_cast<void*>(buf));
		}

		template<class U>
		constexpr bool operator==(pgm_ptr<U> other) const { return ptr() == other.ptr(); }
		template<class U>
		constexpr bool operator!=(pgm_ptr<U> other) const { return ptr() != other.ptr(); }

	private:
		pointer m_ptr;
	};

	using pgm_string = pgm_ptr<char>;

	template<class T, std::size_t S>
	struct pgm_array_traits{
		using value_type = std::remove_cv_t<T>;
		typedef T _type[S];
		static constexpr pgm_ptr<value_type>
		S_ptr(const _type& t, std::size_t n) noexcept
		{ return pgm_ptr<T>{t + n};}
	};

	template<class T>
	struct pgm_array_traits<T, 0>{
		using value_type = std::remove_cv_t<T>;
		struct _type{};
		static constexpr pgm_ptr<value_type>
		S_ptr(const _type&, std::size_t) noexcept
		{ return pgm_ptr<T>{nullptr};}
	};

	/**
	 * @brief pgm_array is a PROGMEM type that stores a statically-sized array of type T
	 * 
	 * @tparam T type to store in array
	 * @tparam S size of array
	 */
	template<class T, std::size_t S>
	struct pgm_array{
	public:
		using type = T;
		using value_type = std::remove_const_t<type>;
		using const_type = const value_type;
		using pointer = pgm_ptr<value_type>;
		using size_type = int;
		// using traits = pgm_array_traits<pgm_array<T, S>;
		typedef pgm_array_traits<T, S> traits;
		static constexpr size_type s_size = S;
		// value_type m_arr[s_size];
		typename traits::_type m_elems;

		/**
		 * @brief Construct a new pgm array object
		 * 
		 * @param init initializer_list to initialize array with
		 */
		// constexpr pgm_array(const std::initializer_list<T> &init)
		// {
		// 	auto it = init.begin();
		// 	for(uint8_t i = 0; i < s_size && it != init.end(); i++)
		// 		m_arr[i] = *it++;
		// }

		constexpr size_type size() const { return s_size; }
		constexpr size_type count() const { return s_size; }
		constexpr pointer begin() const { return traits::S_ptr(m_elems, 0); }
		// constexpr pointer begin() const { return m_arr; }
		constexpr pointer end() const { return traits::S_ptr(m_elems, s_size); }
		// constexpr pointer end() const { return m_arr + s_size; }
		constexpr value_type operator[](size_type idx) const { return begin()[idx]; }

		// Template size deduction
		// template<typename T, typename... U>
		// pgm_array(T, U...) -> 
		// 	pgm_array<	std::enable_if_t<(std::is_same_v<T, U> && ...), T>,
		// 				1 + sizeof...(U)>;
	};

	// class ::AVR::USB::Configuration;
	/**
	 * @brief pgm_span holds a dynamic-size reference to pgm_array
	 * 
	 * @tparam T 
	 */
	template<class T>
	class pgm_span{
		// friend class AVR::USB::Configuration;
	public:
		using type = T;
		using value_type = std::remove_const_t<type>;
		using const_type = const value_type;
		using pointer = pgm_ptr<value_type>;
		using size_type = int;

		constexpr pgm_span() : 
			m_count{0},
			m_ptr{nullptr}
		{}

		constexpr pgm_span(pgm_ptr<T> ptr, size_type size) : 
			m_count{size},
			m_ptr{ptr}
		{}
			
		template<std::size_t S>
		constexpr pgm_span(const pgm_array<T, S> &arr) : 
			m_count{arr.size()}, 
			m_ptr{arr.begin()}
		{}
		constexpr pgm_span(const pgm_span &span) : 
			m_count{span.count()},
			m_ptr{span.begin()}
		{}
		constexpr pointer ptr() const { return m_ptr; }
		constexpr pointer* ptr_p() const { return &m_ptr; }
		constexpr pgm_ptr<pointer> ptr_pgm() const { return pgm_ptr<pointer>{&m_ptr}; }
		constexpr size_type size() const { return m_count; }
		constexpr size_type* size_p() const { return &m_count; }
		constexpr pgm_ptr<size_type> size_pgm() const { return pgm_ptr<size_type>{&m_count}; }
		constexpr size_type count() const { return m_count; }
		constexpr size_type* count_p() const { return &m_count; }
		constexpr pgm_ptr<size_type> count_pgm() const { return pgm_ptr<size_type>{&m_count}; }
		constexpr pointer begin() const { return m_ptr; }
		constexpr pointer end() const { return m_ptr + m_count; }
		constexpr value_type operator[](size_type idx) const { return m_ptr[idx]; }
	private:
		const size_type m_count;
		const pgm_ptr<value_type> m_ptr;
	};

} // namespace AVR