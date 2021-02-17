#pragma once

#include <type_traits>


namespace std
{

	template<class T, size_t S = 0>
	class constexpr_vector : public constexpr_vector<T>
	{
		friend class constexpr_vector<T>;
		using super = constexpr_vector<T>;
		using value_type = super::value_type;
		value_type m_arr[S];

	public:
		constexpr constexpr_vector(const std::initializer_list<T> &init) : 
			constexpr_vector<T, 0>{S}
			{
				auto it = init.begin();
				for(uint8_t i = 0; i < S && it != init.end(); i++)
					m_arr[i] = *it++;
			}

	};

	template<class T>
	class constexpr_vector<T, 0>
	{
	public:
		// using value_type = T;
		using value_type = std::remove_const<T>::type;
		using pointer = value_type*;
		using const_pointer = const T*;
		using iterator = pointer;
		using const_iterator = const T*;
		using reference = value_type&;
		using const_reference = const T&;
		using size_type = size_t;
	private:
		const size_type m_size;
		constexpr constexpr_vector(size_type _size) : 
			m_size{_size} {}
	public:

		constexpr constexpr_vector(const std::initializer_list<T> &init) : 
			m_size{0} {}
			
		constexpr_vector(constexpr_vector&) = delete;
		constexpr_vector(const constexpr_vector&) = delete;
		constexpr_vector(constexpr_vector&&) = delete;
		constexpr_vector(const constexpr_vector&&) = delete;
			
		constexpr pointer ptr() { return static_cast<constexpr_vector<value_type, 1>*>(this)->m_arr; }
		constexpr const_pointer ptr() const { return ptr(); }

		constexpr iterator begin() { return ptr(); }
		constexpr iterator end() { return &ptr()[m_size]; }
		constexpr const_iterator begin() const { return ptr(); }
		constexpr const_iterator end() const { return &ptr()[m_size]; }

		constexpr const_iterator cbegin() const { return ptr(); }
		constexpr const_iterator cend() const { return ptr(); }

		constexpr size_type size() const { return m_size; }
		constexpr const size_type* size_p() const { return &m_size; }

		constexpr reference operator[](size_type idx) { return ptr()[idx]; }
		constexpr const_reference operator[](size_type idx) const { return ptr()[idx]; }

	};
	// template<class T, size_t S = 0>
	// class constexpr_vector : public constexpr_vector<T, 0>
	// {
	// 	friend class constexpr_vector<T>;
	// 	using value_type = std::remove_const<T>::type;
	// 	value_type m_arr[S];

	// public:
	// 	constexpr constexpr_vector(const std::initializer_list<T> &init) : 
	// 		constexpr_vector<T, 0>{S}
	// 		{
	// 			auto it = init.begin();
	// 			for(uint8_t i = 0; i < S && it != init.end(); i++)
	// 				m_arr[i] = *it++;
	// 		}

	// };
} // namespace std
