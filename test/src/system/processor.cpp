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

#include "../../../include/system/processor.h"
#include "../../include/system/processor.h"
#include "./processor_type.h"

namespace c65 {

	namespace test {

		namespace system {

			processor::processor(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			processor::~processor(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			void
			processor::clear_interrupts(void)
			{
				int type = 0;
				c65_word_t *interrupt;

				TRACE_ENTRY();

				interrupt = (c65_word_t *)&m_memory[ADDRESS_PROCESSOR_NON_MASKABLE_BEGIN];

				for(; type <= INTERRUPT_VECTOR_MAX; ++type) {
					interrupt[type] = INTERRUPT_VECTOR_ADDRESS(type);
				}

				TRACE_EXIT();
			}

			void
			processor::clear_memory(void)
			{
				TRACE_ENTRY();

				m_memory.resize(UINT16_MAX + 1, MEMORY_FILL);

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
				c65_byte_t result;

				TRACE_ENTRY_FORMAT("Address=%u(%04x)", address.word, address.word);

				result = m_memory.at(address.word);

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			void
			processor::on_run(
				__in bool quiet
				)
			{
				TRACE_ENTRY_FORMAT("Quiet=%x", quiet);

				EXECUTE_TEST(test_execute_add, quiet);
				EXECUTE_TEST(test_execute_and, quiet);
				EXECUTE_TEST(test_execute_bit, quiet);
				EXECUTE_TEST(test_execute_branch, quiet);
				EXECUTE_TEST(test_execute_branch_bit, quiet);
				EXECUTE_TEST(test_execute_break, quiet);
				EXECUTE_TEST(test_execute_clear, quiet);
				EXECUTE_TEST(test_execute_compare, quiet);
				EXECUTE_TEST(test_execute_compare_index_x, quiet);
				EXECUTE_TEST(test_execute_compare_index_y, quiet);
				EXECUTE_TEST(test_execute_decrement, quiet);
				EXECUTE_TEST(test_execute_decrement_index, quiet);
				EXECUTE_TEST(test_execute_increment, quiet);
				EXECUTE_TEST(test_execute_increment_index, quiet);
				EXECUTE_TEST(test_execute_jump, quiet);
				EXECUTE_TEST(test_execute_jump_subroutine, quiet);
				EXECUTE_TEST(test_execute_load_accumulator, quiet);
				EXECUTE_TEST(test_execute_load_index_x, quiet);
				EXECUTE_TEST(test_execute_load_index_y, quiet);
				EXECUTE_TEST(test_execute_no_operation, quiet);
				EXECUTE_TEST(test_execute_or, quiet);
				EXECUTE_TEST(test_execute_pull, quiet);
				EXECUTE_TEST(test_execute_push, quiet);
				EXECUTE_TEST(test_execute_reset_bit, quiet);
				EXECUTE_TEST(test_execute_return_interrupt, quiet);
				EXECUTE_TEST(test_execute_return_subroutine, quiet);
				EXECUTE_TEST(test_execute_rotate_left, quiet);
				EXECUTE_TEST(test_execute_rotate_right, quiet);
				EXECUTE_TEST(test_execute_set, quiet);
				EXECUTE_TEST(test_execute_set_bit, quiet);
				EXECUTE_TEST(test_execute_shift_left, quiet);
				EXECUTE_TEST(test_execute_shift_right, quiet);
				EXECUTE_TEST(test_execute_stop, quiet);
				EXECUTE_TEST(test_execute_store_accumulator, quiet);
				EXECUTE_TEST(test_execute_store_index_x, quiet);
				EXECUTE_TEST(test_execute_store_index_y, quiet);
				EXECUTE_TEST(test_execute_store_zero, quiet);
				EXECUTE_TEST(test_execute_subtract, quiet);
				EXECUTE_TEST(test_execute_test_reset_bit, quiet);
				EXECUTE_TEST(test_execute_test_set_bit, quiet);
				EXECUTE_TEST(test_execute_transfer, quiet);
				EXECUTE_TEST(test_execute_wait, quiet);
				EXECUTE_TEST(test_execute_xor, quiet);
				EXECUTE_TEST(test_interrupt, quiet);
				EXECUTE_TEST(test_interrupt_pending, quiet);
				EXECUTE_TEST(test_read, quiet);
				EXECUTE_TEST(test_read_register, quiet);
				EXECUTE_TEST(test_read_status, quiet);
				EXECUTE_TEST(test_reset, quiet);
				EXECUTE_TEST(test_stack_overflow, quiet);
				EXECUTE_TEST(test_stack_underflow, quiet);
				EXECUTE_TEST(test_step, quiet);
				EXECUTE_TEST(test_stopped, quiet);
				EXECUTE_TEST(test_waiting, quiet);
				EXECUTE_TEST(test_write, quiet);
				EXECUTE_TEST(test_write_register, quiet);
				EXECUTE_TEST(test_write_status, quiet);

				TRACE_EXIT();
			}

			void
			processor::on_setup(void)
			{
				TRACE_ENTRY();

				clear_memory();
				clear_interrupts();

				TRACE_EXIT();
			}

			void
			processor::on_teardown(void)
			{
				TRACE_ENTRY();

				m_memory.clear();

				TRACE_EXIT();
			}

			void
			processor::on_write(
				__in c65_address_t address,
				__in c65_byte_t value
				)
			{
				TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address.word, address.word, value, value);

				m_memory.at(address.word) = value;

				TRACE_EXIT();
			}

			void
			processor::save_state(
				__in processor_state_t &state
				)
			{
				TRACE_ENTRY_FORMAT("State=%p", &state);

				const c65::system::processor &instance = c65::system::processor::instance();

				state.accumulator = instance.read_register(C65_REGISTER_ACCUMULATOR);
				state.index_x = instance.read_register(C65_REGISTER_INDEX_X);
				state.index_y = instance.read_register(C65_REGISTER_INDEX_Y);
				state.program_counter = instance.read_register(C65_REGISTER_PROGRAM_COUNTER);
				state.stack_pointer = instance.read_register(C65_REGISTER_STACK_POINTER);
				state.status = instance.read_status();

				TRACE_EXIT();
			}

			void
			processor::test_execute_add(void)
			{
				TRACE_ENTRY();

				// TODO

				TRACE_EXIT();
			}

			void
			processor::test_execute_and(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				// Test 1.a: AND a, positive
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_AND_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_AND_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word & 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test 1.b; AND a, zero
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x00, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_AND_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_AND_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word & 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test 1.c: AND a, negative
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0xff, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0xf0;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_AND_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_AND_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word & 0xf0));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #2: AND a, x
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_AND_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_AND_ABSOLUTE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word & 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #3: AND a, y
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_AND_ABSOLUTE_INDEX_Y;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_AND_ABSOLUTE_INDEX_Y);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word & 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #4: AND imm
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_AND_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x0f;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_AND_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word & 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #5: AND zp
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_AND_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_AND_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word & 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #6: AND (zp, x)
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00ac;
				m_memory.at(address.word) = 0xbb;
				m_memory.at(address.word + 1) = 0xaa;
				address.word = 0xaabb;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_AND_ZERO_PAGE_INDEX_INDIRECT;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_AND_ZERO_PAGE_INDEX_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word & 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #7: AND zp, x
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00ac;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_AND_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_AND_ZERO_PAGE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word & 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #8: AND (zp)
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0xbb;
				m_memory.at(address.word + 1) = 0xaa;
				address.word = 0xaabb;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_AND_ZERO_PAGE_INDIRECT;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_AND_ZERO_PAGE_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word & 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #9: AND (zp), y
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0xbb;
				m_memory.at(address.word + 1) = 0xaa;
				address.word = 0xaabd;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_AND_ZERO_PAGE_INDIRECT_INDEX;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_AND_ZERO_PAGE_INDIRECT_INDEX);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word & 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				TRACE_EXIT();
			}

			void
			processor::test_execute_bit(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				// Test #1.a: BIT a, not found
				instance.initialize();
				instance.reset(*this);
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x00;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_BIT_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BIT_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #1.b: BIT a, found
				instance.initialize();
				instance.reset(*this);
				address.word = 0x80;
				instance.write_register(C65_REGISTER_ACCUMULATOR, address);
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0xc0;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_BIT_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BIT_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw
					== (state.status.raw | MASK(FLAG_NEGATIVE) | MASK(FLAG_OVERFLOW) | MASK(FLAG_ZERO)));

				// Test #2.a: BIT a, x, not found
				instance.initialize();
				instance.reset(*this);
				address.word = 0x02;
				instance.write_register(C65_REGISTER_INDEX_X, address);
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x00;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_BIT_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BIT_ABSOLUTE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2.a: BIT a, x, not found, page cross
				instance.initialize();
				instance.reset(*this);
				address.word = 0xaa;
				instance.write_register(C65_REGISTER_INDEX_X, address);
				save_state(state);

				address.word = 0x1154;
				m_memory.at(address.word) = 0x00;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_BIT_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BIT_ABSOLUTE_INDEX_X);
				ASSERT(instance.step(*this) == (instruction.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2.b: BIT a, x, found
				instance.initialize();
				instance.reset(*this);
				address.word = 0x80;
				instance.write_register(C65_REGISTER_ACCUMULATOR, address);
				address.word = 0x02;
				instance.write_register(C65_REGISTER_INDEX_X, address);
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0xc0;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_BIT_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BIT_ABSOLUTE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw
					== (state.status.raw | MASK(FLAG_NEGATIVE) | MASK(FLAG_OVERFLOW) | MASK(FLAG_ZERO)));

				// Test #3.a: BIT imm, not found
				instance.initialize();
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_BIT_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x00;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BIT_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #3.b: BIT imm, found
				instance.initialize();
				instance.reset(*this);
				address.word = 0x80;
				instance.write_register(C65_REGISTER_ACCUMULATOR, address);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_BIT_IMMEDIATE;
				m_memory.at(address.word + 1) = 0xc0;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BIT_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw
					== (state.status.raw | MASK(FLAG_NEGATIVE) | MASK(FLAG_OVERFLOW) | MASK(FLAG_ZERO)));

				// Test #4.a: BIT zp, not found
				instance.initialize();
				instance.reset(*this);
				save_state(state);

				address.word = 0xaa;
				m_memory.at(address.word) = 0x00;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_BIT_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BIT_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #4.b: BIT zp, found
				instance.initialize();
				instance.reset(*this);
				address.word = 0x80;
				instance.write_register(C65_REGISTER_ACCUMULATOR, address);
				save_state(state);

				address.word = 0xaa;
				m_memory.at(address.word) = 0xc0;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_BIT_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BIT_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw
					== (state.status.raw | MASK(FLAG_NEGATIVE) | MASK(FLAG_OVERFLOW) | MASK(FLAG_ZERO)));

				// Test #5.a: BIT zp, x, not found
				instance.initialize();
				instance.reset(*this);
				address.word = 0x02;
				instance.write_register(C65_REGISTER_INDEX_X, address);
				save_state(state);

				address.word = 0xac;
				m_memory.at(address.word) = 0x00;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_BIT_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BIT_ZERO_PAGE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #5.b: BIT zp, x, found
				instance.initialize();
				instance.reset(*this);
				address.word = 0x80;
				instance.write_register(C65_REGISTER_ACCUMULATOR, address);
				address.word = 0x02;
				instance.write_register(C65_REGISTER_INDEX_X, address);
				save_state(state);

				address.word = 0xac;
				m_memory.at(address.word) = 0xc0;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_BIT_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BIT_ZERO_PAGE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw
					== (state.status.raw | MASK(FLAG_NEGATIVE) | MASK(FLAG_OVERFLOW) | MASK(FLAG_ZERO)));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_branch(void)
			{
				c65_status_t status;
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1.a: BCC r, not taken
				instance.reset(*this);
				status = {};
				status.carry = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BCC_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BCC_RELATIVE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #1.b: BCC r, taken forward
				instance.reset(*this);
				status = {};
				status.carry = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BCC_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BCC_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #1.c: BCC r, taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.carry = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BCC_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BCC_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2.a: BCS r, not taken
				instance.reset(*this);
				status = {};
				status.carry = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BCS_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BCS_RELATIVE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2.b: BCS r, taken forward
				instance.reset(*this);
				status = {};
				status.carry = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BCS_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BCS_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2.c: BCS r, taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.carry = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BCS_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BCS_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #3.a: BEQ r, not taken
				instance.reset(*this);
				status = {};
				status.zero = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BEQ_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BEQ_RELATIVE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #3.b: BEQ r, taken forward
				instance.reset(*this);
				status = {};
				status.zero = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BEQ_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BEQ_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #3.c: BEQ r, taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.zero = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BEQ_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BEQ_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #4.a: BMI r, not taken
				instance.reset(*this);
				status = {};
				status.negative = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BMI_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BMI_RELATIVE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #4.b: BMI r, taken forward
				instance.reset(*this);
				status = {};
				status.negative = true;
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = INSTRUCTION_TYPE_BMI_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				instruction = INSTRUCTION(INSTRUCTION_TYPE_BMI_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #4.c: BMI r, taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.negative = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BMI_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BMI_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #5.a: BNE r, not taken
				instance.reset(*this);
				status = {};
				status.zero = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BNE_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BNE_RELATIVE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #5.b: BNE r, taken forward
				instance.reset(*this);
				status = {};
				status.zero = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BNE_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BNE_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #5.c: BNE r, taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.zero = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BNE_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BNE_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #6.a: BPL r, not taken
				instance.reset(*this);
				status = {};
				status.negative = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BPL_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BPL_RELATIVE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #6.b: BPL r, taken forward
				instance.reset(*this);
				status = {};
				status.negative = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BPL_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BPL_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #6.c: BPL r, taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.negative = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BPL_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BPL_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #7.a: BVC r, not taken
				instance.reset(*this);
				status = {};
				status.overflow = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BVC_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BVC_RELATIVE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #7.b: BVC r, taken forward
				instance.reset(*this);
				status = {};
				status.overflow = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BVC_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BVC_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #7.c: BVC r, taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.overflow = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BVC_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BVC_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #8.a: BVS r, not taken
				instance.reset(*this);
				status = {};
				status.overflow = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BVS_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BVS_RELATIVE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #8.b: BVS r, taken forward
				instance.reset(*this);
				status = {};
				status.overflow = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BVS_RELATIVE;
				m_memory.at(address.word + 1) = 10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BVS_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #8.c: BVS r, taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.overflow = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BVS_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BVS_RELATIVE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_branch_bit(void)
			{
				int type;
				c65_byte_t index;
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: BBR zp, r
				for(type = INSTRUCTION_BBR0; type <= INSTRUCTION_BBR7; ++type) {

					// Test #1.a: BBR zp, r, bit unset
					instance.reset(*this);
					save_state(state);

					index = (INSTRUCTION_TYPE_BBR0_ZERO_PAGE_RELATIVE + ((type - INSTRUCTION_BBR0) * 0x10));
					address.word = 0xaa;
					m_memory.at(address.word) = 0;
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;
					m_memory.at(address.word + 2) = 0x02;

					instruction = INSTRUCTION(index);
					ASSERT(instance.step(*this) == instruction.cycle);
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + 5));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);

					// Test #1.b: BBR zp, r, bit set
					instance.reset(*this);
					save_state(state);

					index = (INSTRUCTION_TYPE_BBR0_ZERO_PAGE_RELATIVE + ((type - INSTRUCTION_BBR0) * 0x10));
					address.word = 0xaa;
					m_memory.at(address.word) = MASK(type - INSTRUCTION_BBR0);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;
					m_memory.at(address.word + 2) = 0x02;

					instruction = INSTRUCTION(index);
					ASSERT(instance.step(*this) == instruction.cycle);
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + 3));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
				}

				// Test #2: BBS zp, r
				for(type = INSTRUCTION_BBS0; type <= INSTRUCTION_BBS7; ++type) {

					// Test #2.a: BBS zp, r, bit unset
					instance.reset(*this);
					save_state(state);

					index = (INSTRUCTION_TYPE_BBS0_ZERO_PAGE_RELATIVE + ((type - INSTRUCTION_BBS0) * 0x10));
					address.word = 0xaa;
					m_memory.at(address.word) = 0;
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;
					m_memory.at(address.word + 2) = 0x02;

					instruction = INSTRUCTION(index);
					ASSERT(instance.step(*this) == instruction.cycle);
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + 3));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);

					// Test #2.b: BBS zp, r, bit set
					instance.reset(*this);
					save_state(state);

					index = (INSTRUCTION_TYPE_BBS0_ZERO_PAGE_RELATIVE + ((type - INSTRUCTION_BBS0) * 0x10));
					address.word = 0xaa;
					m_memory.at(address.word) = MASK(type - INSTRUCTION_BBS0);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;
					m_memory.at(address.word + 2) = 0x02;

					instruction = INSTRUCTION(index);
					ASSERT(instance.step(*this) == instruction.cycle);
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + 5));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
				}

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_break(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: BRK 0
				instance.reset(*this);
				address.word = ADDRESS_PROCESSOR_MASKABLE_BEGIN;
				instance.write(address, INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_MASKABLE));
				address.word = ADDRESS_PROCESSOR_MASKABLE_END;
				instance.write(address, INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_MASKABLE) >> CHAR_BIT);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				state.status = instance.read_status();
				state.status.break_instruction = false;
				instance.write_status(state.status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_BRK_INTERRUPT;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_BRK_INTERRUPT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word
					== INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_MASKABLE));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word - 3));
				state.status.break_instruction = true;
				ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END - 2) == state.status.raw);
				ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END - 1) == (uint8_t)(state.program_counter.low + instruction.length + 1));
				ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END) == (uint8_t)(state.program_counter.high >> CHAR_BIT));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_clear(void)
			{
				c65_status_t status;
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1: CLC i
				instance.reset(*this);
				status = {};
				status.carry = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_CLC_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CLC_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(!instance.read_status().carry);

				// Test #2: CLD i
				instance.reset(*this);
				status = {};
				status.decimal_mode = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_CLD_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CLD_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(!instance.read_status().decimal_mode);

				// Test #3: CLI i
				instance.reset(*this);
				status = {};
				status.interrupt_disable = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_CLI_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CLI_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(!instance.read_status().interrupt_disable);

				// Test #4: CLV i
				instance.reset(*this);
				status = {};
				status.overflow = true;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_CLV_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CLV_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(!instance.read_status().overflow);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_compare(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				// Test #1.a: CMP a, positive
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x01;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CMP_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CMP_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #1.b: CMP a, zero
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x02;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CMP_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CMP_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test #1.c: CMP a, negative
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CMP_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CMP_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_CARRY) | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #2: CMP a, x
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x01;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CMP_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CMP_ABSOLUTE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #3: CMP a, y
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x01;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CMP_ABSOLUTE_INDEX_Y;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CMP_ABSOLUTE_INDEX_Y);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #4: CMP imm
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CMP_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x01;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CMP_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #5: CMP zp
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0x01;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CMP_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CMP_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #6: CMP (zp, x)
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00ac;
				m_memory.at(address.word) = 0x00;
				m_memory.at(address.word + 1) = 0x10;
				address.word = 0x1000;
				m_memory.at(address.word) = 0x01;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CMP_ZERO_PAGE_INDEX_INDIRECT;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CMP_ZERO_PAGE_INDEX_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #7: CMP zp, x
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00ac;
				m_memory.at(address.word) = 0x01;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CMP_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CMP_ZERO_PAGE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #8: CMP (zp)
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0x00;
				m_memory.at(address.word + 1) = 0x10;
				address.word = 0x1000;
				m_memory.at(address.word) = 0x01;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CMP_ZERO_PAGE_INDIRECT;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CMP_ZERO_PAGE_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #9: CMP (zp), y
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0x00;
				m_memory.at(address.word + 1) = 0x10;
				address.word = 0x1002;
				m_memory.at(address.word) = 0x01;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CMP_ZERO_PAGE_INDIRECT_INDEX;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CMP_ZERO_PAGE_INDIRECT_INDEX);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				TRACE_EXIT();
			}

			void
			processor::test_execute_compare_index_x(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				// Test #1.a: CPX a, positive
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x01;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CPX_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CPX_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #1.b: CPX a, zero
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x02;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CPX_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CPX_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test #1.c: CPX a, negative
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CPX_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CPX_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_CARRY) | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #4: CPX imm
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CPX_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x01;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CPX_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #5: CPX zp
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0x01;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CPX_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CPX_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				TRACE_EXIT();
			}

			void
			processor::test_execute_compare_index_y(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				// Test #1.a: CPY a, positive
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x01;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CPY_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CPY_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #1.b: CPY a, zero
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x02;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CPY_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CPY_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test #1.c: CPY a, negative
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CPY_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CPY_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_CARRY) | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #4: CPY imm
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CPY_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x01;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CPY_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #5: CPY zp
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0x01;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_CPY_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_CPY_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_CARRY) & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				TRACE_EXIT();
			}

			void
			processor::test_execute_decrement(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1.a: DEC a, non-zero, positive
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_DEC_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;
				m_memory.at(0x1000) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEC_ABSOLUTE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));
				ASSERT(m_memory.at(0x1000) == 0x0f);

				// Test #1.b: DEC a, zero
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_DEC_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;
				m_memory.at(0x1000) = 0x01;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEC_ABSOLUTE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));
				ASSERT(m_memory.at(0x1000) == 0x00);

				// Test #1.c: DEC a, negative
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_DEC_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;
				m_memory.at(0x1000) = 0x00;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEC_ABSOLUTE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));
				ASSERT(m_memory.at(0x1000) == 0xff);

				// Test #2: DEC a, x
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_DEC_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;
				m_memory.at(0x1002) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEC_ABSOLUTE_INDEX_X);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));
				ASSERT(m_memory.at(0x1002) == 0x0f);

				// Test #3: DEC acc
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x10, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_DEC_ACCUMULATOR;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEC_ACCUMULATOR);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #4: DEC zp
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_DEC_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;
				m_memory.at(0x00aa) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEC_ZERO_PAGE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));
				ASSERT(m_memory.at(0x00aa) == 0x0f);

				// Test #5: DEC zp, x
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_DEC_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;
				m_memory.at(0x00ac) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEC_ZERO_PAGE_INDEX_X);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));
				ASSERT(m_memory.at(0x00ac) == 0x0f);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_decrement_index(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1: DEX i, index-x > 1
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{10, 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_DEX_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEX_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == (state.index_x.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2: DEX i, index-x = 1
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{1, 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_DEX_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEX_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == (state.index_x.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				// Test #3: DEX i, index-x = 0
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0, 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_DEX_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEX_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == (uint8_t)(state.index_x.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				// Test #4: DEY i, index-y > 1
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{10, 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_DEY_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEY_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == (state.index_y.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #5: DEY i, index-y = 1
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{1, 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_DEY_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEY_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == (state.index_y.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				// Test #6: DEY i, index-y = 0
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0, 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_DEY_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_DEY_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == (uint8_t)(state.index_y.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_increment(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1.a: INC a, non-zero, positive
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_INC_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;
				m_memory.at(0x1000) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INC_ABSOLUTE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));
				ASSERT(m_memory.at(0x1000) == 0x11);

				// Test #1.b: INC a, zero
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_INC_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;
				m_memory.at(0x1000) = 0xff;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INC_ABSOLUTE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));
				ASSERT(m_memory.at(0x1000) == 0x00);

				// Test #1.c: INC a, negative
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_INC_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;
				m_memory.at(0x1000) = 0xfe;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INC_ABSOLUTE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));
				ASSERT(m_memory.at(0x1000) == 0xff);

				// Test #2: INC a, x
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_INC_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;
				m_memory.at(0x1002) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INC_ABSOLUTE_INDEX_X);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));
				ASSERT(m_memory.at(0x1002) == 0x11);

				// Test #3: INC acc
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x10, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_INC_ACCUMULATOR;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INC_ACCUMULATOR);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #4: INC zp
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_INC_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;
				m_memory.at(0x00aa) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INC_ZERO_PAGE);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));
				ASSERT(m_memory.at(0x00aa) == 0x11);

				// Test #5: INC zp, x
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_INC_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;
				m_memory.at(0x00ac) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INC_ZERO_PAGE_INDEX_X);
				ASSERT(instance.step(*this) == (instruction.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));
				ASSERT(m_memory.at(0x00ac) == 0x11);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_increment_index(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1: INX i, index-x < 127
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{10, 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_INX_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INX_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == (state.index_x.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2: INX i, index-x = 127
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x7f, 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_INX_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INX_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == (state.index_x.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				// Test #3: INX i, index-x = 255
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0xff, 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_INX_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INX_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == (uint8_t)(state.index_x.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				// Test #4: INY i, index-y < 127
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{10, 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_INY_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INY_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == (state.index_y.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #5: INY i,  index-y = 127
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x7f, 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_INY_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INY_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == (state.index_y.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				// Test #6: INY i, index-y = 255
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0xff, 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_INY_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_INY_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == (uint8_t)(state.index_y.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_jump(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: JMP a
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_JMP_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_JMP_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == 0x1000);
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2: JMP (a, x)
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0xaf, 0}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_JMP_ABSOLUTE_INDEX_INDIRECT;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;
				address.word = 0x10af;
				m_memory.at(address.word) = 0xbb;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_JMP_ABSOLUTE_INDEX_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == 0xaabb);
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #3: JMP (a)
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_JMP_ABSOLUTE_INDIRECT;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;
				address.word = 0x1000;
				m_memory.at(address.word) = 0xbb;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_JMP_ABSOLUTE_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == 0xaabb);
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_jump_subroutine(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: JSR a
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_JSR_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_JSR_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == 0x1000);
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word - 2));
				address = instance.read_register(C65_REGISTER_STACK_POINTER);
				ASSERT(m_memory.at(++address.word) == ((INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET) & UINT8_MAX)
					+ instruction.length));
				ASSERT(m_memory.at(++address.word) == (INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET) >> CHAR_BIT));
				ASSERT(instance.read_status().raw == state.status.raw);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_load_accumulator(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1.a: LDA imm, non-zero, non-negative
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDA_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDA_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #1.b: LDA imm, non-zero, negative
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDA_IMMEDIATE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDA_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0xaa);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #1.c: LDA imm, zero
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDA_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x00;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDA_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0x00);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test #2: LDA a, non-zero, non-negative
				instance.reset(*this);
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDA_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDA_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #3: LDA a. x, non-zero, non-negative
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{ 0x02, 0x00 }});
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDA_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDA_ABSOLUTE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #4: LDA a,y , non-zero, non-negative
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{ 0x02, 0x00 }});
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDA_ABSOLUTE_INDEX_Y;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDA_ABSOLUTE_INDEX_Y);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #5: LDA zp, non-zero, non-negative
				instance.reset(*this);
				save_state(state);

				address.word = 0xaa;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDA_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDA_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #6: LDA (zp, x), non-zero, non-negative
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{ 0x02, 0x00 }});
				save_state(state);

				address.word = 0xac;
				m_memory.at(address.word) = 0x00;
				m_memory.at(address.word + 1) = 0x10;
				address.word = 0x1000;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDA_ZERO_PAGE_INDEX_INDIRECT;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDA_ZERO_PAGE_INDEX_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #7: LDA zp, x, non-zero, non-negative
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{ 0x02, 0x00 }});
				save_state(state);

				address.word = 0xac;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDA_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDA_ZERO_PAGE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #8: LDA (zp), non-zero, non-negative
				instance.reset(*this);
				save_state(state);

				address.word = 0xaa;
				m_memory.at(address.word) = 0x00;
				m_memory.at(address.word + 1) = 0x10;
				address.word = 0x1000;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDA_ZERO_PAGE_INDIRECT;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDA_ZERO_PAGE_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #9: LDA (zp), y, non-zero, non-negative
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{ 0x02, 0x00 }});
				save_state(state);

				address.word = 0xaa;
				m_memory.at(address.word) = 0x00;
				m_memory.at(address.word + 1) = 0x10;
				address.word = 0x1002;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDA_ZERO_PAGE_INDIRECT_INDEX;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDA_ZERO_PAGE_INDIRECT_INDEX);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_load_index_x(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1.a: LDX imm, non-zero, non-negative
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDX_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDX_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #1.b: LDX imm, non-zero, negative
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDX_IMMEDIATE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDX_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == 0xaa);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #1.c: LDX imm, zero
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDX_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x00;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDX_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == 0x00);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test #2: LDX a, non-zero, non-negative
				instance.reset(*this);
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDX_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDX_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #3: LDX a. y, non-zero, non-negative
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{ 0x02, 0x00 }});
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDX_ABSOLUTE_INDEX_Y;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDX_ABSOLUTE_INDEX_Y);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #4: LDX zp, non-zero, non-negative
				instance.reset(*this);
				save_state(state);

				address.word = 0xaa;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDX_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDX_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #5: LDX zp, y, non-zero, non-negative
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{ 0x02, 0x00 }});
				save_state(state);

				address.word = 0xac;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDX_ZERO_PAGE_INDEX_Y;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDX_ZERO_PAGE_INDEX_Y);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_load_index_y(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1.a: LDY imm, non-zero, non-negative
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDY_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDY_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #1.b: LDY imm, non-zero, negative
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDY_IMMEDIATE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDY_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0xaa);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #1.c: LDY imm, zero
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDY_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x00;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDY_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0x00);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test #2: LDY a, non-zero, non-negative
				instance.reset(*this);
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDY_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDY_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #3: LDY a. x, non-zero, non-negative
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{ 0x02, 0x00 }});
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDY_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDY_ABSOLUTE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #4: LDY zp, non-zero, non-negative
				instance.reset(*this);
				save_state(state);

				address.word = 0xaa;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDY_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDY_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #5: LDY zp, x, non-zero, non-negative
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{ 0x02, 0x00 }});
				save_state(state);

				address.word = 0xac;
				m_memory.at(address.word) = 0x10;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_LDY_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_LDY_ZERO_PAGE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_no_operation(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: NOP i
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_NOP_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_NOP_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_or(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				// Test 1.a: OR a, positive
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_ORA_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_ORA_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word | 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test 1.b; OR a, zero
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x00, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x00;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_ORA_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_ORA_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word | 0x00));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test 1.c: OR a, negative
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x0f, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0xf0;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_ORA_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_ORA_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word | 0xf0));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #2: OR a, x
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_ORA_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_ORA_ABSOLUTE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word | 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #3: OR a, y
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_ORA_ABSOLUTE_INDEX_Y;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_ORA_ABSOLUTE_INDEX_Y);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word | 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #4: OR imm
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_ORA_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x0f;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_ORA_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word | 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #5: OR zp
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_ORA_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_ORA_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word | 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #6: OR (zp, x)
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00ac;
				m_memory.at(address.word) = 0xbb;
				m_memory.at(address.word + 1) = 0xaa;
				address.word = 0xaabb;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_ORA_ZERO_PAGE_INDEX_INDIRECT;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_ORA_ZERO_PAGE_INDEX_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word | 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #7: OR zp, x
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00ac;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_ORA_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_ORA_ZERO_PAGE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word | 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #8: OR (zp)
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0xbb;
				m_memory.at(address.word + 1) = 0xaa;
				address.word = 0xaabb;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_ORA_ZERO_PAGE_INDIRECT;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_ORA_ZERO_PAGE_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word | 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #9: OR (zp), y
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0xbb;
				m_memory.at(address.word + 1) = 0xaa;
				address.word = 0xaabd;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_ORA_ZERO_PAGE_INDIRECT_INDEX;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_ORA_ZERO_PAGE_INDIRECT_INDEX);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word | 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				TRACE_EXIT();
			}

			void
			processor::test_execute_pull(void)
			{
				c65_byte_t value;
				processor_state_t state;
				instruction_t instruction;
				c65_address_t program_counter, stack_pointer;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				program_counter.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				stack_pointer.word = (ADDRESS_MEMORY_STACK_END - 1);

				// Test #1: PLA s, positive
				value = 10;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{(uint8_t)(value + 1), 0}});
				save_state(state);

				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = INSTRUCTION_TYPE_PLA_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PLA_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == value);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2: PLA s, zero
				value = 0;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{(uint8_t)(value + 1), 0}});
				save_state(state);

				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = INSTRUCTION_TYPE_PLA_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PLA_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == value);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				// Test #3: PLA s, negative
				value = 0x80;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{(uint8_t)(value + 1), 0}});
				save_state(state);

				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = INSTRUCTION_TYPE_PLA_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PLA_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == value);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				// Test #4: PLP s
				value = 10;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				save_state(state);

				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = INSTRUCTION_TYPE_PLP_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PLP_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (value | MASK(FLAG_BREAK_INSTRUCTION)));

				// Test #5: PLX s, positive
				value = 10;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_INDEX_X, {{(uint8_t)(value + 1), 0}});
				save_state(state);

				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = INSTRUCTION_TYPE_PLX_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PLX_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == value);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #6: PLX s, zero
				value = 0;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_INDEX_X, {{(uint8_t)(value + 1), 0}});
				save_state(state);

				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = INSTRUCTION_TYPE_PLX_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PLX_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == value);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				// Test #7: PLX s, negative
				value = 0x80;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_INDEX_X, {{(uint8_t)(value + 1), 0}});
				save_state(state);

				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = INSTRUCTION_TYPE_PLX_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PLX_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == value);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				// Test #8: PLY s, positive
				value = 10;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_INDEX_Y, {{(uint8_t)(value + 1), 0}});
				save_state(state);

				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = INSTRUCTION_TYPE_PLY_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PLY_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).low == value);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #9: PLY s, zero
				value = 0;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_INDEX_Y, {{(uint8_t)(value + 1), 0}});
				save_state(state);

				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = INSTRUCTION_TYPE_PLY_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PLY_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).low == value);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				// Test #10: PLY s, negative
				value = 0x80;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_INDEX_Y, {{(uint8_t)(value + 1), 0}});
				save_state(state);

				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = INSTRUCTION_TYPE_PLY_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PLY_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).low == value);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_push(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1: PHA s
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{(uint8_t)std::rand(), 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_PHA_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PHA_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word - 1));
				ASSERT(m_memory.at(state.stack_pointer.word) == state.accumulator.low);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2: PHP s
				instance.reset(*this);
				instance.write_status({{(uint8_t)std::rand()}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_PHP_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PHP_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word - 1));
				ASSERT(m_memory.at(state.stack_pointer.word) == state.status.raw);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #3: PHX s
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{(uint8_t)std::rand(), 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_PHX_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PHX_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word - 1));
				ASSERT(m_memory.at(state.stack_pointer.word) == state.index_x.low);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #4: PHY s
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{(uint8_t)std::rand(), 0}});
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_PHY_STACK;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_PHY_STACK);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word - 1));
				ASSERT(m_memory.at(state.stack_pointer.word) == state.index_y.low);
				ASSERT(instance.read_status().raw == state.status.raw);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_reset_bit(void)
			{
				int type;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: RMB zp
				for(type = INSTRUCTION_RMB0; type <= INSTRUCTION_RMB7; ++type) {
					c65_byte_t index;
					c65_address_t address;
					processor_state_t state;
					instruction_t instruction;

					// Test #1.a: RMB zp, bit unset
					instance.reset(*this);
					save_state(state);

					address.word = 0xaa;
					m_memory.at(address.word) = 0;
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					index = (INSTRUCTION_TYPE_RMB0_ZERO_PAGE + ((type - INSTRUCTION_RMB0) * 0x10));
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;

					instruction = INSTRUCTION(index);
					ASSERT(instance.step(*this) == (instruction.cycle + 2));
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
					ASSERT(!m_memory.at(0xaa));

					// Test #1.b: RMB zp, bit set
					instance.reset(*this);
					save_state(state);

					address.word = 0xaa;
					m_memory.at(address.word) = MASK(type - INSTRUCTION_RMB0);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					index = (INSTRUCTION_TYPE_RMB0_ZERO_PAGE + ((type - INSTRUCTION_RMB0) * 0x10));
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;

					instruction = INSTRUCTION(index);
					ASSERT(instance.step(*this) == (instruction.cycle + 2));
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
					ASSERT(!m_memory.at(0xaa));
				}

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_return_interrupt(void)
			{
				c65_status_t status;
				processor_state_t state;
				instruction_t instruction;
				c65_address_t address, value;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: RTI R
				instance.reset(*this);
				save_state(state);

				status.raw = std::rand();
				value.word = std::rand();
				address.word = ADDRESS_MEMORY_STACK_END;
				m_memory.at(address.word--) = value.high;
				m_memory.at(address.word--) = value.low;
				m_memory.at(address.word--) = status.raw;
				instance.write_register(C65_REGISTER_STACK_POINTER, address);
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_RTI_RETURN;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_RTI_RETURN);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == value.word);
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == ADDRESS_MEMORY_STACK_END);
				ASSERT(instance.read_status().raw == (status.raw | MASK(FLAG_BREAK_INSTRUCTION)));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_return_subroutine(void)
			{
				processor_state_t state;
				instruction_t instruction;
				c65_address_t address, value;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: RTS R
				instance.reset(*this);
				save_state(state);

				value.word = std::rand();
				address.word = ADDRESS_MEMORY_STACK_END;
				m_memory.at(address.word--) = value.high;
				m_memory.at(address.word--) = value.low;
				instance.write_register(C65_REGISTER_STACK_POINTER, address);
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_RTS_RETURN;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_RTS_RETURN);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (value.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == ADDRESS_MEMORY_STACK_END);
				ASSERT(instance.read_status().raw == state.status.raw);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_rotate_left(void)
			{
				TRACE_ENTRY();

				// TODO

				TRACE_EXIT();
			}

			void
			processor::test_execute_rotate_right(void)
			{
				TRACE_ENTRY();

				// TODO

				TRACE_EXIT();
			}

			void
			processor::test_execute_set(void)
			{
				c65_status_t status;
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1: SEC i
				instance.reset(*this);
				status = {};
				status.carry = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_SEC_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_SEC_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().carry);

				// Test #2: SED i
				instance.reset(*this);

				status = {};
				status.decimal_mode = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_SED_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_SED_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().decimal_mode);

				// Test #3: SEI i
				instance.reset(*this);

				status = {};
				status.interrupt_disable = false;
				instance.write_status(status);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_SEI_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_SEI_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().interrupt_disable);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_set_bit(void)
			{
				int type;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: SMB zp
				for(type = INSTRUCTION_SMB0; type <= INSTRUCTION_SMB7; ++type) {
					c65_byte_t index;
					c65_address_t address;
					processor_state_t state;
					instruction_t instruction;

					// Test #1.a: SMB zp, bit unset
					instance.reset(*this);
					save_state(state);

					address.word = 0xaa;
					m_memory.at(address.word) = 0;
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					index = (INSTRUCTION_TYPE_SMB0_ZERO_PAGE + ((type - INSTRUCTION_SMB0) * 0x10));
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;

					instruction = INSTRUCTION(index);
					ASSERT(instance.step(*this) == (instruction.cycle + 2));
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
					ASSERT(MASK_CHECK(m_memory.at(0xaa), type - INSTRUCTION_SMB0));

					// Test #1.b: SMB zp, bit set
					instance.reset(*this);
					save_state(state);

					address.word = 0xaa;
					m_memory.at(address.word) = MASK(type - INSTRUCTION_SMB0);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					index = (INSTRUCTION_TYPE_SMB0_ZERO_PAGE + ((type - INSTRUCTION_SMB0) * 0x10));
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;

					instruction = INSTRUCTION(index);
					ASSERT(instance.step(*this) == (instruction.cycle + 2));
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
					ASSERT(MASK_CHECK(m_memory.at(0xaa), type - INSTRUCTION_SMB0));
				}

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_shift_left(void)
			{
				TRACE_ENTRY();

				// TODO

				TRACE_EXIT();
			}

			void
			processor::test_execute_shift_right(void)
			{
				TRACE_ENTRY();

				// TODO

				TRACE_EXIT();
			}

			void
			processor::test_execute_stop(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: STP i
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STP_STATE;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STP_STATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(instance.stopped());

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_store_accumulator(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: STA a
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0xaa, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STA_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STA_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0xaa);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x1000) == 0xaa);
				m_memory.at(0x1000) = 0x00;

				// Test #2: STA a, x
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0xaa, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STA_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STA_ABSOLUTE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0xaa);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x1002) == 0xaa);
				m_memory.at(0x1002) = 0x00;

				// Test #3: STA a, y
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0xaa, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STA_ABSOLUTE_INDEX_Y;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STA_ABSOLUTE_INDEX_Y);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0xaa);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x1002) == 0xaa);
				m_memory.at(0x1002) = 0x00;

				// Test #4: STA zp
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x10, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STA_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STA_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x00aa) == 0x10);
				m_memory.at(0x00aa) = 0x00;

				// Test #5: STA (zp, x)
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0xaa, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0xac;
				m_memory.at(address.word) = 0x00;
				m_memory.at(address.word + 1) = 0x10;

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STA_ZERO_PAGE_INDEX_INDIRECT;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STA_ZERO_PAGE_INDEX_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0xaa);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x1000) == 0xaa);
				m_memory.at(0x1000) = 0x00;

				// Test #6: STA zp, x
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x10, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STA_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STA_ZERO_PAGE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x00ac) == 0x10);
				m_memory.at(0x00ac) = 0x00;

				// Test #7: STA (zp)
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0xaa, 0x00}});
				save_state(state);

				address.word = 0xaa;
				m_memory.at(address.word) = 0x00;
				m_memory.at(address.word + 1) = 0x10;

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STA_ZERO_PAGE_INDIRECT;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STA_ZERO_PAGE_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0xaa);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x1000) == 0xaa);
				m_memory.at(0x1000) = 0x00;

				// Test #5: STA (zp), y
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0xaa, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = 0xaa;
				m_memory.at(address.word) = 0x00;
				m_memory.at(address.word + 1) = 0x10;

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STA_ZERO_PAGE_INDIRECT_INDEX;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STA_ZERO_PAGE_INDIRECT_INDEX);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == 0xaa);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x1002) == 0xaa);
				m_memory.at(0x1002) = 0x00;

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_store_index_x(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: STX a
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0xaa, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STX_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STX_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == 0xaa);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x1000) == 0xaa);
				m_memory.at(0x1000) = 0x00;

				// Test #2: STX zp
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x10, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STX_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STX_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x00aa) == 0x10);
				m_memory.at(0x00aa) = 0x00;

				// Test #3: STX zp, y
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x10, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STX_ZERO_PAGE_INDEX_Y;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STX_ZERO_PAGE_INDEX_Y);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x00ac) == 0x10);
				m_memory.at(0x00ac) = 0x00;

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_store_index_y(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: STY a
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0xaa, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STY_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STY_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0xaa);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x1000) == 0xaa);
				m_memory.at(0x1000) = 0x00;

				// Test #2: STY zp
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x10, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STY_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STY_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x00aa) == 0x10);
				m_memory.at(0x00aa) = 0x00;

				// Test #3: STY zp, x
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x10, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STY_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STY_ZERO_PAGE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0x10);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x00ac) == 0x10);
				m_memory.at(0x00ac) = 0x00;

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_store_zero(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: STZ a
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STZ_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STZ_ABSOLUTE);
				m_memory.at(0x1000) = 0xaa;
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x1000) == 0x00);

				// Test #2: STZ a, x
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STZ_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STZ_ABSOLUTE_INDEX_X);
				m_memory.at(0x1002) = 0xaa;
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x1002) == 0x00);

				// Test #3: STZ zp
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STZ_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STZ_ZERO_PAGE);
				m_memory.at(0x00aa) = 0xaa;
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x00aa) == 0x00);

				// Test #4: STZ zp, x
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_STZ_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_STZ_ZERO_PAGE_INDEX_X);
				m_memory.at(0x00ac) = 0xaa;
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(m_memory.at(0x00ac) == 0x00);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_subtract(void)
			{
				TRACE_ENTRY();

				// TODO

				TRACE_EXIT();
			}

			void
			processor::test_execute_test_reset_bit(void)
			{
				int type = 0;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				for(; type < CHAR_BIT; ++type) {
					processor_state_t state;
					instruction_t instruction;
					c65_address_t address, value;

					// Test #1.a: TRB a, found
					instance.reset(*this);
					value.word = MASK(type);
					instance.write_register(C65_REGISTER_ACCUMULATOR, value);
					save_state(state);

					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					m_memory.at(address.word) = INSTRUCTION_TYPE_TRB_ABSOLUTE;
					m_memory.at(address.word + 1) = 0x00;
					m_memory.at(address.word + 2) = 0x10;
					address.word = 0x1000;
					m_memory.at(address.word) = 0xff;

					instruction = INSTRUCTION(INSTRUCTION_TYPE_TRB_ABSOLUTE);
					ASSERT(instance.step(*this) == (instruction.cycle + 2));
					ASSERT(m_memory.at(address.word) == (0xff & ~value.low));
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

					// Test #1.b: TRB a, not found
					instance.reset(*this);
					value.word = MASK(type);
					instance.write_register(C65_REGISTER_ACCUMULATOR, value);
					save_state(state);

					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					m_memory.at(address.word) = INSTRUCTION_TYPE_TRB_ABSOLUTE;
					m_memory.at(address.word + 1) = 0x00;
					m_memory.at(address.word + 2) = 0x10;
					address.word = 0x1000;
					m_memory.at(address.word) = (0xff & ~value.low);

					instruction = INSTRUCTION(INSTRUCTION_TYPE_TRB_ABSOLUTE);
					ASSERT(instance.step(*this) == (instruction.cycle + 2));
					ASSERT(m_memory.at(address.word) == (0xff & ~value.low));
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);

					// Test #2.a: TRB zp, found
					instance.reset(*this);
					value.word = MASK(type);
					instance.write_register(C65_REGISTER_ACCUMULATOR, value);
					save_state(state);

					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					m_memory.at(address.word) = INSTRUCTION_TYPE_TRB_ZERO_PAGE;
					m_memory.at(address.word + 1) = 0xaa;
					address.word = 0x00aa;
					m_memory.at(address.word) = 0xff;

					instruction = INSTRUCTION(INSTRUCTION_TYPE_TRB_ZERO_PAGE);
					ASSERT(instance.step(*this) == (instruction.cycle + 2));
					ASSERT(m_memory.at(address.word) == (0xff & ~value.low));
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

					// Test #2.b: TRB zp, not found
					instance.reset(*this);
					value.word = MASK(type);
					instance.write_register(C65_REGISTER_ACCUMULATOR, value);
					save_state(state);

					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					m_memory.at(address.word) = INSTRUCTION_TYPE_TRB_ZERO_PAGE;
					m_memory.at(address.word + 1) = 0xaa;
					address.word = 0x00aa;
					m_memory.at(address.word) = (0xff & ~value.low);

					instruction = INSTRUCTION(INSTRUCTION_TYPE_TRB_ZERO_PAGE);
					ASSERT(instance.step(*this) == (instruction.cycle + 2));
					ASSERT(m_memory.at(address.word) == (0xff & ~value.low));
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
				}

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_test_set_bit(void)
			{
				int type = 0;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				for(; type < CHAR_BIT; ++type) {
					processor_state_t state;
					instruction_t instruction;
					c65_address_t address, value;

					// Test #1.a: TSB a, found
					instance.reset(*this);
					value.word = MASK(type);
					instance.write_register(C65_REGISTER_ACCUMULATOR, value);
					save_state(state);

					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					m_memory.at(address.word) = INSTRUCTION_TYPE_TSB_ABSOLUTE;
					m_memory.at(address.word + 1) = 0x00;
					m_memory.at(address.word + 2) = 0x10;
					address.word = 0x1000;
					m_memory.at(address.word) = value.low;

					instruction = INSTRUCTION(INSTRUCTION_TYPE_TSB_ABSOLUTE);
					ASSERT(instance.step(*this) == (instruction.cycle + 2));
					ASSERT(m_memory.at(address.word) == value.low);
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

					// Test #1.b: TSB a, not found
					instance.reset(*this);
					value.word = MASK(type);
					instance.write_register(C65_REGISTER_ACCUMULATOR, value);
					save_state(state);

					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					m_memory.at(address.word) = INSTRUCTION_TYPE_TSB_ABSOLUTE;
					m_memory.at(address.word + 1) = 0x00;
					m_memory.at(address.word + 2) = 0x10;
					address.word = 0x1000;
					m_memory.at(address.word) = 0x00;

					instruction = INSTRUCTION(INSTRUCTION_TYPE_TSB_ABSOLUTE);
					ASSERT(instance.step(*this) == (instruction.cycle + 2));
					ASSERT(m_memory.at(address.word) == value.low);
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);

					// Test #2.a: TSB zp, found
					instance.reset(*this);
					value.word = MASK(type);
					instance.write_register(C65_REGISTER_ACCUMULATOR, value);
					save_state(state);

					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					m_memory.at(address.word) = INSTRUCTION_TYPE_TSB_ZERO_PAGE;
					m_memory.at(address.word + 1) = 0xaa;
					address.word = 0x00aa;
					m_memory.at(address.word) = value.low;

					instruction = INSTRUCTION(INSTRUCTION_TYPE_TSB_ZERO_PAGE);
					ASSERT(instance.step(*this) == (instruction.cycle + 2));
					ASSERT(m_memory.at(address.word) == value.low);
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

					// Test #2.b: TSB zp, not found
					instance.reset(*this);
					value.word = MASK(type);
					instance.write_register(C65_REGISTER_ACCUMULATOR, value);
					save_state(state);

					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					m_memory.at(address.word) = INSTRUCTION_TYPE_TSB_ZERO_PAGE;
					m_memory.at(address.word + 1) = 0xaa;
					address.word = 0x00aa;
					m_memory.at(address.word) = 0x00;

					instruction = INSTRUCTION(INSTRUCTION_TYPE_TSB_ZERO_PAGE);
					ASSERT(instance.step(*this) == (instruction.cycle + 2));
					ASSERT(m_memory.at(address.word) == value.low);
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
				}

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_transfer(void)
			{
				processor_state_t state;
				instruction_t instruction;
				c65_address_t address, value;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1.a: TAX i, positive
				instance.reset(*this);
				value.word = 10;
				instance.write_register(C65_REGISTER_ACCUMULATOR, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TAX_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TAX_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #1.b: TAX i, zero
				instance.reset(*this);
				value.word = 0;
				instance.write_register(C65_REGISTER_ACCUMULATOR, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TAX_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TAX_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test #1.c: TAX i, negative
				instance.reset(*this);
				value.word = -10;
				instance.write_register(C65_REGISTER_ACCUMULATOR, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TAX_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TAX_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #2.a: TAY i, positive
				instance.reset(*this);
				value.word = 10;
				instance.write_register(C65_REGISTER_ACCUMULATOR, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TAY_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TAY_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #2.b: TAY i, zero
				value.word = 0;
				instance.reset(*this);
				value.word = 0;
				instance.write_register(C65_REGISTER_ACCUMULATOR, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TAY_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TAY_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test #2.c: TAY i, negative
				instance.reset(*this);
				value.word = -10;
				instance.write_register(C65_REGISTER_ACCUMULATOR, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TAY_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TAY_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #3.a: TSX i, positive
				instance.reset(*this);
				value.word = 10;
				instance.write_register(C65_REGISTER_STACK_POINTER, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TSX_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TSX_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == state.stack_pointer.low);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #3.b: TSX i, zero
				instance.reset(*this);
				value.word = 0;
				instance.write_register(C65_REGISTER_STACK_POINTER, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TSX_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TSX_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == state.stack_pointer.low);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test #3.c: TSX i, negative
				instance.reset(*this);
				value.word = -10;
				instance.write_register(C65_REGISTER_STACK_POINTER, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TSX_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TSX_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == state.stack_pointer.low);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #4.a: TXA i, positive
				instance.reset(*this);
				value.word = 10;
				instance.write_register(C65_REGISTER_INDEX_X, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TXA_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TXA_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #4.b: TXA i, zero
				instance.reset(*this);
				value.word = 0;
				instance.write_register(C65_REGISTER_INDEX_X, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TXA_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TXA_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test #4.c: TXA i, negative
				instance.reset(*this);
				value.word = -10;
				instance.write_register(C65_REGISTER_INDEX_X, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TXA_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TXA_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #5: TXS i
				value.word = std::rand();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_INDEX_X, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TXS_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TXS_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).high == state.stack_pointer.high);
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).low == value.low);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #6.a: TYA i, positive
				instance.reset(*this);
				value.word = 10;
				instance.write_register(C65_REGISTER_INDEX_Y, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TYA_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TYA_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #6.b: TYA i, zero
				instance.reset(*this);
				value.word = 0;
				instance.write_register(C65_REGISTER_INDEX_Y, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TYA_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TYA_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test #6.c: TYA i, negative
				instance.reset(*this);
				value.word = -10;
				instance.write_register(C65_REGISTER_INDEX_Y, value);
				save_state(state);

				m_memory.at(address.word) = INSTRUCTION_TYPE_TYA_IMPLIED;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_TYA_IMPLIED);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_wait(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: WAI i
				instance.reset(*this);
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_WAI_STATE;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_WAI_STATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(instance.waiting());

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_xor(void)
			{
				c65_address_t address;
				processor_state_t state;
				instruction_t instruction;

				TRACE_ENTRY();

				clear_memory();
				c65::system::processor &instance = c65::system::processor::instance();

				// Test 1.a: EOR a, positive
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_EOR_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_EOR_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word ^ 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test 1.b; EOR a, zero
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x00, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0x00;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_EOR_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_EOR_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word ^ 0x00));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO)));

				// Test 1.c: EOR a, negative
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x0f, 0x00}});
				save_state(state);

				address.word = 0x1000;
				m_memory.at(address.word) = 0xf0;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_EOR_ABSOLUTE;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_EOR_ABSOLUTE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word ^ 0xf0));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE) & ~MASK(FLAG_ZERO)));

				// Test #2: EOR a, x
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_EOR_ABSOLUTE_INDEX_X;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_EOR_ABSOLUTE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word ^ 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #3: EOR a, y
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x1002;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_EOR_ABSOLUTE_INDEX_Y;
				m_memory.at(address.word + 1) = 0x00;
				m_memory.at(address.word + 2) = 0x10;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_EOR_ABSOLUTE_INDEX_Y);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word ^ 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #4: EOR imm
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_EOR_IMMEDIATE;
				m_memory.at(address.word + 1) = 0x0f;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_EOR_IMMEDIATE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word ^ 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #5: EOR zp
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_EOR_ZERO_PAGE;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_EOR_ZERO_PAGE);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word ^ 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #6: EOR (zp, x)
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00ac;
				m_memory.at(address.word) = 0xbb;
				m_memory.at(address.word + 1) = 0xaa;
				address.word = 0xaabb;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_EOR_ZERO_PAGE_INDEX_INDIRECT;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_EOR_ZERO_PAGE_INDEX_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word ^ 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #7: EOR zp, x
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_X, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00ac;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_EOR_ZERO_PAGE_INDEX_X;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_EOR_ZERO_PAGE_INDEX_X);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word ^ 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #8: EOR (zp)
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0xbb;
				m_memory.at(address.word + 1) = 0xaa;
				address.word = 0xaabb;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_EOR_ZERO_PAGE_INDIRECT;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_EOR_ZERO_PAGE_INDIRECT);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word ^ 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				// Test #9: EOR (zp), y
				instance.initialize();
				instance.reset(*this);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{0x02, 0x00}});
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x02, 0x00}});
				save_state(state);

				address.word = 0x00aa;
				m_memory.at(address.word) = 0xbb;
				m_memory.at(address.word + 1) = 0xaa;
				address.word = 0xaabd;
				m_memory.at(address.word) = 0x0f;
				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				m_memory.at(address.word) = INSTRUCTION_TYPE_EOR_ZERO_PAGE_INDIRECT_INDEX;
				m_memory.at(address.word + 1) = 0xaa;

				instruction = INSTRUCTION(INSTRUCTION_TYPE_EOR_ZERO_PAGE_INDIRECT_INDEX);
				ASSERT(instance.step(*this) == instruction.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == (state.accumulator.word ^ 0x0f));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + instruction.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw & ~(MASK(FLAG_NEGATIVE) | MASK(FLAG_ZERO))));

				TRACE_EXIT();
			}

			void
			processor::test_interrupt(void)
			{
				int type = 0;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: Valid interrupt
				for(; type <= C65_INTERRUPT_MAX; ++type) {
					c65_status_t status = {};
					c65_address_t address = {};
					c65_register_t program_counter = {}, stack = {};

					instance.reset(*this);

					address.word = ADDRESS_PROCESSOR_NON_MASKABLE_BEGIN;
					instance.write(address, INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_NON_MASKABLE));
					address.word = ADDRESS_PROCESSOR_NON_MASKABLE_END;
					instance.write(address, INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_NON_MASKABLE) >> CHAR_BIT);
					address.word = ADDRESS_PROCESSOR_MASKABLE_BEGIN;
					instance.write(address, INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_MASKABLE));
					address.word = ADDRESS_PROCESSOR_MASKABLE_END;
					instance.write(address, INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_MASKABLE) >> CHAR_BIT);

					for(address.word = ADDRESS_MEMORY_STACK_END; address.word > (ADDRESS_MEMORY_STACK_END - 3);
							address.word--) {
						m_memory.at(address.word) = MEMORY_FILL;
					}

					program_counter.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					stack.word = ADDRESS_MEMORY_STACK_END;

					switch(type) {
						case C65_INTERRUPT_NON_MASKABLE:
							instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
							instance.write_register(C65_REGISTER_STACK_POINTER, stack);
							instance.interrupt(type);

							status = instance.read_status();
							status.break_instruction = false;
							ASSERT(instance.step(*this) == (INSTRUCTION_MODE_CYCLE(INSTRUCTION_MODE_IMPLIED)
								+ INSTRUCTION_MODE_CYCLE(INSTRUCTION_MODE_INTERRUPT)));
							ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word
								== (INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_NON_MASKABLE) + 1));
							ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word
								== (ADDRESS_MEMORY_STACK_END - 3));
							ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END - 2) == status.raw);
							ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END - 1) == program_counter.low);
							ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END) == program_counter.high);
							break;
						case C65_INTERRUPT_MASKABLE:

							// Test #1.a: IRQ with interrupts disabled
							instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
							instance.write_register(C65_REGISTER_STACK_POINTER, stack);

							instance.interrupt(type);
							m_memory.at(program_counter.word) = INSTRUCTION_TYPE_NOP_IMPLIED;
							ASSERT(instance.step(*this) == INSTRUCTION_MODE_CYCLE(INSTRUCTION_MODE_IMPLIED));
							ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word
								== (program_counter.word + 1));
							ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == stack.word);

							// Test #1.b: IRQ with interrupts enabled
							status = instance.read_status();
							status.interrupt_disable = false;
							instance.write_status(status);
							instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
							instance.write_register(C65_REGISTER_STACK_POINTER, stack);
							instance.interrupt(type);

							status = instance.read_status();
							status.break_instruction = false;
							status.interrupt_disable = false;
							ASSERT(instance.step(*this) == (INSTRUCTION_MODE_CYCLE(INSTRUCTION_MODE_IMPLIED)
								+ INSTRUCTION_MODE_CYCLE(INSTRUCTION_MODE_INTERRUPT)));
							ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word
								== (INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_MASKABLE) + 1));
							ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word
								== (ADDRESS_MEMORY_STACK_END - 3));
							ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END - 2) == status.raw);
							ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END - 1) == program_counter.low);
							ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END) == program_counter.high);
							break;
						default:
							break;
					}
				}

				// Test #2: Invalid interrupt
				try {
					instance.interrupt(type);
					ASSERT(false);
				} catch(...) { }

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_interrupt_pending(void)
			{
				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: Maskable interrupt
				instance.reset(*this);
				ASSERT(!instance.interrupt_pending());
				instance.interrupt(C65_INTERRUPT_MASKABLE);
				ASSERT(instance.interrupt_pending());

				// Test #2: Non-maskable interrupt
				instance.reset(*this);
				ASSERT(!instance.interrupt_pending());
				instance.interrupt(C65_INTERRUPT_NON_MASKABLE);
				ASSERT(instance.interrupt_pending());

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_read(void)
			{
				c65_address_t address = {};

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: Valid range
				address.word = ADDRESS_PROCESSOR_NON_MASKABLE_BEGIN;

				while(address.word) {
					instance.read(address);
					++address.word;
				}

				// Test #2: Invalid range
				try {
					address.word = ((std::rand() % ADDRESS_LENGTH(ADDRESS_MEMORY_HIGH_BEGIN, ADDRESS_MEMORY_HIGH_END))
								+ ADDRESS_MEMORY_HIGH_BEGIN);
					instance.read(address);
					ASSERT(false);
				} catch(...) { }

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_read_register(void)
			{
				int type = 0;
				c65_register_t value = {};

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();
				instance.reset(*this);

				// Test #1: Valid register
				for(; type <= C65_REGISTER_MAX; ++type) {
					value = instance.read_register(type);

					switch(type) {
						case C65_REGISTER_ACCUMULATOR:
						case C65_REGISTER_INDEX_X:
						case C65_REGISTER_INDEX_Y:
							ASSERT(value.low == 0);
							break;
						case C65_REGISTER_PROGRAM_COUNTER:
							ASSERT(value.word == ADDRESS_MEMORY_HIGH_BEGIN);
							break;
						case C65_REGISTER_STACK_POINTER:
							ASSERT(value.word == ADDRESS_MEMORY_STACK_END);
							break;
						default:
							break;
					}
				}

				// Test #2: Invalid register
				try {
					instance.read_register(type);
					ASSERT(false);
				} catch(...) { }

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_read_status(void)
			{
				c65_status_t status = {};

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				status.break_instruction = true;
				status.interrupt_disable = true;
				status.unused = true;

				instance.initialize();
				instance.reset(*this);

				ASSERT(instance.read_status().raw == status.raw);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_reset(void)
			{
				c65_status_t status = {};

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				status.break_instruction = true;
				status.interrupt_disable = true;
				status.unused = true;

				instance.initialize();
				instance.reset(*this);

				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == 0);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == 0);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word ==
					INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == ADDRESS_MEMORY_STACK_END);
				ASSERT(instance.read_status().raw == status.raw);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_stack_overflow(void)
			{
				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				ASSERT(!instance.stack_overflow());

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_stack_underflow(void)
			{
				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				ASSERT(!instance.stack_underflow());

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_step(void)
			{
				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				ASSERT(instance.step(*this) == INSTRUCTION_MODE_CYCLE(INSTRUCTION_MODE_IMPLIED));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_stopped(void)
			{
				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				ASSERT(!instance.stopped());

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_waiting(void)
			{
				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				ASSERT(!instance.waiting());

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_write(void)
			{
				c65_byte_t value;
				c65_address_t address = {};

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();
				value = std::rand();

				instance.initialize();

				// Test #1: Valid range
				address.word = ADDRESS_PROCESSOR_NON_MASKABLE_BEGIN;

				while(address.word) {
					instance.write(address, value);
					ASSERT(instance.read(address) == value);
					++address.word;
				}

				// Test #2: Invalid range
				try {
					address.word = ((std::rand() % ADDRESS_LENGTH(ADDRESS_MEMORY_HIGH_BEGIN, ADDRESS_MEMORY_HIGH_END))
								+ ADDRESS_MEMORY_HIGH_BEGIN);
					instance.write(address, value);
					ASSERT(false);
				} catch(...) { }

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_write_register(void)
			{
				int type = 0;
				c65_register_t value = {};

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();
				instance.reset(*this);

				// Test #1: Valid register
				for(; type <= C65_REGISTER_MAX; ++type) {
					value.word = std::rand();
					instance.write_register(type, value);

					switch(type) {
						case C65_REGISTER_ACCUMULATOR:
						case C65_REGISTER_INDEX_X:
						case C65_REGISTER_INDEX_Y:
							ASSERT(instance.read_register(type).low == value.low);
							break;
						case C65_REGISTER_PROGRAM_COUNTER:
							ASSERT(instance.read_register(type).word == value.word);
							break;
						case C65_REGISTER_STACK_POINTER:
							ASSERT(instance.read_register(type).low == value.low);
							break;
						default:
							break;
					}
				}

				// Test #2: Invalid register
				try {
					value.word = std::rand();
					instance.write_register(type, value);
					ASSERT(false);
				} catch(...) { }

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_write_status(void)
			{
				c65_status_t value = {};

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				value.raw = std::rand();
				instance.write_status(value);
				ASSERT(instance.read_status().raw == value.raw);

				instance.uninitialize();

				TRACE_EXIT();
			}
		}
	}
}
