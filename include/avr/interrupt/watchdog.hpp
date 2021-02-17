#pragma once

/*
AVR C++ Standard Library
Written by Alex Mansfield
Copyright 2020

AVR Watchdog Header

The <avr/interrupt/watchdog.hpp> header includes:

*/

#include <avr/interrupt/interrupt.hpp>
#include <avr/io/io.hpp>

namespace AVR
{
	namespace Watchdog
	{
		
	
		inline static void __attribute__((always_inline)) reset() 
		{
			__asm__ __volatile__ ("wdr");
		}
		enum class Mode
		{
			Stopped,
			Interrupt,
			Reset,
			InterruptAndReset,
		};
		inline static void setMode(Mode mode)
		{
			AVR::Interrupt::InterruptHolder hold;
			uint8_t bits = 0;
			switch (mode)
			{
			case Mode::InterruptAndReset:
				bits |= _BV(WDE);
				[[fallthrough]];
			case Mode::Interrupt:
				bits |= _BV(WDIE);
				break;
			case Mode::Reset:
				bits |= _BV(WDE);
				break;
			case Mode::Stopped:
				break;
			}

			bits |= WDTCSR & WDTCSR & (_BV(WDP0) | _BV(WDP1) | _BV(WDP2) | _BV(WDP3));

			reset();
			WDTCSR |= _BV(WDCE) | _BV(WDE);
			WDTCSR = bits;
		}
		enum class TimeoutCycles : uint8_t
		{
			_2K = 0,
			_4K = 1,
			_8K = 2,
			_16K = 3,
			_32K = 4,
			_64K = 5,
			_128K = 6,
			_256K = 7,
			_512K = 8,
			_1024K = 9
		};
		inline static void setTimeoutCycles(TimeoutCycles mask)
		{
			AVR::Interrupt::InterruptHolder hold; //hold interrupts until we leave scope
			uint8_t val = static_cast<uint8_t>(mask);
			uint8_t bits = (val & 0x07) | ((val << 2) & 0x20);
			bits |= WDTCSR & (_BV(WDIE) | _BV(WDE));
			reset();
			WDTCSR |= _BV(WDCE) | _BV(WDE);
			WDTCSR = bits;
			reset();
		}

		inline static void enable(Mode mode = Mode::Reset)
		{
			setMode(mode);
		}
		inline static void disable()
		{
			setMode(Mode::Stopped);
		}
	} // namespace Watchdog

} // namespace AVR
