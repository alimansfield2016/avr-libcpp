#pragma once

namespace std
{
	template <class Alloc>
	struct alloc_traits
	{
		typedef Alloc allocator_type;
		typedef Alloc::value_type value_type;
		typedef Alloc::pointer pointer;
		typedef Alloc::const_pointer const_pointer;
		typedef Alloc::void_pointer void_pointer;
		typedef Alloc::const_void_pointer;
		typedef Alloc::reference reference;
	};
} // namespace std
