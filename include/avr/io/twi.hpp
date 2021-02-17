#pragma once

#include <stdint.h>

#ifndef F_CPU
#error TWI requires F_CPU to be defined
#else
#if !F_CPU
#error TWI requires that F_CPU Must have a value
#endif
#endif

namespace AVR
{

	class TWITransmission;

	class TWI
	{
		friend class TWITransmission;

	public:
		struct TWIRegisters
		{
			const intptr_t r_twbr;
			const intptr_t r_twcr;
			const intptr_t r_twsr;
			const intptr_t r_twdr;
			const intptr_t r_twar;
			const intptr_t r_twamr;
		};

		enum class state
		{
			None,
			Start,
			RepeatStart,
			SlaAck,
			SlaNack,
			DataAck,
			DataNack,
			AbribtrationLost,
		};

	private:
		typedef volatile uint8_t reg_t;
		TWIRegisters m_regs;

		inline static constexpr reg_t &reg(intptr_t ptr) { return *(reg_t *)ptr; }

		inline void start() const
		{
			uint8_t reg_val = reg(m_regs.r_twcr);
			reg_val &= ~0x12;
			reg_val |= 0xA4;
			reg(m_regs.r_twcr) = reg_val;
		}
		inline void stop() const
		{
			uint8_t reg_val = reg(m_regs.r_twcr);
			reg_val &= ~22;
			reg_val |= 0x94;
			reg(m_regs.r_twcr) = reg_val;
		}

		inline void tx(uint8_t d) const
		{
			reg(m_regs.r_twdr) = d;

			uint8_t reg_val = reg(m_regs.r_twcr);
			reg_val &= ~0x32;
			reg_val |= 0x84;
			reg(m_regs.r_twcr) = reg_val;
		}

		inline uint8_t rx() const
		{
			return reg(m_regs.r_twdr);
		}

		inline void set_ack(bool ack = true) const
		{
			uint8_t reg_val = reg(m_regs.r_twcr);
			reg_val &= ~0x72;
			reg_val |= 0x84;
			if (ack)
				reg_val |= 0x40;
			reg(m_regs.r_twcr) = reg_val;
		}

		inline void wait_int() const
		{
			//wait for interrupt flag
			while (!(reg(m_regs.r_twcr) & 0x80))
				;
		}

	public:
		constexpr TWI(const TWIRegisters &twi_regs)
			: m_regs(twi_regs)
		{
		}

		inline void set_scl_freq(uint32_t freq) const
		{
			uint16_t scale = (F_CPU / freq - 16) / 2;
			// (TWBR) * 4^(twps) = scale;
			uint8_t twps = 0;
			while (scale >= 256)
			{
				twps++;
				scale >>= 2; //scale /= 4
			}
			reg(m_regs.r_twbr) = scale;
			//set twps

			reg(m_regs.r_twsr) &= ~0x03;	   //clear least two bits
			reg(m_regs.r_twsr) |= twps & 0x03; //set least two bits with prescaler
		}

		inline void init_master(uint32_t baud) const
		{
			//clear status register
			reg(m_regs.r_twsr) = 0;
			//set baud rate
			set_scl_freq(baud);
			//enable
		}

		TWITransmission beginTransmission(uint8_t addr) const;

		bool device(uint8_t addr) const;
		void scan(void (*l)(uint8_t)) const;

		inline void init_slave(uint8_t addr) const
		{
			//set slave addr
			//enable
		}
	};

	class TWITransmission
	{
		friend class TWI;

		const TWI m_interface;
		uint8_t m_slave_addr;

		mutable TWI::state m_state;
		bool m_complete;
		bool m_rnw;

		inline void updateStatus() const
		{
			if (m_rnw)
			{
				//Master Read mode
				//check status flags
				uint8_t s = status();
				switch (s)
				{
				case 0x08:
					m_state = TWI::state::Start;
					break;
				case 0x10:
					m_state = TWI::state::RepeatStart;
					break;
				case 0x38:
					m_state = TWI::state::AbribtrationLost;
					break;
				case 0x40:
					m_state = TWI::state::SlaAck;
					break;
				case 0x48:
					m_state = TWI::state::SlaNack;
				case 0x50:
					m_state = TWI::state::DataAck;
					break;
				case 0x58:
					m_state = TWI::state::DataNack;
					break;

				default:
					m_state = TWI::state::None;
					break;
				}
			}
			else
			{
				//Master Write mode
				//check status flags
				uint8_t s = status();
				switch (s)
				{
				case 0x08:
					m_state = TWI::state::Start;
					break;
				case 0x10:
					m_state = TWI::state::RepeatStart;
					break;
				case 0x18:
					m_state = TWI::state::SlaAck;
					break;
				case 0x20:
					m_state = TWI::state::SlaNack;
				case 0x28:
					m_state = TWI::state::DataAck;
					break;
				case 0x30:
					m_state = TWI::state::DataNack;
					break;
				case 0x38:
					m_state = TWI::state::AbribtrationLost;
					break;
				default:
					m_state = TWI::state::None;
					break;
				}
			}
		}

		inline void begin(bool rnw)
		{

			m_interface.start();
			m_interface.wait_int();
			//check status register
			//Begin always in write mode;
			m_interface.tx((m_slave_addr << 1) | rnw);
			m_interface.wait_int();
			//check status register
			updateStatus();
		}

		inline uint8_t status() const
		{
			return m_interface.reg(m_interface.m_regs.r_twsr) & ~0x03;
		}

	public:
		TWITransmission(const TWI &interface, uint8_t slave_addr)
			: m_interface(interface),
			  m_slave_addr(slave_addr),
			  m_state(TWI::state::None),
			  m_complete(false),
			  m_rnw(false)
		{
			begin(m_rnw);
		}

		~TWITransmission()
		{
			if (!m_complete)
				end();
		}

		inline TWI::state state() const { return m_state; }

		inline TWITransmission &write(uint8_t data)
		{

			m_interface.tx(data);
			m_interface.wait_int();
			updateStatus();

			return *this;
		}

		inline TWITransmission &read(uint8_t &data, bool ack = true)
		{
			m_interface.set_ack(ack);
			m_interface.wait_int();
			updateStatus();

			data = m_interface.rx();
			return *this;
		}

		inline TWITransmission &beginRepeat(bool rnw = false)
		{
			m_rnw = rnw;
			begin(rnw);
			return *this;
		}
		inline TWITransmission &beginRepeat(uint8_t addr, bool rnw = false)
		{
			m_slave_addr = addr;
			m_rnw = rnw;
			begin(rnw);
			return *this;
		}

		inline void end()
		{
			m_interface.stop();
			m_complete = true;
		}
	};

	inline TWITransmission TWI::beginTransmission(uint8_t addr) const
	{
		return {*this, addr};
	}

	inline bool TWI::device(uint8_t addr) const
	{
		auto tx = beginTransmission(addr);
		if (tx.state() == state::SlaAck)
			return true;
		return false;
	}

	inline void TWI::scan(void (*l)(uint8_t)) const
	{
		for (uint8_t addr = (0x10 >> 1); addr < (0xF0 >> 1); addr++)
		{
			if (device(addr))
				l(addr);
		}
	}

} // namespace AVR
