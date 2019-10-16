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
			processor::on_run(void)
			{
				TRACE_ENTRY();

				test_execute_branch();
				test_execute_branch_bit();
				test_execute_break();
				test_execute_clear();
				test_execute_decrement_index();
				test_execute_increment_index();
				test_execute_pull();
				test_execute_push();
				test_execute_no_operation();
				test_execute_reset_bit();
				test_execute_set();
				test_execute_set_bit();
				test_execute_stop();
				test_execute_wait();
				test_interrupt();
				test_interrupt_pending();
				test_read();
				test_read_register();
				test_read_status();
				test_reset();
				test_step();
				test_stopped();
				test_waiting();
				test_write();
				test_write_register();
				test_write_status();

				TRACE_EXIT();
			}

			void
			processor::on_setup(void)
			{
				int type = 0;
				c65_word_t *interrupt;

				TRACE_ENTRY();

				m_memory.resize(UINT16_MAX + 1, MEMORY_FILL);
				interrupt = (c65_word_t *)&m_memory[ADDRESS_PROCESSOR_NON_MASKABLE_BEGIN];

				for(; type <= INTERRUPT_VECTOR_MAX; ++type) {
					interrupt[type] = INTERRUPT_VECTOR_ADDRESS(type);
				}

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
			processor::test_execute_branch(void)
			{
				command_t command;
				c65_status_t status;
				c65_address_t address;
				processor_state_t state;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1.a: BCC not taken
				instance.reset(*this);
				status = {};
				status.carry = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BCC_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BCC_RELATIVE);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #1.b: BCC taken forward
				instance.reset(*this);
				status = {};
				status.carry = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BCC_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BCC_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #1.c: BCC taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.carry = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BCC_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;
				command = COMMAND(COMMAND_TYPE_BCC_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2.a: BCS not taken
				instance.reset(*this);
				status = {};
				status.carry = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BCS_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BCS_RELATIVE);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2.b: BCS taken forward
				instance.reset(*this);
				status = {};
				status.carry = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BCS_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BCS_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2.c: BCS taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.carry = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BCS_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;
				command = COMMAND(COMMAND_TYPE_BCS_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #3.a: BEQ not taken
				instance.reset(*this);
				status = {};
				status.zero = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BEQ_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BEQ_RELATIVE);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #3.b: BEQ taken forward
				instance.reset(*this);
				status = {};
				status.zero = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BEQ_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BEQ_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #3.c: BEQ taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.zero = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BEQ_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;
				command = COMMAND(COMMAND_TYPE_BEQ_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #4.a: BMI not taken
				instance.reset(*this);
				status = {};
				status.negative = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BMI_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BMI_RELATIVE);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #4.b: BMI taken forward
				instance.reset(*this);
				status = {};
				status.negative = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BMI_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BMI_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #4.c: BMI taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.negative = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BMI_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;
				command = COMMAND(COMMAND_TYPE_BMI_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #5.a: BNE not taken
				instance.reset(*this);
				status = {};
				status.zero = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BNE_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BNE_RELATIVE);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #5.b: BNE taken forward
				instance.reset(*this);
				status = {};
				status.zero = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BNE_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BNE_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #5.c: BNE taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.zero = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BNE_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;
				command = COMMAND(COMMAND_TYPE_BNE_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #6.a: BPL not taken
				instance.reset(*this);
				status = {};
				status.negative = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BPL_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BPL_RELATIVE);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #6.b: BPL taken forward
				instance.reset(*this);
				status = {};
				status.negative = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BPL_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BPL_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #6.c: BPL taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.negative = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BPL_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;
				command = COMMAND(COMMAND_TYPE_BPL_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #7.a: BVC not taken
				instance.reset(*this);
				status = {};
				status.overflow = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BVC_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BVC_RELATIVE);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #7.b: BVC taken forward
				instance.reset(*this);
				status = {};
				status.overflow = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BVC_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BVC_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #7.c: BVC taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.overflow = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BVC_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;
				command = COMMAND(COMMAND_TYPE_BVC_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 2));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #8.a: BVS not taken
				instance.reset(*this);
				status = {};
				status.overflow = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BVS_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BVS_RELATIVE);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #8.b: BVS taken forward
				instance.reset(*this);
				status = {};
				status.overflow = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BVS_RELATIVE;
				m_memory.at(address.word + 1) = 10;
				command = COMMAND(COMMAND_TYPE_BVS_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 1));
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 11));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #8.c: BVS taken backwards, page crossing
				instance.reset(*this);
				status = {};
				status.overflow = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_BVS_RELATIVE;
				m_memory.at(address.word + 1) = 0xfd;
				command = COMMAND(COMMAND_TYPE_BVS_RELATIVE);
				ASSERT(instance.step(*this) == (command.cycle + 2));
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
				command_t command;
				c65_address_t address;
				processor_state_t state;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: BBR
				for(type = COMMAND_BBR0; type <= COMMAND_BBR7; ++type) {

					// Test #1.a: BBR with bit unset
					index = (COMMAND_TYPE_BBR0_ZERO_PAGE_RELATIVE + ((type - COMMAND_BBR0) * 0x10));
					instance.reset(*this);
					address.word = 0xaa;
					m_memory.at(address.word) = 0;
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
					save_state(state);
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;
					m_memory.at(address.word + 2) = 0x02;
					command = COMMAND(index);
					ASSERT(instance.step(*this) == command.cycle);
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + 5));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);

					// Test #1.b: BBR with bit set
					index = (COMMAND_TYPE_BBR0_ZERO_PAGE_RELATIVE + ((type - COMMAND_BBR0) * 0x10));
					instance.reset(*this);
					address.word = 0xaa;
					m_memory.at(address.word) = MASK(type - COMMAND_BBR0);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
					save_state(state);
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;
					m_memory.at(address.word + 2) = 0x02;
					command = COMMAND(index);
					ASSERT(instance.step(*this) == command.cycle);
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + 3));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
				}

				// Test #2: BBS
				for(type = COMMAND_BBS0; type <= COMMAND_BBS7; ++type) {

					// Test #2.a: BBS with bit unset
					index = (COMMAND_TYPE_BBS0_ZERO_PAGE_RELATIVE + ((type - COMMAND_BBS0) * 0x10));
					instance.reset(*this);
					address.word = 0xaa;
					m_memory.at(address.word) = 0;
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
					save_state(state);
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;
					m_memory.at(address.word + 2) = 0x02;
					command = COMMAND(index);
					ASSERT(instance.step(*this) == command.cycle);
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + 3));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);

					// Test #2.b: BBS with bit set
					index = (COMMAND_TYPE_BBS0_ZERO_PAGE_RELATIVE + ((type - COMMAND_BBS0) * 0x10));
					instance.reset(*this);
					address.word = 0xaa;
					m_memory.at(address.word) = MASK(type - COMMAND_BBS0);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
					save_state(state);
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;
					m_memory.at(address.word + 2) = 0x02;
					command = COMMAND(index);
					ASSERT(instance.step(*this) == command.cycle);
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
				command_t command;
				c65_address_t address;
				processor_state_t state;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

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
				m_memory.at(address.word) = COMMAND_TYPE_BRK_INTERRUPT;
				command = COMMAND(COMMAND_TYPE_BRK_INTERRUPT);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word
					== INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_MASKABLE));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word - 3));
				state.status.break_instruction = true;
				ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END - 2) == state.status.raw);
				ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END - 1) == (uint8_t)(state.program_counter.low + command.length + 1));
				ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END) == (uint8_t)(state.program_counter.high >> CHAR_BIT));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_clear(void)
			{
				command_t command;
				c65_status_t status;
				c65_address_t address;
				processor_state_t state;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1: CLC
				instance.reset(*this);
				status = {};
				status.carry = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_CLC_IMPLIED;
				command = COMMAND(COMMAND_TYPE_CLC_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(!instance.read_status().carry);

				// Test #2: CLD
				instance.reset(*this);
				status = {};
				status.decimal_mode = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_CLD_IMPLIED;
				command = COMMAND(COMMAND_TYPE_CLD_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(!instance.read_status().decimal_mode);

				// Test #3: CLI
				instance.reset(*this);
				status = {};
				status.interrupt_disable = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_CLI_IMPLIED;
				command = COMMAND(COMMAND_TYPE_CLI_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(!instance.read_status().interrupt_disable);

				// Test #4: CLV
				instance.reset(*this);
				status = {};
				status.overflow = true;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_CLV_IMPLIED;
				command = COMMAND(COMMAND_TYPE_CLV_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(!instance.read_status().overflow);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_decrement_index(void)
			{
				command_t command;
				c65_address_t address;
				processor_state_t state;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1: DEX with index-x > 1
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_X, {{10, 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_DEX_IMPLIED;
				command = COMMAND(COMMAND_TYPE_DEX_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == (state.index_x.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2: DEX with index-x = 1
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_X, {{1, 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_DEX_IMPLIED;
				command = COMMAND(COMMAND_TYPE_DEX_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == (state.index_x.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				// Test #3: DEX with index-x = 0
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_X, {{0, 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_DEX_IMPLIED;
				command = COMMAND(COMMAND_TYPE_DEX_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == (uint8_t)(state.index_x.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				// Test #4: DEY with index-y > 1
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_Y, {{10, 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_DEY_IMPLIED;
				command = COMMAND(COMMAND_TYPE_DEY_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == (state.index_y.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #5: DEY with index-y = 1
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_Y, {{1, 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_DEY_IMPLIED;
				command = COMMAND(COMMAND_TYPE_DEY_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == (state.index_y.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				// Test #6: DEY with index-y = 0
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0, 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_DEY_IMPLIED;
				command = COMMAND(COMMAND_TYPE_DEY_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == (uint8_t)(state.index_y.word - 1));
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_increment_index(void)
			{
				command_t command;
				c65_address_t address;
				processor_state_t state;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1: INX with index-x < 127
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_X, {{10, 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_INX_IMPLIED;
				command = COMMAND(COMMAND_TYPE_INX_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == (state.index_x.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2: INX with index-x = 127
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_X, {{0x7f, 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_INX_IMPLIED;
				command = COMMAND(COMMAND_TYPE_INX_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == (state.index_x.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				// Test #3: INX with index-x = 255
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_X, {{0xff, 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_INX_IMPLIED;
				command = COMMAND(COMMAND_TYPE_INX_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == (uint8_t)(state.index_x.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				// Test #4: INY with index-y < 127
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_Y, {{10, 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_INY_IMPLIED;
				command = COMMAND(COMMAND_TYPE_INY_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == (state.index_y.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #5: INY with index-y = 127
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0x7f, 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_INY_IMPLIED;
				command = COMMAND(COMMAND_TYPE_INY_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == (state.index_y.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				// Test #6: INY with index-y = 255
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_Y, {{0xff, 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_INY_IMPLIED;
				command = COMMAND(COMMAND_TYPE_INY_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == (uint8_t)(state.index_y.word + 1));
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_no_operation(void)
			{
				command_t command;
				c65_address_t address;
				processor_state_t state;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_NOP_IMPLIED;
				command = COMMAND(COMMAND_TYPE_NOP_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_pull(void)
			{
				c65_byte_t value;
				command_t command;
				processor_state_t state;
				c65_address_t program_counter, stack_pointer;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				program_counter.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
				stack_pointer.word = (ADDRESS_MEMORY_STACK_END - 1);

				// Test #1: PLA positive
				value = 10;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{(uint8_t)(value + 1), 0}});
				save_state(state);
				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = COMMAND_TYPE_PLA_STACK;
				command = COMMAND(COMMAND_TYPE_PLA_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == value);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2: PLA zero
				value = 0;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{(uint8_t)(value + 1), 0}});
				save_state(state);
				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = COMMAND_TYPE_PLA_STACK;
				command = COMMAND(COMMAND_TYPE_PLA_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == value);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				// Test #3: PLA negative
				value = 0x80;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{(uint8_t)(value + 1), 0}});
				save_state(state);
				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = COMMAND_TYPE_PLA_STACK;
				command = COMMAND(COMMAND_TYPE_PLA_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).low == value);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				// Test #4: PLP
				value = 10;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				save_state(state);
				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = COMMAND_TYPE_PLP_STACK;
				command = COMMAND(COMMAND_TYPE_PLP_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == value);

				// Test #5: PLX positive
				value = 10;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_INDEX_X, {{(uint8_t)(value + 1), 0}});
				save_state(state);
				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = COMMAND_TYPE_PLX_STACK;
				command = COMMAND(COMMAND_TYPE_PLX_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == value);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #6: PLX zero
				value = 0;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_INDEX_X, {{(uint8_t)(value + 1), 0}});
				save_state(state);
				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = COMMAND_TYPE_PLX_STACK;
				command = COMMAND(COMMAND_TYPE_PLX_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == value);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				// Test #7: PLX negative
				value = 0x80;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_INDEX_X, {{(uint8_t)(value + 1), 0}});
				save_state(state);
				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = COMMAND_TYPE_PLX_STACK;
				command = COMMAND(COMMAND_TYPE_PLX_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).low == value);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				// Test #8: PLY positive
				value = 10;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_INDEX_Y, {{(uint8_t)(value + 1), 0}});
				save_state(state);
				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = COMMAND_TYPE_PLY_STACK;
				command = COMMAND(COMMAND_TYPE_PLY_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).low == value);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #9: PLY zero
				value = 0;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_INDEX_Y, {{(uint8_t)(value + 1), 0}});
				save_state(state);
				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = COMMAND_TYPE_PLY_STACK;
				command = COMMAND(COMMAND_TYPE_PLY_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).low == value);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_ZERO)));

				// Test #10: PLY negative
				value = 0x80;
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
				instance.write_register(C65_REGISTER_STACK_POINTER, stack_pointer);
				instance.write_register(C65_REGISTER_INDEX_Y, {{(uint8_t)(value + 1), 0}});
				save_state(state);
				m_memory.at(stack_pointer.word + 1) = value;
				m_memory.at(program_counter.word) = COMMAND_TYPE_PLY_STACK;
				command = COMMAND(COMMAND_TYPE_PLY_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).low == value);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (program_counter.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word + 1));
				ASSERT(instance.read_status().raw == (state.status.raw | MASK(FLAG_NEGATIVE)));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_push(void)
			{
				command_t command;
				c65_address_t address;
				processor_state_t state;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1: PHA
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_ACCUMULATOR, {{(uint8_t)std::rand(), 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_PHA_STACK;
				command = COMMAND(COMMAND_TYPE_PHA_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word - 1));
				ASSERT(m_memory.at(state.stack_pointer.word) == state.accumulator.low);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #2: PHP
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status({{(uint8_t)std::rand()}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_PHP_STACK;
				command = COMMAND(COMMAND_TYPE_PHP_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word - 1));
				ASSERT(m_memory.at(state.stack_pointer.word) == state.status.raw);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #3: PHX
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_X, {{(uint8_t)std::rand(), 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_PHX_STACK;
				command = COMMAND(COMMAND_TYPE_PHX_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == (state.stack_pointer.word - 1));
				ASSERT(m_memory.at(state.stack_pointer.word) == state.index_x.low);
				ASSERT(instance.read_status().raw == state.status.raw);

				// Test #4: PHY
				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_register(C65_REGISTER_INDEX_Y, {{(uint8_t)std::rand(), 0}});
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_PHY_STACK;
				command = COMMAND(COMMAND_TYPE_PHY_STACK);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
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

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: RMB
				for(type = COMMAND_RMB0; type <= COMMAND_RMB7; ++type) {
					c65_byte_t index;
					command_t command;
					c65_address_t address;
					processor_state_t state;

					// Test #1.a: RMB with bit unset
					index = (COMMAND_TYPE_RMB0_ZERO_PAGE + ((type - COMMAND_RMB0) * 0x10));
					instance.reset(*this);
					address.word = 0xaa;
					m_memory.at(address.word) = 0;
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
					save_state(state);
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;
					command = COMMAND(index);
					ASSERT(instance.step(*this) == (command.cycle + 2));
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
					ASSERT(!m_memory.at(0xaa));

					// Test #1.b: RMB with bit set
					index = (COMMAND_TYPE_RMB0_ZERO_PAGE + ((type - COMMAND_RMB0) * 0x10));
					instance.reset(*this);
					address.word = 0xaa;
					m_memory.at(address.word) = MASK(type - COMMAND_RMB0);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
					save_state(state);
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;
					command = COMMAND(index);
					ASSERT(instance.step(*this) == (command.cycle + 2));
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
					ASSERT(!m_memory.at(0xaa));
				}

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_set(void)
			{
				command_t command;
				c65_status_t status;
				c65_address_t address;
				processor_state_t state;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				// Test #1: SEC
				instance.reset(*this);
				status = {};
				status.carry = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_SEC_IMPLIED;
				command = COMMAND(COMMAND_TYPE_SEC_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().carry);

				// Test #2: SED
				instance.reset(*this);
				status = {};
				status.decimal_mode = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_SED_IMPLIED;
				command = COMMAND(COMMAND_TYPE_SED_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().decimal_mode);

				// Test #3: SEI
				instance.reset(*this);
				status = {};
				status.interrupt_disable = false;
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				instance.write_status(status);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_SEI_IMPLIED;
				command = COMMAND(COMMAND_TYPE_SEI_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
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

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				// Test #1: SMB
				for(type = COMMAND_SMB0; type <= COMMAND_SMB7; ++type) {
					c65_byte_t index;
					command_t command;
					c65_address_t address;
					processor_state_t state;

					// Test #1.a: SMB with bit unset
					index = (COMMAND_TYPE_SMB0_ZERO_PAGE + ((type - COMMAND_SMB0) * 0x10));
					instance.reset(*this);
					address.word = 0xaa;
					m_memory.at(address.word) = 0;
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
					save_state(state);
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;
					command = COMMAND(index);
					ASSERT(instance.step(*this) == (command.cycle + 2));
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
					ASSERT(MASK_CHECK(m_memory.at(0xaa), type - COMMAND_SMB0));

					// Test #1.b: SMB with bit set
					index = (COMMAND_TYPE_SMB0_ZERO_PAGE + ((type - COMMAND_SMB0) * 0x10));
					instance.reset(*this);
					address.word = 0xaa;
					m_memory.at(address.word) = MASK(type - COMMAND_SMB0);
					address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);
					instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
					save_state(state);
					m_memory.at(address.word) = index;
					m_memory.at(address.word + 1) = 0xaa;
					command = COMMAND(index);
					ASSERT(instance.step(*this) == (command.cycle + 2));
					ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
					ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
					ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
					ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
					ASSERT(instance.read_status().raw == state.status.raw);
					ASSERT(MASK_CHECK(m_memory.at(0xaa), type - COMMAND_SMB0));
				}

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_stop(void)
			{
				command_t command;
				c65_address_t address;
				processor_state_t state;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_STP_IMPLIED;
				command = COMMAND(COMMAND_TYPE_STP_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(instance.stopped());

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_execute_wait(void)
			{
				command_t command;
				c65_address_t address;
				processor_state_t state;

				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				address.word = INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET);

				instance.reset(*this);
				instance.write_register(C65_REGISTER_PROGRAM_COUNTER, address);
				save_state(state);
				m_memory.at(address.word) = COMMAND_TYPE_WAI_IMPLIED;
				command = COMMAND(COMMAND_TYPE_WAI_IMPLIED);
				ASSERT(instance.step(*this) == command.cycle);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == state.accumulator.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == state.index_x.word);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == state.index_y.word);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == (address.word + command.length + 1));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == state.stack_pointer.word);
				ASSERT(instance.read_status().raw == state.status.raw);
				ASSERT(instance.waiting());

				instance.uninitialize();

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
							ASSERT(instance.step(*this) == (COMMAND_MODE_CYCLE(COMMAND_MODE_IMPLIED)
								+ COMMAND_MODE_CYCLE(COMMAND_MODE_INTERRUPT)));
							ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word
								== (INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_NON_MASKABLE) + 1));
							ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word
								== (ADDRESS_MEMORY_STACK_END - 3));
							status = instance.read_status();
							status.break_instruction = false;
							ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END - 2) == status.raw);
							ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END - 1) == program_counter.low);
							ASSERT(m_memory.at(ADDRESS_MEMORY_STACK_END) == program_counter.high);
							break;
						case C65_INTERRUPT_MASKABLE:

							// Test #1.a: IRQ with interrupts disabled
							instance.write_register(C65_REGISTER_PROGRAM_COUNTER, program_counter);
							instance.write_register(C65_REGISTER_STACK_POINTER, stack);
							instance.interrupt(type);
							ASSERT(instance.step(*this) == COMMAND_MODE_CYCLE(COMMAND_MODE_IMPLIED));
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
							ASSERT(instance.step(*this) == (COMMAND_MODE_CYCLE(COMMAND_MODE_IMPLIED)
								+ COMMAND_MODE_CYCLE(COMMAND_MODE_INTERRUPT)));
							ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word
								== (INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_MASKABLE) + 1));
							ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word
								== (ADDRESS_MEMORY_STACK_END - 3));
							status = instance.read_status();
							status.break_instruction = false;
							status.interrupt_disable = false;
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

				instance.initialize();

				instance.reset(*this);
				status.break_instruction = true;
				status.interrupt_disable = true;
				status.unused = true;
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

				instance.initialize();

				instance.reset(*this);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == 0);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == 0);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word ==
					INTERRUPT_VECTOR_ADDRESS(INTERRUPT_VECTOR_RESET));
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == ADDRESS_MEMORY_STACK_END);
				status.break_instruction = true;
				status.interrupt_disable = true;
				status.unused = true;
				ASSERT(instance.read_status().raw == status.raw);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_step(void)
			{
				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				ASSERT(instance.step(*this) == COMMAND_MODE_CYCLE(COMMAND_MODE_IMPLIED));

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
