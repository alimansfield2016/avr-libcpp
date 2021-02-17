#pragma once

#include <avr/interrupt/interrupt.hpp>

namespace AVR
{
	template<class T>
	class atomic
	{
		T m_val;
	public:
		typedef T value_type;

		operator value_type() const { return m_val; }

	};

	class mutex
	{
		volatile bool m_lock;
	public:
		void lock() { while(!try_lock()); }
		bool try_lock() { AVR::Interrupt::InterruptHolder hold; if(locked()) return false; return m_lock = true; }
		bool locked() const { return m_lock; }
	};
} // namespace AVR
