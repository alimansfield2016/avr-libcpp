#pragma once

#include <avr/io/register.hpp>

namespace AVR
{
	class ADC
	{
		//ADCH
		//ADCL
		//ADC
		//ACDSRA
		//ACDSRB
	public:

		void complete() const volatile;
		void waitComplete() const volatile;
		
	};
} // namespace AVR
