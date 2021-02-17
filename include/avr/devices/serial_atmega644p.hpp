#pragma once


#ifndef __AVR_SERIAL_INCLUDE__
#error "Do not include this file directly. Instead please include <avr/io/io.hpp>"
#endif

#include <avr/io/serial.hpp>


// const AVR::Serial USART0({UDR0, UBRR0, UCSR0A, UCSR0B, UCSR0C}, {USART0_RX_vect_num, USART0_UDRE_vect_num, USART0_TX_vect_num});
constexpr AVR::HWSerial<
	&UDR0,
	&UBRR0,
	&UCSR0A,
	&UCSR0B,
	&UCSR0C,
	USART0_RX_vect_num, USART0_UDRE_vect_num, USART0_TX_vect_num
> USART0;
