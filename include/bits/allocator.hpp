#pragma once

#include <bits/c++allocator.hpp>
#include <utility>

namespace std
{
	template <typename T>
	class allocator;

#if true //check if < c++20
	template <>
	class allocator<void>
	{
	public:
		typedef void value_type;
		// typedef T &reference;
		// typedef const T &const_reference;
		typedef void *pointer;
		typedef const void *const_pointer;
		typedef size_t size_type;

		template <typename T1>
		struct rebind
		{
			typedef allocator<T1> other;
		};

		template <typename U, typename... Args>
		void construct(U *ptr, Args &&... args)
		{
			::new ((pointer)ptr) U(std::forward<Args>(args)...);
		}

		template <typename U>
		void destroy(U *ptr)
		{
			ptr->~U();
		}
	};
#endif

	template <typename T>
	class allocator : public allocator_base<T>
	{
	public:
		typedef T value_type;
		typedef T &reference;
		typedef const T &const_reference;
		typedef T *pointer;
		typedef const T *const_pointer;
		typedef size_t size_type;
	};
} // namespace std
