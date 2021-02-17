#pragma once

/*
AVR C++ Standard Library
Written by Alex Mansfield
Copyright 2020

Strong ordering Header

The <strong_ordering.hpp> header includes:

*/	

#include <bits/weak_ordering.hpp>
#include <bits/partial_ordering.hpp>


namespace std
{


	class strong_ordering
	{
		int value;

		constexpr explicit strong_ordering(eq v) : value(int(v)) {}
		constexpr explicit strong_ordering(ord v) : value(int(v)) {}

	public:
		//valid values
		static const strong_ordering less;
		static const strong_ordering equal;
		static const strong_ordering equivalent;
		static const strong_ordering greater;

		//conversions
		constexpr operator partial_ordering() const noexcept { return (value == int(ord::less)) ? partial_ordering::less : (value == int(ord::greater)) ? partial_ordering::greater : partial_ordering::equivalent; };
		constexpr operator weak_ordering() const noexcept { return (value == int(ord::less)) ? weak_ordering::less : (value == int(ord::greater)) ? weak_ordering::greater : weak_ordering::equivalent; };

		friend constexpr bool operator==(strong_ordering v, int) noexcept { return v.value == int(eq::equal) || v.value == int(eq::equivalent); };
		friend constexpr bool operator==(strong_ordering v, strong_ordering w) noexcept = default;
		friend constexpr bool operator< (strong_ordering v, int) noexcept { return v.value == int(ord::less); };
		friend constexpr bool operator> (strong_ordering v, int) noexcept { return v.value == int(ord::greater); };
		friend constexpr bool operator<=(strong_ordering v, int) noexcept { return v.value != int(ord::greater); };
		friend constexpr bool operator>=(strong_ordering v, int) noexcept { return v.value != int(ord::less); };
		friend constexpr bool operator< (int, strong_ordering v) noexcept { return v.value != int(ord::less); };
		friend constexpr bool operator> (int, strong_ordering v) noexcept { return v.value != int(ord::greater); };
		friend constexpr bool operator<=(int, strong_ordering v) noexcept { return v.value == int(ord::greater); };
		friend constexpr bool operator>=(int, strong_ordering v) noexcept { return v.value == int(ord::less); };

		friend constexpr strong_ordering operator<=>(strong_ordering v, int) noexcept { return v; }
		friend constexpr strong_ordering operator<=>(int, strong_ordering v) noexcept { return v.value == int(ord::less) ? strong_ordering::greater : v.value == int(ord::greater) ? strong_ordering::less : v; };
	};

	inline constexpr strong_ordering strong_ordering::less(ord::less);
	inline constexpr strong_ordering strong_ordering::equal(eq::equal);
	inline constexpr strong_ordering strong_ordering::equivalent(eq::equivalent);
	inline constexpr strong_ordering strong_ordering::greater(ord::greater);
} // namespace std