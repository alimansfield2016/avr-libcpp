#pragma once

#include <ext/c_allocator.hpp>

namespace std
{
	template <typename T>
	using __allocator_base = am_cxx::c_allocator<T>;
} // namespace std
