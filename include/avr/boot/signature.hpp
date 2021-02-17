#pragma once

#include <avr/boot/flash.hpp>

namespace AVR::Boot
{
	inline static uint8_t signatureRead(uint8_t b)
	{
		return lpmRead(b+b, lpmOperation::SignatureRead);
	}
	inline static uint8_t oscCalibrationRead()
	{
		return lpmRead(0x0001, lpmOperation::SignatureRead);
	}
} // namespace AVR::Boot
