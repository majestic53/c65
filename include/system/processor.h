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

#ifndef C65_SYSTEM_PROCESSOR_H_
#define C65_SYSTEM_PROCESSOR_H_

#include "../interface/bus.h"
#include "../interface/singleton.h"

namespace c65 {

	namespace system {

		class processor :
				public c65::interface::bus,
				public c65::interface::singleton<c65::system::processor> {

			public:

				~processor(void);

				void interrupt(
					__in int type
					);

				bool interrupt_pending(void) const;

				c65_register_t read_register(
					__in int type
					) const;

				c65_status_t read_status(void) const;

				void reset(
					__in c65::interface::bus &bus
					);

				bool stack_overflow(void) const;

				bool stack_underflow(void) const;

				uint8_t step(
					__in c65::interface::bus &bus
					);

				bool stopped(void) const;

				bool waiting(void) const;

				void write_register(
					__in int type,
					__in c65_register_t value
					);

				void write_status(
					__in c65_status_t value
					);

			protected:

				friend class c65::interface::singleton<c65::system::processor>;

				processor(void);

				processor(
					__in const processor &other
					) = delete;

				processor &operator=(
					__in const processor &other
					) = delete;

				void calculate_sum(
					__in uint8_t value
					);

				c65_address_t effective_address(
					__in c65::interface::bus &bus,
					__in c65_word_t indirect,
					__in int mode,
					__inout uint8_t &cycle
					);

				uint8_t execute(
					__in c65::interface::bus &bus
					);

				uint8_t execute_add(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_and(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_bit(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_branch(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_branch_bit(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_break(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction
					);

				uint8_t execute_clear(
					__in const instruction_t &instruction
					);

				uint8_t execute_compare(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_compare_index_x(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_compare_index_y(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_decrement(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_decrement_index(
					__in const instruction_t &instruction
					);

				uint8_t execute_increment(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_increment_index(
					__in const instruction_t &instruction
					);

				uint8_t execute_jump(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_jump_subroutine(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_load_accumulator(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_load_index_x(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_load_index_y(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_no_operation(void);

				uint8_t execute_no_operation(
					__in const instruction_t &instruction
					);

				uint8_t execute_or(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_pull(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction
					);

				uint8_t execute_push(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction
					);

				uint8_t execute_reset_bit(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_return_interrupt(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction
					);

				uint8_t execute_return_subroutine(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction
					);

				uint8_t execute_rotate_left(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_rotate_right(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_set(
					__in const instruction_t &instruction
					);

				uint8_t execute_set_bit(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_shift_left(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_shift_right(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_stop(
					__in const c65::interface::bus &bus,
					__in const instruction_t &instruction
					);

				uint8_t execute_store_accumulator(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_store_index_x(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_store_index_y(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_store_zero(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_subtract(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_test_reset_bit(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_test_set_bit(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				uint8_t execute_transfer(
					__in const instruction_t &instruction
					);

				uint8_t execute_wait(
					__in const c65::interface::bus &bus,
					__in const instruction_t &instruction
					);

				uint8_t execute_xor(
					__in c65::interface::bus &bus,
					__in const instruction_t &instruction,
					__in c65_word_t operand
					);

				void on_initialize(void) override;

				void on_notify(
					__in const c65_event_t &event
					) const override;

				c65_byte_t on_read(
					__in c65_address_t address
					) const override;

				void on_uninitialize(void) override;

				void on_write(
					__in c65_address_t address,
					__in c65_byte_t value
					) override;

				c65_byte_t pull_byte(
					__in const c65::interface::bus &bus
					);

				c65_word_t pull_word(
					__in const c65::interface::bus &bus
					);

				void push_byte(
					__in c65::interface::bus &bus,
					__in c65_byte_t value
					);

				void push_word(
					__in c65::interface::bus &bus,
					__in c65_word_t value
					);

				c65_byte_t read_byte(
					__in const c65::interface::bus &bus,
					__in c65_address_t address
					) const;

				c65_word_t read_word(
					__in const c65::interface::bus &bus,
					__in c65_address_t address
					) const;

				uint8_t service(
					__in c65::interface::bus &bus
					);

				uint8_t service_interrupt(
					__in c65::interface::bus &bus,
					__in c65_address_t address,
					__in bool breakpoint
					);

				void write_byte(
					__in c65::interface::bus &bus,
					__in c65_address_t address,
					__in c65_byte_t value
					);

				void write_word(
					__in c65::interface::bus &bus,
					__in c65_address_t address,
					__in c65_word_t value
					);

				c65_register_t m_accumulator;

				c65_register_t m_index_x;

				c65_register_t m_index_y;

				int m_interrupt;

				c65_address_t m_maskable;

				c65_address_t m_non_maskable;

				c65_register_t m_program_counter;

				c65_address_t m_reset;

				int m_stack;

				c65_register_t m_stack_pointer;

				c65_status_t m_status;

				bool m_stop;

				bool m_wait;
		};
	}
}

#endif // C65_SYSTEM_PROCESSOR_H_
