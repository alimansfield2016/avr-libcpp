#pragma once

/*
AVR C++ Standard Library
Written by Alex Mansfield
Copyright 2020

Partial ordering Header

The <partial_ordering.hpp> header includes:

*/

namespace std
{
	
	enum class ord{less = 0, greater = 3};
	enum class eq{equal = 1, equivalent = 2};
	enum class ncmp{unordered = 4};

	class partial_ordering
	{
		int value;
		bool is_ordered;

		constexpr explicit partial_ordering(eq v) noexcept : value(int(v)), is_ordered(true) {}
		constexpr explicit partial_ordering(ord v) noexcept : value(int(v)), is_ordered(true) {}
		constexpr explicit partial_ordering(ncmp v) noexcept : value(int(v)), is_ordered(false) {}

	public:
		//valid values
		static const partial_ordering less;
		static const partial_ordering equivalent;
		static const partial_ordering greater;
		static const partial_ordering unordered;

		//comparisons
		friend constexpr bool operator==(partial_ordering v, int) noexcept { return v.value == int(eq::equivalent); }
		friend constexpr bool operator==(partial_ordering v, partial_ordering w) noexcept = default;
		friend constexpr bool operator< (partial_ordering v, int) noexcept { return v.value == int(ord::less); };
		friend constexpr bool operator> (partial_ordering v, int) noexcept { return v.value == int(ord::greater); };
		friend constexpr bool operator<=(partial_ordering v, int) noexcept { return v.value != int(ord::greater); };
		friend constexpr bool operator>=(partial_ordering v, int) noexcept { return v.value != int(ord::less); };
		friend constexpr bool operator< (int, partial_ordering v) noexcept { return v.value != int(ord::less); };
		friend constexpr bool operator> (int, partial_ordering v) noexcept { return v.value != int(ord::greater); };
		friend constexpr bool operator<=(int, partial_ordering v) noexcept { return v.value == int(ord::greater); };
		friend constexpr bool operator>=(int, partial_ordering v) noexcept { return v.value == int(ord::less); };

		friend constexpr partial_ordering operator<=>(partial_ordering v, int) noexcept;
		friend constexpr partial_ordering operator<=>(int, partial_ordering v) noexcept;

	};

	inline constexpr partial_ordering partial_ordering::less(ord::less);
	inline constexpr partial_ordering partial_ordering::equivalent(eq::equivalent);
	inline constexpr partial_ordering partial_ordering::greater(ord::greater);
	inline constexpr partial_ordering partial_ordering::unordered(ncmp::unordered);
} // namespace std
