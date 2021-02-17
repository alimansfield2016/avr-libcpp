#pragma once

/*
AVR C++ Standard Library
Written by Alex Mansfield
Copyright 2020

Weak ordering Header

The <weak_ordering.hpp> header includes:

*/

#include <bits/partial_ordering.hpp>

namespace std
{
	class weak_ordering
	{
		int value;

		constexpr explicit weak_ordering(eq v) noexcept : value(int(v)) {}
		constexpr explicit weak_ordering(ord v) noexcept : value(int(v)) {}

	public:
		static const weak_ordering less;
		static const weak_ordering equivalent;
		static const weak_ordering greater;

		//conversions
		constexpr operator partial_ordering() const noexcept { return (value == int(ord::less)) ? partial_ordering::less : (value == int(ord::greater)) ? partial_ordering::greater : partial_ordering::equivalent; };

		//comparisons
		friend constexpr bool operator==(weak_ordering v, int) noexcept { return v.value == int(eq::equivalent); }
		friend constexpr bool operator==(weak_ordering v, weak_ordering w) noexcept { return v.value == w.value; }
		friend constexpr bool operator< (weak_ordering v, int) noexcept { return v.value == int(ord::less); };
		friend constexpr bool operator> (weak_ordering v, int) noexcept { return v.value == int(ord::greater); };
		friend constexpr bool operator<=(weak_ordering v, int) noexcept { return v.value != int(ord::greater); };
		friend constexpr bool operator>=(weak_ordering v, int) noexcept { return v.value != int(ord::less); };
		friend constexpr bool operator< (int, weak_ordering v) noexcept { return v.value != int(ord::less); };
		friend constexpr bool operator> (int, weak_ordering v) noexcept { return v.value != int(ord::greater); };
		friend constexpr bool operator<=(int, weak_ordering v) noexcept { return v.value == int(ord::greater); };
		friend constexpr bool operator>=(int, weak_ordering v) noexcept { return v.value == int(ord::less); };

		friend constexpr weak_ordering operator<=>(weak_ordering v, int) noexcept { return v; };
		friend constexpr weak_ordering operator<=>(int, weak_ordering v) noexcept { return v.value == int(ord::less) ? weak_ordering(ord::greater) : v.value == int(ord::greater) ? weak_ordering(ord::less) : v;  };

	};

	inline constexpr weak_ordering weak_ordering::less(ord::less);
	inline constexpr weak_ordering weak_ordering::equivalent(eq::equivalent);
	inline constexpr weak_ordering weak_ordering::greater(ord::greater);
} // namespace std
