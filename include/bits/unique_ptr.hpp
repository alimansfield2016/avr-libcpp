#pragma once

/*

AVR C++ Standard Library
Written by Alex Mansfield
Copyright 2020

uniqie_ptr Header

The <unique_ptr.hpp> header includes:
	- std::unique_ptr	c++11


*/


namespace std
{
	template<class T>
	struct default_delete{
		constexpr default_delete() = default;
		
		void operator()(T* ptr) const 
		{
			static_assert(sizeof(T) > 0, "Can't delete pointer to incomplete type!");
			delete(ptr);
		}

	};

	template<class T, class Deleter = std::default_delete<T>>
	class unique_ptr
	{
		public:
		typedef T* pointer;
		typedef T element_type;
		typedef Deleter deleter_type;

		unique_ptr(pointer ptr) : m_ptr(ptr) {}
		unique_ptr(const unique_ptr &copy) = delete;
		unique_ptr &operator= (const unique_ptr &copy) = delete;
		unique_ptr(unique_ptr &&move) {
			m_ptr = move->m_ptr;
			move->m_ptr = nullptr;
		}
		~unique_ptr() { delete_ptr(); }
		unique_ptr &operator=(unique_ptr &&move) {
			delete_ptr();
			m_ptr = move.m_ptr;
			move.m_ptr = nullptr;
			return *this;
		}

		Deleter get_deleter() const { return {}; }

		private:
		void delete_ptr() { if(m_ptr) get_deleter()(m_ptr); m_ptr = nullptr; }

		pointer m_ptr;
	};
	
} // namespace std

