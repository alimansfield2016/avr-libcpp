#pragma once

/*
AVR C++ Standard Library
Written by Alex Mansfield
Copyright 2020

AVR Boot Flash Header

The <avr/boot/flash.hpp> header includes:
- 

*/

#include <avr/io/io.hpp>

namespace AVR::Boot
{
	constexpr AVR::Register8 __SPM_REG{SPMCSR};

	enum class spmOperation : uint8_t{
		PageBufferWrite = 0x01,
		PageErase = 0x03,
		PageWrite = 0x05,
		FuseWrite = 0x09,
	};

	enum class lpmOperation : uint8_t{
		FuseRead = 0x09,
		SignatureRead = 0x21,
	};

	enum class FuseAddr : uint16_t{
		FuseLow = 0x0000,
		Lock = 0x0001,
		FuseExt = 0x0002,
		FuseHigh = 0x0003,
	};

	inline static void spmOp(uintptr_t z, spmOperation operation){
		
		__asm__ __volatile__(
				"mov	r0, %0			\n\t"
				"sts	%1, r0			\n\t"
                "spm                    \n\t"
			::	"r" (operation),
				"i" (&__SPM_REG),
				"z" (z)
			:	"r0"
		);
	}
	inline static void spmWrite8(uint8_t b, uintptr_t z, spmOperation operation){
		
		__asm__ __volatile__(
				"mov	r0, %1			\n\t"
				"sts	%2, r0			\n\t"
				"mov	r0, %0			\n\t"
                "spm                    \n\t"
			::	"r" (b),
				"r" (operation),
				"i" (&__SPM_REG),
				"z" (z)
			:	"r0"
		);
	}
	inline static void spmWrite16(uint16_t w, uintptr_t z, spmOperation operation){
		
		__asm__ __volatile__(
				"movw	r0, %1			\n\t"
				"sts	%2, r0			\n\t"
				"mov	r0, %0			\n\t"
                "spm                    \n\t"
			::	"r" (w),
				"r" (operation),
				"i" (&__SPM_REG),
				"z" (z)
			:	"r0"
		);
	}

	inline static uint8_t lpmRead(uintptr_t z, lpmOperation operation){
		uint8_t out;
		__asm__ __volatile__(
				"mov	r0, %1			\n\t"
				"sts	%2, r0			\n\t"
                "lpm                    \n\t"
				"mov	%0, r0			\n\t"
			:	"=r" (out)
			:	"r" (operation),
				"i" (&__SPM_REG),
				"z" (z)
			:	"r0"
		);
		return out;
	}

	inline static void flashPageErase(){}
	inline static void flashPageWrite(){}
} // namespace AVR::Boot
