#pragma once

#include <stdint.h>
#include <avr/io/io.hpp>
#include <avr/boot/flash.hpp>


namespace AVR::Boot
{
	inline static uint8_t getLockByte()
	{
		return lpmRead(static_cast<uint16_t>(FuseAddr::Lock), lpmOperation::FuseRead);
	}

	inline static void setLockByte(uint8_t in)
	{
		spmWrite8(in, static_cast<uint16_t>(FuseAddr::Lock), spmOperation::FuseWrite);
	}
} // namespace AVR::Boot
