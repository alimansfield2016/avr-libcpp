#pragma once

#include <stdlib.h>

typedef int16_t ptrdiff_t;

namespace am_cxx
{
	template <typename T>
	class c_allocator
	{
	public:
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T *pointer;
		typedef const T *const_pointer;
		typedef T &reference;
		typedef const T &const_reference;
		typedef T value_type;

		template <typename T1>
		struct rebind
		{
			typedef c_allocator<T1> other;
		};

		c_allocator() noexcept {}
		c_allocator(const c_allocator &) noexcept {}
		template <typename T1>
		c_allocator(const c_allocator<T1> &) noexcept {}
		~c_allocator() noexcept {}

		pointer allocate(size_type n, const void * = nullptr)
		{
			return static_cast<T *>(malloc(n * sizeof(T)));
		}

		void deallocate(T *ptr, size_type)
		{
			free(ptr);
		}

		constexpr size_type max_size() const noexcept { return size_type(-1) / sizeof(T); }
	};
} // namespace am_cxx
