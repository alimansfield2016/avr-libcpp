#pragma once

#include <initializer_list>

#include <stdint.h>

#include <avr/io.h>

namespace AVR
{

	struct PortRegisters
	{
		const intptr_t r_ddr;
		const intptr_t r_pin;
		const intptr_t r_port;
	};

	// template <const struct PortRegisters &... ports>
	class Port
	{
	private:
		typedef volatile uint8_t reg_t;
		constexpr static reg_t &reg(intptr_t ptr) { return *(reg_t *)ptr; }

		const PortRegisters m_regs;

	public:
		constexpr Port(const PortRegisters &port_regs)
			: m_regs(port_regs)
		{
		}
		constexpr Port(const Port &copy)
			: m_regs(copy.m_regs)
		{
		}

		inline uint8_t operator=(uint8_t val) const { return reg(m_regs.r_port) = val; }
		inline reg_t &ddr() const { return reg(m_regs.r_ddr); }
		inline reg_t &pin() const { return reg(m_regs.r_pin); }
		inline reg_t &port() const { return reg(m_regs.r_port); }
	};
} // namespace AVR
