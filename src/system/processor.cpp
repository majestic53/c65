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
			c65_word_t value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			const instruction_t &instruction = INSTRUCTION(read_byte(bus, m_program_counter));

			TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE, "Opcode", "[%04x] %s %s", m_program_counter.word, INSTRUCTION_STRING(instruction.type),
				INSTRUCTION_MODE_STRING(instruction.mode));

			++m_program_counter.word;

			switch(instruction.length) {
				case INSTRUCTION_LENGTH_BYTE:
					value = read_byte(bus, m_program_counter);

					TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE, "Operand", "[%04x] %u(%02x)", m_program_counter.word, value, value);
					break;
				case INSTRUCTION_LENGTH_WORD:
					value = read_word(bus, m_program_counter);

					TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE, "Operand", "[%04x] %u(%04x)", m_program_counter.word, value, value);
					break;
				default:
					break;
			}

			m_program_counter.word += instruction.length;

			switch(instruction.type) {
				/*case INSTRUCTION_ADC:
					// TODO
					break;
				case INSTRUCTION_AND:
					// TODO
					break;
				case INSTRUCTION_ASL:
					// TODO
					break;*/
				case INSTRUCTION_BBR0 ... INSTRUCTION_BBR7:
				case INSTRUCTION_BBS0 ... INSTRUCTION_BBS7:
					result = execute_branch_bit(bus, instruction, value);
					break;
				case INSTRUCTION_BCC:
				case INSTRUCTION_BCS:
				case INSTRUCTION_BEQ:
				case INSTRUCTION_BMI:
				case INSTRUCTION_BNE:
				case INSTRUCTION_BPL:
				case INSTRUCTION_BRA:
				case INSTRUCTION_BVC:
				case INSTRUCTION_BVS:
					result = execute_branch(instruction, value);
					break;
				/*case INSTRUCTION_BIT:
					// TODO
					break;*/
				case INSTRUCTION_BRK:
					result = execute_break(bus, instruction);
					break;
				case INSTRUCTION_CLC:
				case INSTRUCTION_CLD:
				case INSTRUCTION_CLI:
				case INSTRUCTION_CLV:
					result = execute_clear(instruction);
					break;
				/*case INSTRUCTION_CMP:
					// TODO
					break;
				case INSTRUCTION_CPX:
					// TODO
					break;
				case INSTRUCTION_CPY:
					// TODO
					break;
				case INSTRUCTION_DEC:
					// TODO
					break;*/
				case INSTRUCTION_DEX:
				case INSTRUCTION_DEY:
					result = execute_decrement_index(instruction);
					break;
				/*case INSTRUCTION_EOR:
					// TODO
					break;
				case INSTRUCTION_INC:
					// TODO
					break;*/
				case INSTRUCTION_INX:
				case INSTRUCTION_INY:
					result = execute_increment_index(instruction);
					break;
				/*case INSTRUCTION_JMP:
					// TODO
					break;
				case INSTRUCTION_JSR:
					// TODO
					break;
				case INSTRUCTION_LDA:
					// TODO
					break;
				case INSTRUCTION_LDX:
					// TODO
					break;
				case INSTRUCTION_LDY:
					// TODO
					break;
				case INSTRUCTION_LSR:
					// TODO
					break;
				case INSTRUCTION_ORA:
					// TODO
					break;*/
				case INSTRUCTION_PHA:
				case INSTRUCTION_PHP:
				case INSTRUCTION_PHX:
				case INSTRUCTION_PHY:
					result = execute_push(bus, instruction);
					break;
				case INSTRUCTION_PLA:
				case INSTRUCTION_PLP:
				case INSTRUCTION_PLX:
				case INSTRUCTION_PLY:
					result = execute_pull(bus, instruction);
					break;
				case INSTRUCTION_RMB0 ... INSTRUCTION_RMB7:
					result = execute_reset_bit(bus, instruction, value);
					break;
				/*case INSTRUCTION_ROL:
					// TODO
					break;
				case INSTRUCTION_ROR:
					// TODO
					break;*/
				case INSTRUCTION_RTI:
					result = execute_return_interrupt(bus, instruction);
					break;
				case INSTRUCTION_RTS:
					result = execute_return_subroutine(bus, instruction);
					break;
				/*case INSTRUCTION_SBC:
					// TODO
					break;*/
				case INSTRUCTION_SEC:
				case INSTRUCTION_SED:
				case INSTRUCTION_SEI:
					result = execute_set(instruction);
					break;
				case INSTRUCTION_SMB0 ... INSTRUCTION_SMB7:
					result = execute_set_bit(bus, instruction, value);
					break;
				/*case INSTRUCTION_STA:
					// TODO
					break;*/
				case INSTRUCTION_STP:
					result = execute_stop(instruction);
					break;
				/*case INSTRUCTION_STX:
					// TODO
					break;
				case INSTRUCTION_STY:
					// TODO
					break;
				case INSTRUCTION_STZ:
					// TODO
					break;*/
				case INSTRUCTION_TAX:
				case INSTRUCTION_TAY:
				case INSTRUCTION_TSX:
				case INSTRUCTION_TXA:
				case INSTRUCTION_TXS:
				case INSTRUCTION_TYA:
					result = execute_transfer(instruction);
					break;
				/*case INSTRUCTION_TRB:
					// TODO
					break;
				case INSTRUCTION_TSB:
					// TODO
					break;*/
				case INSTRUCTION_WAI:
					result = execute_wait(instruction);
					break;
				default:
					result = execute_no_operation(instruction);
					break;
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_branch(
			__in const instruction_t &instruction,
			__in c65_word_t value
			)
		{
			uint8_t result;
			bool taken = false;

			TRACE_ENTRY_FORMAT("Command=%p, Value=%u(%04x)", &instruction, value, value);

			switch(instruction.type) {
				case INSTRUCTION_BCC:
					taken = !m_status.carry;
					break;
				case INSTRUCTION_BCS:
					taken = m_status.carry;
					break;
				case INSTRUCTION_BEQ:
					taken = m_status.zero;
					break;
				case INSTRUCTION_BMI:
					taken = m_status.negative;
					break;
				case INSTRUCTION_BNE:
					taken = !m_status.zero;
					break;
				case INSTRUCTION_BPL:
					taken = !m_status.negative;
					break;
				case INSTRUCTION_BRA:
					taken = true;
					break;
				case INSTRUCTION_BVC:
					taken = !m_status.overflow;
					break;
				case INSTRUCTION_BVS:
					taken = m_status.overflow;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			result = instruction.cycle;

			if(taken) {
				c65_address_t address = {};
				address.word = m_program_counter.word + (int8_t)value;

				if(address.high != m_program_counter.high) {
					result += CYCLE_PAGE_CROSSED;
				}

				result += CYCLE_BRANCH_TAKEN;
				m_program_counter.word = address.word;
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_branch_bit(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction,
			__in c65_word_t value
			)
		{
			uint8_t result;
			bool taken = false;
			c65_address_t address;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%p, Value=%u(%04x)", &bus, &instruction, value, value);

			address.word = (value & UINT8_MAX);

			switch(instruction.type) {
				case INSTRUCTION_BBR0 ... INSTRUCTION_BBR7:
					taken = !MASK_CHECK(read_byte(bus, address), instruction.type - INSTRUCTION_BBR0);
					break;
				case INSTRUCTION_BBS0 ... INSTRUCTION_BBS7:
					taken = MASK_CHECK(read_byte(bus, address), instruction.type - INSTRUCTION_BBS0);
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			result = instruction.cycle;

			if(taken) {
				m_program_counter.word += (int8_t)(value >> CHAR_BIT);
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_break(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%p", &bus, &instruction);

			result = service_interrupt(bus, m_maskable, true);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_clear(
			__in const instruction_t &instruction
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Command=%p", &instruction);

			switch(instruction.type) {
				case INSTRUCTION_CLC:
					m_status.carry = false;
					break;
				case INSTRUCTION_CLD:
					m_status.decimal_mode = false;
					break;
				case INSTRUCTION_CLI:
					m_status.interrupt_disable = false;
					break;
				case INSTRUCTION_CLV:
					m_status.overflow = false;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			result = instruction.cycle;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_decrement_index(
			__in const instruction_t &instruction
			)
		{
			uint8_t result;
			c65_byte_t value = 0;

			TRACE_ENTRY_FORMAT("Command=%p", &instruction);

			switch(instruction.type) {
				case INSTRUCTION_DEX:
					value = --m_index_x.low;
					break;
				case INSTRUCTION_DEY:
					value = --m_index_y.low;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			m_status.negative = MASK_CHECK(value, CHAR_BIT - 1);
			m_status.zero = !value;
			result = instruction.cycle;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_increment_index(
			__in const instruction_t &instruction
			)
		{
			uint8_t result;
			c65_byte_t value = 0;

			TRACE_ENTRY_FORMAT("Command=%p", &instruction);

			switch(instruction.type) {
				case INSTRUCTION_INX:
					value = ++m_index_x.low;
					break;
				case INSTRUCTION_INY:
					value = ++m_index_y.low;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			m_status.negative = MASK_CHECK(value, CHAR_BIT - 1);
			m_status.zero = !value;
			result = instruction.cycle;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_no_operation(
			__in const instruction_t &instruction
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Command=%p", &instruction);

			result = instruction.cycle;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_pull(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction
			)
		{
			uint8_t result;
			c65_byte_t value;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%p", &bus, &instruction);

			value = pull_byte(bus);

			switch(instruction.type) {
				case INSTRUCTION_PLA:
					m_accumulator.low = value;
					break;
				case INSTRUCTION_PLP:
					m_status.raw = value;
					break;
				case INSTRUCTION_PLX:
					m_index_x.low = value;
					break;
				case INSTRUCTION_PLY:
					m_index_y.low = value;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			if(instruction.type != INSTRUCTION_PLP) {
				m_status.negative = MASK_CHECK(value, CHAR_BIT - 1);
				m_status.zero = !value;
			}

			result = instruction.cycle;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_push(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction
			)
		{
			uint8_t result;
			c65_byte_t value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%p", &bus, &instruction);

			switch(instruction.type) {
				case INSTRUCTION_PHA:
					value = m_accumulator.low;
					break;
				case INSTRUCTION_PHP:
					value = m_status.raw;
					break;
				case INSTRUCTION_PHX:
					value = m_index_x.low;
					break;
				case INSTRUCTION_PHY:
					value = m_index_y.low;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			push_byte(bus, value);
			result = instruction.cycle;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_reset_bit(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction,
			__in c65_word_t value
			)
		{
			uint8_t result = 0;
			c65_address_t address;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%p, Value=%u(%04x)", &bus, &instruction, value, value);

			address.word = (value & UINT8_MAX);
			value = read_byte(bus, address);

			switch(instruction.type) {
				case INSTRUCTION_RMB0 ... INSTRUCTION_RMB7:
					MASK_CLEAR(value, instruction.type - INSTRUCTION_RMB0);
					write_byte(bus, address, value);
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			result = (instruction.cycle + CYCLE_READ_MODIFY_WRITE);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_return_interrupt(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%p", &bus, &instruction);

			m_status.raw = pull_byte(bus);
			m_program_counter.word = pull_word(bus);

			result = instruction.cycle;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_return_subroutine(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%p", &bus, &instruction);

			m_program_counter.word = (pull_word(bus) + 1);

			result = instruction.cycle;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_set(
			__in const instruction_t &instruction
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Command=%p", &instruction);

			switch(instruction.type) {
				case INSTRUCTION_SEC:
					m_status.carry = true;
					break;
				case INSTRUCTION_SED:
					m_status.decimal_mode = true;
					break;
				case INSTRUCTION_SEI:
					m_status.interrupt_disable = true;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			result = instruction.cycle;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_set_bit(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction,
			__in c65_word_t value
			)
		{
			uint8_t result = 0;
			c65_address_t address;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%p, Value=%u(%04x)", &bus, &instruction, value, value);

			address.word = (value & UINT8_MAX);
			value = read_byte(bus, address);

			switch(instruction.type) {
				case INSTRUCTION_SMB0 ... INSTRUCTION_SMB7:
					MASK_SET(value, instruction.type - INSTRUCTION_SMB0);
					write_byte(bus, address, value);
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			result = (instruction.cycle + CYCLE_READ_MODIFY_WRITE);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_transfer(
			__in const instruction_t &instruction
			)
		{
			c65_byte_t value;
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Command=%p", &instruction);

			switch(instruction.type) {
				case INSTRUCTION_TAX:
					value = m_accumulator.low;
					m_index_x.low = value;
					break;
				case INSTRUCTION_TAY:
					value = m_accumulator.low;
					m_index_y.low = value;
					break;
				case INSTRUCTION_TSX:
					value = m_stack_pointer.low;
					m_index_x.low = value;
					break;
				case INSTRUCTION_TXA:
					value = m_index_x.low;
					m_accumulator.low = value;
					break;
				case INSTRUCTION_TXS:
					value = m_index_x.low;
					m_stack_pointer.low = value;
					break;
				case INSTRUCTION_TYA:
					value = m_index_y.low;
					m_accumulator.low = value;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			if(instruction.type != INSTRUCTION_TXS) {
				m_status.negative = MASK_CHECK(value, CHAR_BIT - 1);
				m_status.zero = !value;
			}

			result = instruction.cycle;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_stop(
			__in const instruction_t &instruction
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Command=%p", &instruction);

			m_stop = true;
			result = instruction.cycle;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_wait(
			__in const instruction_t &instruction
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Command=%p", &instruction);

			m_wait = true;
			result = instruction.cycle;

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

		bool
		processor::interrupt_pending(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = (m_interrupt != 0);

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		processor::on_initialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Processor initializing");

			m_maskable.word = DEFAULT_INTERRUPT_MASKABLE;
			m_non_maskable.word = DEFAULT_INTERRUPT_NON_MASKABLE;
			m_reset.word = DEFAULT_INTERRUPT_RESET;

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
		processor::pull_byte(
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
		processor::pull_word(
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

		c65_register_t
		processor::read_register(
			__in int type
			) const
		{
			c65_register_t result;

			TRACE_ENTRY_FORMAT("Type=%i(%s)", type, REGISTER_STRING(type));

			switch(type) {
				case C65_REGISTER_ACCUMULATOR:
					result = m_accumulator;
					break;
				case C65_REGISTER_INDEX_X:
					result = m_index_x;
					break;
				case C65_REGISTER_INDEX_Y:
					result = m_index_y;
					break;
				case C65_REGISTER_PROGRAM_COUNTER:
					result = m_program_counter;
					break;
				case C65_REGISTER_STACK_POINTER:
					result = m_stack_pointer;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_REGISTER_INVALID,
						"%i(%s)", type, REGISTER_STRING(type));
			}

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result.word, result.word);
			return result;
		}

		c65_status_t
		processor::read_status(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%u(%02x)", m_status.raw);
			return m_status;
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

			m_accumulator = {};
			m_index_x = {};
			m_index_y = {};
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
						MASK_CLEAR(m_interrupt, type);
						result += service_interrupt(bus, address, false);
					}

					break;
				}
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::service_interrupt(
			__in c65::interface::bus &bus,
			__in c65_address_t address,
			__in bool breakpoint
			)
		{
			c65_status_t status = m_status;
			uint8_t result = CYCLE_INTERRUPT;

			TRACE_ENTRY_FORMAT("Bus=%p, Address=%u(%04x), Breakpoint=%x", &bus, address.word, address.word, breakpoint);

			m_wait = false;
			status.break_instruction = breakpoint;
			push_word(bus, m_program_counter.word);
			push_byte(bus, status.raw);
			m_status.interrupt_disable = true;
			m_program_counter = address;

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
					result += execute_no_operation(INSTRUCTION(INSTRUCTION_TYPE_NOP_IMPLIED));
				}
			} else {
				result += execute_no_operation(INSTRUCTION(INSTRUCTION_TYPE_NOP_IMPLIED));
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		bool
		processor::stopped(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%x", m_stop);
			return m_stop;
		}

		bool
		processor::waiting(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%x", m_wait);
			return m_wait;
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
		processor::write_register(
			__in int type,
			__in c65_register_t value
			)
		{
			TRACE_ENTRY_FORMAT("Type=%i(%s), Value=%u(%04x)", type, REGISTER_STRING(type), value.word, value.word);

			switch(type) {
				case C65_REGISTER_ACCUMULATOR:
					m_accumulator.low = value.low;
					break;
				case C65_REGISTER_INDEX_X:
					m_index_x.low = value.low;
					break;
				case C65_REGISTER_INDEX_Y:
					m_index_y.low = value.low;
					break;
				case C65_REGISTER_PROGRAM_COUNTER:
					m_program_counter = value;
					break;
				case C65_REGISTER_STACK_POINTER:
					m_stack_pointer.low = value.low;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_REGISTER_INVALID,
						"%i(%s)", type, REGISTER_STRING(type));
			}

			TRACE_EXIT();
		}

		void
		processor::write_status(
			__in c65_status_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value.raw, value.raw);

			m_status = value;

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
