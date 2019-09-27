/**
 * C65
 * Copyright (C) 2019 David Jolly
 *
 * C65 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * C65 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../include/system/processor.h"
#include "./processor_type.h"

namespace c65 {

	namespace system {

		processor::processor(void) :
			m_accumulator({}),
			m_index_x({}),
			m_index_y({}),
			m_interrupt(0),
			m_maskable({}),
			m_non_maskable({}),
			m_program_counter({}),
			m_reset({}),
			m_stack_pointer({}),
			m_status({}),
			m_stop(false),
			m_wait(false)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		processor::~processor(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		uint8_t
		processor::execute(
			__in c65::interface::bus &bus
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			// TODO: FETCH/EXECUTE INSTRUCTION, INCREMENT RETURN
			result = execute_nop();
			// ---

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_nop(void)
		{
			uint8_t result;

			TRACE_ENTRY();

			// TODO: DEFINE INSTRUCTION CYCLES
			result = 2;
			// ---

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		void
		processor::interrupt(
			__in int type
			)
		{
			TRACE_ENTRY_FORMAT("Type=%i(%s)", type, INTERRUPT_STRING(type));

			switch(type) {
				case C65_INTERRUPT_MASKABLE:
				case C65_INTERRUPT_NON_MASKABLE:
					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Processor interrupt request", "%i(%s)",
						type, INTERRUPT_STRING(type));
					MASK_SET(m_interrupt, type);
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INTERRUPT_INVALID,
						"%i(%s)", type, INTERRUPT_STRING(type));
			}

			TRACE_EXIT();
		}

		void
		processor::on_initialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Processor initializing");
			TRACE_MESSAGE(LEVEL_INFORMATION, "Processor initialized");

			TRACE_EXIT();
		}

		c65_byte_t
		processor::on_read(
			__in c65_address_t address
			) const
		{
			c65_byte_t result = MEMORY_ZERO;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address.word, address.word);

			switch(address.word) {
				case ADDRESS_PROCESSOR_MASKABLE_BEGIN:
					result = m_maskable.low;
					break;
				case ADDRESS_PROCESSOR_MASKABLE_END:
					result = m_maskable.high;
					break;
				case ADDRESS_PROCESSOR_NON_MASKABLE_BEGIN:
					result = m_non_maskable.low;
					break;
				case ADDRESS_PROCESSOR_NON_MASKABLE_END:
					result = m_non_maskable.high;
					break;
				case ADDRESS_PROCESSOR_RESET_BEGIN:
					result = m_reset.low;
					break;
				case ADDRESS_PROCESSOR_RESET_END:
					result = m_reset.high;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_ADDRESS_INVALID,
						"%u(%04x)", address.word, address.word);
			}

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		processor::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Processor uninitializing");

			m_accumulator = {};
			m_index_x = {};
			m_index_y = {};
			m_interrupt = 0;
			m_maskable = {};
			m_non_maskable = {};
			m_program_counter = {};
			m_reset = {};
			m_stack_pointer = {};
			m_status = {};
			m_stop = false;
			m_wait = false;

			TRACE_MESSAGE(LEVEL_INFORMATION, "Processor uninitialized");

			TRACE_EXIT();
		}

		void
		processor::on_write(
			__in c65_address_t address,
			__in c65_byte_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address.word, address.word, value, value);

			switch(address.word) {
				case ADDRESS_PROCESSOR_MASKABLE_BEGIN:
					m_maskable.low = value;
					break;
				case ADDRESS_PROCESSOR_MASKABLE_END:
					m_maskable.high = value;
					break;
				case ADDRESS_PROCESSOR_NON_MASKABLE_BEGIN:
					m_non_maskable.low = value;
					break;
				case ADDRESS_PROCESSOR_NON_MASKABLE_END:
					m_non_maskable.high = value;
					break;
				case ADDRESS_PROCESSOR_RESET_BEGIN:
					m_reset.low = value;
					break;
				case ADDRESS_PROCESSOR_RESET_END:
					m_reset.high = value;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_ADDRESS_INVALID,
						"%u(%04x)", address.word, address.word);
			}

			TRACE_EXIT();
		}

		c65_byte_t
		processor::pop_byte(
			__in const c65::interface::bus &bus
			)
		{
			c65_byte_t result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			++m_stack_pointer.low;
			result = bus.read(m_stack_pointer);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		c65_word_t
		processor::pop_word(
			__in const c65::interface::bus &bus
			)
		{
			c65_word_t result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);


			++m_stack_pointer.low;
			result = bus.read(m_stack_pointer);
			++m_stack_pointer.low;
			result |= (bus.read(m_stack_pointer) << CHAR_BIT);

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result, result);
			return result;
		}

		void
		processor::push_byte(
			__in c65::interface::bus &bus,
			__in c65_byte_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Value=%u(%02x)", &bus, value, value);


			bus.write(m_stack_pointer, value);
			--m_stack_pointer.low;

			TRACE_EXIT();
		}

		void
		processor::push_word(
			__in c65::interface::bus &bus,
			__in c65_word_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Value=%u(%04x)", &bus, value, value);

			bus.write(m_stack_pointer, value >> CHAR_BIT);
			--m_stack_pointer.low;
			bus.write(m_stack_pointer, value);
			--m_stack_pointer.low;

			TRACE_EXIT();
		}

		c65_byte_t
		processor::read_byte(
			__in const c65::interface::bus &bus,
			__in c65_address_t address
			) const
		{
			c65_byte_t result;

			TRACE_ENTRY_FORMAT("Bus=%p, Address=%u(%04x)", &bus, address.word, address.word);

			result = bus.read(address);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		c65_word_t
		processor::read_word(
			__in const c65::interface::bus &bus,
			__in c65_address_t address
			) const
		{
			c65_word_t result;

			TRACE_ENTRY_FORMAT("Bus=%p, Address=%u(%04x)", &bus, address.word, address.word);

			result = bus.read(address);
			++address.word;
			result |= (bus.read(address) << CHAR_BIT);

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result, result);
			return result;
		}

		void
		processor::reset(
			__in c65::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Processor resetting");

			m_accumulator.low = RESET_ACCUMULATOR;
			m_index_x.low = RESET_INDEX_X;
			m_index_y.low = RESET_INDEX_Y;
			m_interrupt = 0;
			m_program_counter = m_reset;
			m_stack_pointer.word = RESET_STACK_POINTER;
			m_status.raw = RESET_STATUS;
			m_stop = false;
			m_wait = false;

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Processor reset", "%u(%04x)", m_program_counter.word, m_program_counter.word);

			TRACE_EXIT();
		}

		uint8_t
		processor::service(
			__in c65::interface::bus &bus
			)
		{
			int type = 0;
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			for(; type <= C65_INTERRUPT_MAX; ++type) {

				if(MASK_CHECK(m_interrupt, type)) {
					bool taken = false;
					c65_address_t address;

					MASK_CLEAR(m_interrupt, type);

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Processor interrupt serviced", "%i(%s)",
						type, INTERRUPT_STRING(type));

					switch(type) {
						case C65_INTERRUPT_MASKABLE:

							taken = !m_status.interrupt_disable;
							if(taken) {
								address = m_maskable;
							}
							break;
						case C65_INTERRUPT_NON_MASKABLE:
							taken = true;
							address = m_non_maskable;
							break;
						default:
							break;
					}

					if(taken) {
						m_wait = false;
						push_word(bus, m_program_counter.word);
						push_byte(bus, m_status.raw & ~MASK(FLAG_BREAK_INSTRUCTION));
						m_program_counter = address;
						result += INTERRUPT_CYCLES;
					}

					break;
				}
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::step(
			__in c65::interface::bus &bus
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if(!m_stop) {
				result += service(bus);

				if(!m_wait) {
					result += execute(bus);
				} else {
					result += execute_nop();
				}
			} else {
				result += execute_nop();
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		void
		processor::write_byte(
			__in c65::interface::bus &bus,
			__in c65_address_t address,
			__in c65_byte_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Address=%u(%04x), Value=%u(%02x)", &bus, address.word, address.word, value, value);

			bus.write(address, value);

			TRACE_EXIT();
		}

		void
		processor::write_word(
			__in c65::interface::bus &bus,
			__in c65_address_t address,
			__in c65_word_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Address=%u(%04x), Value=%u(%04x)", &bus, address.word, address.word, value, value);

			bus.write(address, value);
			++address.word;
			bus.write(address, value >> CHAR_BIT);

			TRACE_EXIT();
		}
	}
}
