#pragma once

/*
AVR C++ Standard Library
Written by Alex Mansfield
Copyright 2020

Buffer Header

The <buffer.hpp> header includes:
- ring_buffer<T, N>

*/
#include <cstdlib>
#include <bit>

namespace std
{

	
	template<class T, std::size_t N> requires(std::popcount(N) == 1)
	class ring_buffer 
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
	private:
		constexpr static std::size_t s_size = N;
		constexpr static std::size_t s_mask = s_size-1;
		T m_buf[s_size];
		std::size_t m_offset;
		volatile std::size_t m_count;
	public:
		inline constexpr std::size_t idx(std::size_t _idx) const { return (_idx + m_offset)&s_mask; }
		inline constexpr std::size_t count() const volatile { return m_count; }
		inline constexpr std::size_t space() const { return s_size-m_count; }
		inline constexpr std::size_t size() const { return s_size; }
		inline constexpr bool full() const { return m_count >= s_size; }
		inline constexpr bool empty() const { return m_count == 0; }

		constexpr const_reference operator*() const { return m_buf[m_offset]; }
		reference operator*() { return m_buf[m_offset]; }
		constexpr const_reference operator[](std::size_t _idx) const { return m_buf[idx(_idx)]; }
		reference operator[](std::size_t _idx) { return m_buf[idx(_idx)]; }
		
		void push_back(value_type v) { m_buf[idx(m_count++)] = v; }
		value_type pop_front() { value_type v = *(*this); m_count--; ++m_offset &= s_mask; return v; }

	};

	// template<class T, std::size_t N>
	// class ring_buffer
	// {
	// public:
	// 	using value_type = T;
	// 	using pointer = T*;
	// 	using const_pointer = const T*;
	// 	using reference = T&;
	// 	using const_reference = const T&;
	// private:
	// 	constexpr static std::size_t s_size = N;
	// 	T m_buf[s_size];
	// 	std::size_t m_offset;
	// 	std::size_t m_count;
	// public:
	// 	constexpr std::size_t idx(std::size_t _idx) const { return (_idx + m_offset)%s_size; }
	// 	constexpr std::size_t count() const { return m_count; }
	// 	constexpr bool full() const { return m_count <= s_size; }
	// 	constexpr bool empty() const { return m_count == 0; }

	// 	constexpr const_reference operator*() const { return m_buf[m_offset]; }
	// 	reference operator*() { return m_buf[m_offset]; }
	// 	constexpr const_reference operator[](std::size_t _idx) const { return m_buf[idx(_idx)]; }
	// 	reference operator[](std::size_t _idx) { return m_buf[idx(_idx)]; }
		
	// 	void push_back(value_type v) { m_buf[idx(m_count++)] = v; }
	// 	value_type pop_front() { value_type v = m_buf[idx(0)]; m_count--; m_offset++; if(m_offset >= s_size) m_offset -= s_size;  return v; }

	// };

	
} // namespace std
