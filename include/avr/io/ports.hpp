#pragma once

#include <avr/io/port.hpp>

#define REG(x) (intptr_t)(&x)
#define PORT(x)                   \
	static constexpr Port Port##x \
	{                             \
		{                         \
			REG(DDR##x),          \
				REG(PIN##x),      \
				REG(PORT##x)      \
		}                         \
	}

namespace AVR
{
	namespace Ports
	{
		PORT(A);
		PORT(B);
		PORT(C);
		PORT(D);
	} // namespace Ports

} // namespace AVR
