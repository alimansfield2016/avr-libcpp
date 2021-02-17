#ifndef AVR_INTERNAL_INTERRUPTS_HPP
#define AVR_INTERNAL_INTERRUPTS_HPP

#ifdef ENABLE_INTERRUPTS
#error
#include <avr/interrupt/interrupts.hpp>
#endif

// #include <avr/interrupt.h>
#include <avr/io/io.hpp>
#include <array>
#include <list>


namespace AVR
{

	namespace Interrupt
	{
		extern ::std::array<::std::list<void (*)()>, _VECTORS_COUNT + 1> s_callbacks;
/* 
		struct Handler
		{
			template<typename T>
			using member_handler = void(T::*)();
			using handler = void __attribute__((interrupt))(*)();
			union
			{
				member_handler<Handler> m_member_handler;
				handler m_handler;
			};
			
			Handler *m_this;
			
			template<class T>
			constexpr Handler(member_handler<T> _handler, T* _this) : 
				m_member_handler{reinterpret_cast<member_handler<Handler>>(_handler)},
				m_this{reinterpret_cast<Handler*>(_this)} { }
			constexpr Handler(handler _handler) : 
				m_handler{_handler},
				m_this{nullptr} { }
			void operator()()
			{
				if(m_this)
					(m_this->*m_member_handler)();
				else
					m_handler();
			}
		};
 */
	
	
		[[maybe_unused]] void Interrupt(uint8_t int_id);
		// void setHandler(Handler, uint8_t vector);
		//setHandler
		//removeHandler
		//addHandler
		bool registerHandler(void (*cb)(), uint8_t vector);
		bool unregisterHandler(void (*cb)(), uint8_t vector);


		inline void enable() { __asm__ __volatile__ ("sei" ::: "memory"); }
		inline void disable() { __asm__ __volatile__ ("cli" ::: "memory"); }

		enum class Vector : bool {
			Application = false,
			BootLoader = true,
		};

		inline void setVectors(Vector vec)
		{
			uint8_t mask = 0;
			if(vec == Vector::BootLoader) mask = 0x01;
			MCUCR = 0x02;
			MCUCR = mask;
		}

	// }
	// namespace Interrupt
	// {

		class InterruptKeeper
		{
		private:
			bool prev_state;

		public:
			InterruptKeeper()
			{
				prev_state = SREG & _BV(SREG_I);
			}
			~InterruptKeeper()
			{
				if (prev_state)
					enable();
				else
					disable();
			}

		};

		template<bool K = false>
		class InterruptHolder;

		template<>
		class InterruptHolder<false> : public InterruptKeeper
		{
		public:
			InterruptHolder() : InterruptKeeper() 
			{
				disable();
			}
		};

		//Optional optimised version
		//Guarantees enabling of interrupts on destruction
		template<>
		class InterruptHolder<true>
		{
		public:
			InterruptHolder()
			{
				disable();
			}
			~InterruptHolder()
			{
				enable();
			}
		};

		template<bool K = true>
		class InterruptWindow;

		template<>
		class InterruptWindow<true> : public InterruptKeeper
		{
		public:
			InterruptWindow() : InterruptKeeper()
			{
				enable();
			}
		};
		//Optional optimised version
		//Guarantees disabling of interrupts on destruction
		template<>
		class InterruptWindow<false> : public InterruptKeeper
		{
		public:
			InterruptWindow()
			{
				enable();
			}
			~InterruptWindow()
			{
				disable();
			}
		};


		// DEFINE_VECTORS(_VECTORS_COUNT)


	} // namespace Interrupt

} // namespace AVR

#endif