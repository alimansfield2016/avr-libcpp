#pragma once

#include <stdint.h>
// #include <string_old>
#include <string>
#include <avr/interrupt/interrupt.hpp>
#include <avr/io/register.hpp>

#ifndef F_CPU
#error Serial requires F_CPU to be defined
#else
#if !F_CPU
#error Serial requires that F_CPU Must have a value
#endif
#endif

#include <avr/memory/pgmspace.hpp>

// #include <iostream_old>
// #include <istream>

const char clr_str[] PROGMEM = "\033[2J";

namespace AVR
{
	// class SerialOut;
	// class SerialIn;
	class Serial
	{
	public:
		constexpr Serial() {}
		char getc() const { char c; read(&c, 1); return c; }
		void putc(char c) const { write(&c, 1); }
		void puts(const char *s) const 
		{
			std::size_t len = 0;
			while(s[len])++len;
			write(s, len);
		}
		void puts(AVR::pgm_string s) const
		{
			std::size_t len = 0;
			while(s[len])++len;
			writePGM(s, len);
		}
		virtual void read(char *dst, std::size_t len) const = 0;
		virtual void write(const char *src, std::size_t len) const = 0;
		virtual void writePGM(AVR::pgm_ptr<char> src, std::size_t len) const = 0;
		void Print(char c) const { putc(c); }
		virtual std::size_t available() const volatile;
	};

	class _Serial //: public std::os
	{
	public:
		constexpr static AVR::pgm_ptr<char> clr = AVR::pgm_ptr<char>(clr_str);
		struct SerialRegisters
		{
			const AVR::Register8 r_udr;
			const AVR::Register16 r_ubrr;
			const AVR::Register8 r_ucsra;
			const AVR::Register8 r_ucsrb;
			const AVR::Register8 r_ucsrc;
		};
	
		struct SerialInterrupts
		{
			const uint8_t int_rx;
			const uint8_t int_udre;
			const uint8_t int_tx;
		};
	
		enum class Interrupt
		{
			rx,
			udre,
			tx,
		};
	
	
	private:
		const SerialRegisters m_regs;
		const SerialInterrupts m_ints;
	
		typedef volatile uint8_t reg_t;
		typedef volatile uint16_t wreg_t;
	
	
	private:
		inline void tx(char c) const __attribute__((always_inline))
		{
			while (!(m_regs.r_ucsra & 0x20))
				;
			m_regs.r_udr = c;
		}
		inline char rx() const __attribute__((always_inline))
		{
			while (!(m_regs.r_ucsra & 0x80))
				;
			return m_regs.r_udr;
		}
	
	public:
	
	public:
		constexpr _Serial() = delete;
		constexpr _Serial(SerialRegisters regs, SerialInterrupts ints)
			: m_regs(regs),
			  m_ints(ints)
		{
		}
	
	
		// inline void registerHandler(void (*cb)(), Serial::Interrupt interrupt) const
		// {
		// 	uint8_t _int = 0;
		// 	switch (interrupt)
		// 	{
		// 	case Serial::Interrupt::rx:
		// 		_int = m_ints.int_rx;
		// 		break;
		// 	case Serial::Interrupt::udre:
		// 		_int = m_ints.int_udre;
		// 		break;
		// 	case Serial::Interrupt::tx:
		// 		_int = m_ints.int_tx;
		// 		break;
		// 	}
		// 	if (_int == 0)
		// 		return;
		// 	AVR::Interrupt::registerHandler(cb, _int);
		// }
		// inline void unregisterHandler(void (*cb)(), Serial::Interrupt interrupt) const
		// {
		// 	uint8_t _int = 0;
		// 	switch (interrupt)
		// 	{
		// 	case Serial::Interrupt::rx:
		// 		_int = m_ints.int_rx;
		// 		break;
		// 	case Serial::Interrupt::udre:
		// 		_int = m_ints.int_udre;
		// 		break;
		// 	case Serial::Interrupt::tx:
		// 		_int = m_ints.int_tx;
		// 		break;
		// 	}
		// 	if (_int == 0)
		// 		return;
		// 	AVR::Interrupt::unregisterHandler(cb, _int);
		// }
	
		void PrintLn(const char *s = nullptr) const 
		{
			if (s)
				while (*s)
					tx(*s++);
	
			tx('\r');
			tx('\n');
		}
	
		// void PrintLn(const std::fixed_string &str) const
		// {
		// 	const char *s = str.constData();
		// 	if (s)
		// 		while (*s){
		// 			tx(*s);
		// 			s++;
		// 		}
	
		// 	tx('\r');
		// 	tx('\n');
		// }
	
		/**
		 * Always inline this function to avoid issue
		 * where AVR crashes and resets weirdly.
		 * Not at all sure why this happens, 
		 * but think it might be a GCC issue
		 */
		inline void PrintLn(const AVR::pgm_ptr<char> &ps) const //__attribute__((always_inline))
		{
			// const char *s = ps.get();
			const uint8_t *s = reinterpret_cast<const uint8_t*>(ps.get());
			char c;
			if(s)
				while((c=pgm_read_byte(s++)))
					tx(c);
			tx('\r');
			tx('\n');
		}
	
		inline void Print(const AVR::pgm_ptr<char> &ps) const //__attribute__((always_inline))
		{
			const uint8_t *s = reinterpret_cast<const uint8_t*>(ps.get());
			char c;
			if(s)
				while((c=pgm_read_byte(s++)))
					tx(c);
		}
	
		inline void Print(const char *s) const
		{
			if(s)
				while (*s)
					tx(*s++);
		}
		// inline void Print(const std::fixed_string &str) const
		// {
		// 	Print('F');
		// 	const char *s = str.constData();
		// 	if (s)
		// 		while (*s)
		// 			tx(*s++);
		// }
		inline void Print(char c) const
		{
			tx(c);
		}
	
		inline void PrintHex(uint8_t hex) const
		{
			uint8_t val = hex >> 4;
			if (val > 0x09)
				Print('A' + val - 0x0A);
			else
				Print('0' + val);
	
			val = hex & 0x0F;
			if (val > 0x09)
				Print('A' + val - 0x0A);
			else
				Print('0' + val);
		}
	
		inline char getChar() const { return rx(); }
		bool available() const volatile { return const_cast<const Register8&>(m_regs.r_ucsra) & 0x80; }
	};
	// inline constexpr static AVR::pgm_ptr<char> Serial::clr(clr_str);

	template<
	
			std::uintptr_t addr_udr,
			std::uintptr_t addr_ubrr,
			std::uintptr_t addr_ucsra,
			std::uintptr_t addr_ucsrb,
			std::uintptr_t addr_ucsrc,
			std::uint8_t int_rx,
			std::uint8_t int_udre,
			std::uint8_t int_tx
		>
	class HWSerial final : public Serial
	{
		static constexpr AVR::Register8 s_udr   = AVR::Register8(addr_udr);
		static constexpr AVR::Register16 s_ubrr = AVR::Register16(addr_ubrr);
		static constexpr AVR::Register8 s_ucsra = AVR::Register8(addr_ucsra);
		static constexpr AVR::Register8 s_ucsrb = AVR::Register8(addr_ucsrb);
		static constexpr AVR::Register8 s_ucsrc = AVR::Register8(addr_ucsrc);

		inline void tx(char c) const __attribute__((always_inline))
		{
			while (!(s_ucsra & 0x20))
				;
			s_udr = c;
		}
		inline char rx() const __attribute__((always_inline))
		{
			while (!(s_ucsra & 0x80))
				;
			return s_udr;
		}


		inline constexpr static uint16_t calc_ubrr(uint32_t baud, bool u2x = true)
		{
			if (u2x)
				return F_CPU / (8 * baud) - 1;
	
			else
				return F_CPU / (16 * baud) - 1;
		}
		inline constexpr static uint32_t calc_baud(uint16_t ubrr, bool u2x = true)
		{
			if (u2x)
				return (F_CPU / 8) * (ubrr + 1);
			else
				return (F_CPU / 16) * (ubrr + 1);
		}
	
	public:
		constexpr HWSerial(){}
		/**
		 * Open a serial connection using specified Baud Rate
		 * Optionally use 2x speed. 
		 * For speeds > 57600 Baud, 2x speed IS REQUIRED
		 * in order for the serial to work correctly
		 * Maximum speed with 2x is 115200 Baud
		 * at 12MHz we could actually go up to 1.5Mbaud potentially
		 */
		inline uint32_t baud(bool u2x = true) const { return calc_baud(s_ubrr, u2x); }
		inline uint32_t begin(uint32_t baud, bool u2x = true) const
		{
			uint16_t ubrr_val = calc_ubrr(baud, u2x);
			s_ubrr = ubrr_val;
			if (u2x)
				s_ucsra |= 0x02;
			else
				s_ucsra &= ~0x02;
	
			s_ucsrc = 0x06;
	
			//enable Rx AND Tx
			rx_enable();
			tx_enable();
			//Interrupts not currently enabled, but may be if required
			return calc_baud(u2x);
		}
		inline void end() const
		{
			//disable all serial features
			rx_disable();
			tx_disable();
			//disable interrupts
			rxc_int_disable();
			txc_int_disable();
			dre_int_disable();
		}
		void write(const char *s, std::size_t len) const override
		{
			while(len--)
				tx(*s++);
		}
		void writePGM(AVR::pgm_ptr<char> s, std::size_t len) const override
		{
			while(len--)
				tx(*s++);
		}
		void read(char *buf, std::size_t len) const override
		{
			while(len--)
				*buf++ = rx();
		}
		std::size_t available() const volatile override
		{
			if(s_ucsra & 0x80) return 1;
			return 0;
		}
	public:

		inline void rx_enable()  const { s_ucsrb |= 0x10; }
		inline void rx_disable() const { s_ucsrb &= ~0x10; }
		inline void tx_enable()  const { s_ucsrb |= 0x08; }
		inline void tx_disable() const { s_ucsrb &= ~0x08; }
	
		inline void rxc_int_enable()  const { s_ucsrb |= 0x80; }
		inline void rxc_int_disable() const { s_ucsrb &= ~0x80; }
		inline void txc_int_enable()  const { s_ucsrb |= 0x40; }
		inline void txc_int_disable() const { s_ucsrb &= ~0x40; }
		inline void dre_int_enable()  const { s_ucsrb |= 0x20; }
		inline void dre_int_disable() const { s_ucsrb &= ~0x20; }
	};
} // namespace AVR



#define __AVR_SERIAL_INCLUDE__
#include <avr/devices/serial_atmega644p.hpp>
