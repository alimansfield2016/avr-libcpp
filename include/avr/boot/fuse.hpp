#pragma once

#include <stdint.h>
#include <avr/io/io.hpp>
#include <avr/boot/flash.hpp>

namespace AVR::Boot
{

	static uint8_t getLowFuse()
	{
		return lpmRead(static_cast<uint16_t>(FuseAddr::FuseLow), lpmOperation::FuseRead);
	}
	static uint8_t getHighFuse()
	{
		return lpmRead(static_cast<uint16_t>(FuseAddr::FuseHigh), lpmOperation::FuseRead);
	}
	static uint8_t getExtendedFuse()
	{		
		return lpmRead(static_cast<uint16_t>(FuseAddr::FuseExt), lpmOperation::FuseRead);
	}
} // namespace AVR::Boot
