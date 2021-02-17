#pragma once

#include <avr/io/port.hpp>

namespace AVR
{
	class Pin
	{
		const Port m_port;
		const uint8_t m_mask;

	public:
		constexpr Pin(const Port &port, uint8_t bit)
			: m_port(port),
			  m_mask(1 << bit)
		{
		}

		inline void direction(bool output) const
		{
			if (output)
				m_port.ddr() |= m_mask;
			else
				m_port.ddr() &= ~m_mask;
		}

		inline void operator=(bool val) const
		{
			if (val)
				m_port.port() |= m_mask;
			else
				m_port.port() &= ~m_mask;
		}
	};
} // namespace AVR
