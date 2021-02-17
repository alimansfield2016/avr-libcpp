#pragma once

namespace std
{
	template <typename T, T val>
	struct integral_constant
	{
		static constexpr T value = val;
		using value_type = T;
		using type = integral_constant;

		constexpr operator value_type() const noexcept { return value; }

#if __cplusplus >= 201402L
		constexpr value_type operator()() const noexcept
		{
			return value;
		}
#endif
	};

#if __cplusplus >= 201703L
	template <bool b>
	using bool_constant = integral_constant<bool, b>;
#endif

	using true_type = integral_constant<bool, true>;
	using false_type = integral_constant<bool, false>;

} // namespace std
