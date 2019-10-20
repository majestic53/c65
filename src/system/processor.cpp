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
			m_interrupt(INTERRUPT_NONE),
			m_maskable({}),
			m_non_maskable({}),
			m_program_counter({}),
			m_reset({}),
			m_stack(STACK_NONE),
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

		c65_address_t
		processor::derive_address(
			__in c65::interface::bus &bus,
			__in int mode,
			__in c65_word_t value,
			__inout uint8_t &cycle
			)
		{
			c65_address_t result = {};

			TRACE_ENTRY_FORMAT("Bus=%p, Mode=%i(%s), Value=%u(%04x), Cycles=%u", &bus, mode, INSTRUCTION_MODE_STRING(mode),
				value, value, cycle);

			switch(mode) {
				case INSTRUCTION_MODE_ABSOLUTE: // a
				case INSTRUCTION_MODE_ABSOLUTE_INDIRECT: // (a)
					result.word = value;
					break;
				case INSTRUCTION_MODE_ABSOLUTE_INDEX_INDIRECT: // (a, x)
					result.word = (value + m_index_x.low);
					break;
				case INSTRUCTION_MODE_ABSOLUTE_INDEX_X: // a, x

					result.word = (value + m_index_x.low);
					if(result.high != (value >> CHAR_BIT)) {
						cycle += CYCLE_PAGE_CROSSED;
					}
					break;
				case INSTRUCTION_MODE_ABSOLUTE_INDEX_Y: // a, y

					result.word = (value + m_index_y.low);
					if(result.high != (value >> CHAR_BIT)) {
						cycle += CYCLE_PAGE_CROSSED;
					}
					break;
				case INSTRUCTION_MODE_RELATIVE: // r

					result.word = (m_program_counter.word + (int8_t)value);
					if(result.high != m_program_counter.high) {
						cycle += CYCLE_PAGE_CROSSED;
					}
					break;
				case INSTRUCTION_MODE_ZERO_PAGE: // zp
				case INSTRUCTION_MODE_ZERO_PAGE_INDIRECT: // (zp)
				case INSTRUCTION_MODE_ZERO_PAGE_RELATIVE: // zp, r
					result.low = value;
					break;
				case INSTRUCTION_MODE_ZERO_PAGE_INDEX_INDIRECT: // (zp, x)
				case INSTRUCTION_MODE_ZERO_PAGE_INDEX_X: // zp, x
					result.low = (value + m_index_x.low);
					break;
				case INSTRUCTION_MODE_ZERO_PAGE_INDEX_Y: // zp, y
					result.low = (value + m_index_y.low);
					break;
				case INSTRUCTION_MODE_ZERO_PAGE_INDIRECT_INDEX: // (zp), y
					result.low = value;
					value = read_word(bus, result);

					result.word = (value + m_index_y.low);
					if(result.high != (value >> CHAR_BIT)) {
						cycle += CYCLE_PAGE_CROSSED;
					}
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_MODE_INVALID,
						"%u(%s)", mode, INSTRUCTION_MODE_STRING(mode));
			}

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result.word, result.word);
			return result;
		}

		uint8_t
		processor::execute(
			__in c65::interface::bus &bus
			)
		{
			c65_address_t address;
			c65_word_t operand = 0;
			uint8_t opcode, result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			address = m_program_counter;
			opcode = read_byte(bus, m_program_counter);
			const instruction_t &instruction = INSTRUCTION(opcode);

			TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE, "Opcode", "[%04x] %s %s", m_program_counter.word, INSTRUCTION_STRING(instruction.type),
				INSTRUCTION_MODE_STRING(instruction.mode));

			++m_program_counter.word;

			switch(instruction.length) {
				case INSTRUCTION_LENGTH_BYTE:
					operand = read_byte(bus, m_program_counter);

					TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE, "Operand", "[%04x] %u(%02x)", m_program_counter.word,
						operand, operand);
					break;
				case INSTRUCTION_LENGTH_WORD:
					operand = read_word(bus, m_program_counter);

					TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE, "Operand", "[%04x] %u(%04x)", m_program_counter.word,
						operand, operand);
					break;
				default:
					break;
			}

			m_program_counter.word += instruction.length;
			result = instruction.cycle;

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
					result += execute_branch_bit(bus, instruction, operand);
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
					result += execute_branch(bus, instruction, operand);
					break;
				case INSTRUCTION_BIT:
					result += execute_bit(bus, instruction, operand);
					break;
				case INSTRUCTION_BRK:
					bus.notify(C65_EVENT_BREAKPOINT, address);
					result += execute_break(bus, instruction);
					break;
				case INSTRUCTION_CLC:
				case INSTRUCTION_CLD:
				case INSTRUCTION_CLI:
				case INSTRUCTION_CLV:
					result += execute_clear(instruction);
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
					result += execute_decrement_index(instruction);
					break;
				/*case INSTRUCTION_EOR:
					// TODO
					break;
				case INSTRUCTION_INC:
					// TODO
					break;*/
				case INSTRUCTION_INX:
				case INSTRUCTION_INY:
					result += execute_increment_index(instruction);
					break;
				case INSTRUCTION_JMP:
					result += execute_jump(bus, instruction, operand);
					break;
				case INSTRUCTION_JSR:
					result += execute_jump_subroutine(bus, instruction, operand);
					break;
				/*case INSTRUCTION_LDA:
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
					result += execute_push(bus, instruction);
					break;
				case INSTRUCTION_PLA:
				case INSTRUCTION_PLP:
				case INSTRUCTION_PLX:
				case INSTRUCTION_PLY:
					result += execute_pull(bus, instruction);
					break;
				case INSTRUCTION_RMB0 ... INSTRUCTION_RMB7:
					result += execute_reset_bit(bus, instruction, operand);
					break;
				/*case INSTRUCTION_ROL:
					// TODO
					break;
				case INSTRUCTION_ROR:
					// TODO
					break;*/
				case INSTRUCTION_RTI:
					result += execute_return_interrupt(bus, instruction);
					break;
				case INSTRUCTION_RTS:
					result += execute_return_subroutine(bus, instruction);
					break;
				/*case INSTRUCTION_SBC:
					// TODO
					break;*/
				case INSTRUCTION_SEC:
				case INSTRUCTION_SED:
				case INSTRUCTION_SEI:
					result += execute_set(instruction);
					break;
				case INSTRUCTION_SMB0 ... INSTRUCTION_SMB7:
					result += execute_set_bit(bus, instruction, operand);
					break;
				/*case INSTRUCTION_STA:
					// TODO
					break;*/
				case INSTRUCTION_STP:
					result += execute_stop(bus, instruction);
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
					result += execute_transfer(instruction);
					break;
				case INSTRUCTION_TRB:
					result += execute_test_reset_bit(bus, instruction, operand);
					break;
				case INSTRUCTION_TSB:
					result += execute_test_set_bit(bus, instruction, operand);
					break;
				case INSTRUCTION_WAI:
					result += execute_wait(bus, instruction);
					break;
				default:

					if(instruction.mode != INSTRUCTION_MODE_IMPLIED) {
						bus.notify(C65_EVENT_ILLEGAL_INSTRUCTION, address, opcode);
					}

					result += execute_no_operation(instruction);
					break;
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_bit(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction,
			__in c65_word_t operand
			)
		{
			uint8_t result = 0;
			c65_byte_t value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p, Operand=%u(%04x)", &bus, &instruction, operand, operand);

			switch(instruction.mode) {
				case INSTRUCTION_MODE_ABSOLUTE:
				case INSTRUCTION_MODE_ABSOLUTE_INDEX_X:
				case INSTRUCTION_MODE_ZERO_PAGE:
				case INSTRUCTION_MODE_ZERO_PAGE_INDEX_X:
					value = read_byte(bus, derive_address(bus, instruction.mode, operand, result));
					break;
				case INSTRUCTION_MODE_IMMEDIATE:
					value = operand;
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_MODE_INVALID,
						"%u(%s), %u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type),
						instruction.mode, INSTRUCTION_MODE_STRING(instruction.mode));
			}

			m_status.zero = !BIT_CHECK(m_accumulator.low, value);
			m_status.overflow = MASK_CHECK(value, FLAG_OVERFLOW);
			m_status.negative = MASK_CHECK(value, FLAG_NEGATIVE);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_branch(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction,
			__in c65_word_t operand
			)
		{
			uint8_t result = 0;
			bool taken = false;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p, Operand=%u(%04x)", &bus, &instruction, operand, operand);

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

			if(taken) {
				c65_address_t address;

				address = derive_address(bus, instruction.mode, operand, result);
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
			__in c65_word_t operand
			)
		{
			uint8_t result = 0;
			bool taken = false;
			c65_address_t address;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p, Operand=%u(%04x)", &bus, &instruction, operand, operand);

			address = derive_address(bus, instruction.mode, operand, result);

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

			if(taken) {
				m_program_counter.word += (int8_t)(operand >> CHAR_BIT);
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
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p", &bus, &instruction);

			service_interrupt(bus, m_maskable, true);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_clear(
			__in const instruction_t &instruction
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Instruction=%p", &instruction);

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

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_decrement_index(
			__in const instruction_t &instruction
			)
		{
			uint8_t result = 0;
			c65_byte_t value = 0;

			TRACE_ENTRY_FORMAT("Instruction=%p", &instruction);

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

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_increment_index(
			__in const instruction_t &instruction
			)
		{
			uint8_t result = 0;
			c65_byte_t value = 0;

			TRACE_ENTRY_FORMAT("Instruction=%p", &instruction);

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

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_jump(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction,
			__in c65_word_t operand
			)
		{
			uint8_t result = 0;
			c65_address_t address;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p, Operand=%u(%04x)", &bus, &instruction, operand, operand);

			address = derive_address(bus, instruction.mode, operand, result);

			switch(instruction.mode) {
				case INSTRUCTION_MODE_ABSOLUTE:
					break;
				case INSTRUCTION_MODE_ABSOLUTE_INDEX_INDIRECT:
					operand = read_word(bus, address);
					break;
				case INSTRUCTION_MODE_ABSOLUTE_INDIRECT:
					operand = read_word(bus, address);
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_MODE_INVALID,
						"%u(%s), %u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type),
						instruction.mode, INSTRUCTION_MODE_STRING(instruction.mode));
			}

			m_program_counter.word = operand;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_jump_subroutine(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction,
			__in c65_word_t operand
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p, Operand=%u(%04x)", &bus, &instruction, operand, operand);

			push_word(bus, m_program_counter.word - 1);
			m_program_counter = derive_address(bus, instruction.mode, operand, result);
			bus.notify(C65_EVENT_SUBROUTINE_ENTRY, m_program_counter);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_no_operation(void)
		{
			uint8_t result;

			TRACE_ENTRY();

			result = INSTRUCTION_MODE_CYCLE(INSTRUCTION_MODE_IMPLIED);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_no_operation(
			__in const instruction_t &instruction
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Instruction=%p", &instruction);
			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_pull(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction
			)
		{
			uint8_t result = 0;
			c65_byte_t value;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p", &bus, &instruction);

			value = pull_byte(bus);

			switch(instruction.type) {
				case INSTRUCTION_PLA:
					m_accumulator.low = value;
					break;
				case INSTRUCTION_PLP:
					m_status.raw = (value | MASK(FLAG_BREAK_INSTRUCTION));
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

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_push(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction
			)
		{
			uint8_t result = 0;
			c65_byte_t value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p", &bus, &instruction);

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

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_reset_bit(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction,
			__in c65_word_t operand
			)
		{
			c65_address_t address;
			uint8_t result = CYCLE_READ_MODIFY_WRITE;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p, Operand=%u(%04x)", &bus, &instruction, operand, operand);

			address = derive_address(bus, instruction.mode, operand, result);
			operand = read_byte(bus, address);

			switch(instruction.type) {
				case INSTRUCTION_RMB0 ... INSTRUCTION_RMB7:
					MASK_CLEAR(operand, instruction.type - INSTRUCTION_RMB0);
					write_byte(bus, address, operand);
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_return_interrupt(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p", &bus, &instruction);

			m_status.raw = (pull_byte(bus) | MASK(FLAG_BREAK_INSTRUCTION));
			m_program_counter.word = pull_word(bus);
			bus.notify(C65_EVENT_INTERRUPT_EXIT, m_program_counter);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_return_subroutine(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p", &bus, &instruction);

			m_program_counter.word = (pull_word(bus) + 1);
			bus.notify(C65_EVENT_SUBROUTINE_EXIT, m_program_counter);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_set(
			__in const instruction_t &instruction
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Instruction=%p", &instruction);

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

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_set_bit(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction,
			__in c65_word_t operand
			)
		{
			c65_address_t address;
			uint8_t result = CYCLE_READ_MODIFY_WRITE;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p, Operand=%u(%04x)", &bus, &instruction, operand, operand);

			address = derive_address(bus, instruction.mode, operand, result);
			operand = read_byte(bus, address);

			switch(instruction.type) {
				case INSTRUCTION_SMB0 ... INSTRUCTION_SMB7:
					MASK_SET(operand, instruction.type - INSTRUCTION_SMB0);
					write_byte(bus, address, operand);
					break;
				default:
					THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(C65_SYSTEM_PROCESSOR_EXCEPTION_INSTRUCTION_INVALID,
						"%u(%s)", instruction.type, INSTRUCTION_STRING(instruction.type));
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_stop(
			__in const c65::interface::bus &bus,
			__in const instruction_t &instruction
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p", &bus, &instruction);

			m_stop = true;
			bus.notify(C65_EVENT_STOP_ENTRY, m_program_counter);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_test_reset_bit(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction,
			__in c65_word_t operand
			)
		{
			c65_byte_t value = 0;
			c65_address_t address;
			uint8_t result = CYCLE_READ_MODIFY_WRITE;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p, Operand=%u(%04x)", &bus, &instruction, operand, operand);

			address = derive_address(bus, instruction.mode, operand, result);
			value = read_byte(bus, address);
			m_status.zero = BIT_CHECK(value, m_accumulator.low);
			BIT_CLEAR(value, m_accumulator.low);
			write_byte(bus, address, value);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_test_set_bit(
			__in c65::interface::bus &bus,
			__in const instruction_t &instruction,
			__in c65_word_t operand
			)
		{
			c65_byte_t value = 0;
			c65_address_t address;
			uint8_t result = CYCLE_READ_MODIFY_WRITE;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p, Operand=%u(%04x)", &bus, &instruction, operand, operand);

			address = derive_address(bus, instruction.mode, operand, result);
			value = read_byte(bus, address);
			m_status.zero = BIT_CHECK(value, m_accumulator.low);
			BIT_SET(value, m_accumulator.low);
			write_byte(bus, address, value);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_transfer(
			__in const instruction_t &instruction
			)
		{
			uint8_t result = 0;
			c65_byte_t value = 0;

			TRACE_ENTRY_FORMAT("Instruction=%p", &instruction);

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

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		processor::execute_wait(
			__in const c65::interface::bus &bus,
			__in const instruction_t &instruction
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Instruction=%p", &bus, &instruction);

			m_wait = true;
			bus.notify(C65_EVENT_WAIT_ENTRY, m_program_counter);

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

		void
		processor::on_notify(
			__in const c65_event_t &event
			) const
		{
			TRACE_ENTRY_FORMAT("Event=%p", &event);
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
			m_interrupt = INTERRUPT_NONE;
			m_maskable = {};
			m_non_maskable = {};
			m_program_counter = {};
			m_reset = {};
			m_stack = STACK_NONE;
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
			c65_byte_t previous, result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			previous = m_stack_pointer.low;
			++m_stack_pointer.low;
			result = bus.read(m_stack_pointer);

			if(previous > m_stack_pointer.low) {
				MASK_SET(m_stack, STACK_UNDERFLOW);
				bus.notify(C65_EVENT_STACK_UNDERFLOW, m_program_counter);
			}

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		c65_word_t
		processor::pull_word(
			__in const c65::interface::bus &bus
			)
		{
			c65_word_t previous, result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			previous = m_stack_pointer.low;
			++m_stack_pointer.low;
			result = bus.read(m_stack_pointer);
			++m_stack_pointer.low;
			result |= (bus.read(m_stack_pointer) << CHAR_BIT);

			if(previous > m_stack_pointer.low) {
				MASK_SET(m_stack, STACK_UNDERFLOW);
				bus.notify(C65_EVENT_STACK_UNDERFLOW, m_program_counter);
			}

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result, result);
			return result;
		}

		void
		processor::push_byte(
			__in c65::interface::bus &bus,
			__in c65_byte_t value
			)
		{
			c65_byte_t previous;

			TRACE_ENTRY_FORMAT("Bus=%p, Value=%u(%02x)", &bus, value, value);

			previous = m_stack_pointer.low;
			bus.write(m_stack_pointer, value);
			--m_stack_pointer.low;

			if(previous < m_stack_pointer.low) {
				MASK_SET(m_stack, STACK_OVERFLOW);
				bus.notify(C65_EVENT_STACK_OVERFLOW, m_program_counter);
			}

			TRACE_EXIT();
		}

		void
		processor::push_word(
			__in c65::interface::bus &bus,
			__in c65_word_t value
			)
		{
			c65_byte_t previous;

			TRACE_ENTRY_FORMAT("Bus=%p, Value=%u(%04x)", &bus, value, value);

			previous = m_stack_pointer.low;
			bus.write(m_stack_pointer, value >> CHAR_BIT);
			--m_stack_pointer.low;
			bus.write(m_stack_pointer, value);
			--m_stack_pointer.low;

			if(previous < m_stack_pointer.low) {
				MASK_SET(m_stack, STACK_OVERFLOW);
				bus.notify(C65_EVENT_STACK_OVERFLOW, m_program_counter);
			}

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
			m_interrupt = INTERRUPT_NONE;
			m_program_counter = m_reset;
			m_stack = STACK_NONE;
			m_stack_pointer.word = RESET_STACK_POINTER;
			m_status.raw = RESET_STATUS;

			if(m_stop) {
				m_stop = false;
				bus.notify(C65_EVENT_STOP_EXIT, m_program_counter);
			}

			if(m_wait) {
				m_wait = false;
				bus.notify(C65_EVENT_WAIT_EXIT, m_program_counter);
			}

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

			status.break_instruction = breakpoint;
			push_word(bus, m_program_counter.word);
			push_byte(bus, status.raw);
			m_status.interrupt_disable = true;
			m_program_counter = address;

			if(m_wait) {
				m_wait = false;
				bus.notify(C65_EVENT_WAIT_EXIT, m_program_counter);
			}

			bus.notify(C65_EVENT_INTERRUPT_ENTRY, m_program_counter);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		bool
		processor::stack_overflow(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = MASK_CHECK(m_stack, STACK_OVERFLOW);

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		bool
		processor::stack_underflow(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = MASK_CHECK(m_stack, STACK_UNDERFLOW);

			TRACE_EXIT_FORMAT("Result=%x", result);
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
					result += execute_no_operation();
				}
			} else {
				result += execute_no_operation();
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
					m_stack = STACK_NONE;
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
