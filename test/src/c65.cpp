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

#include "../include/c65.h"
#include "./c65_type.h"

static void c65_event_handler(
	__in const c65_event_t *event
	)
{
	return;
}

namespace c65 {

	namespace test {

		runtime::runtime(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		runtime::~runtime(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		void
		runtime::on_run(void)
		{
			TRACE_ENTRY();

			test_action();
			test_cleanup();
			test_event_handler();
			test_interrupt();
			test_load();
			test_reset();
			test_step();
			test_unload();

			TRACE_EXIT();
		}

		void
		runtime::on_setup(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		void
		runtime::on_teardown(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		void
		runtime::test_action(void)
		{
			int type;
			c65_status_t status = {};
			c65_register_t value = {};
			c65_address_t address = {};
			c65_action_t request = {}, response = {};

			TRACE_ENTRY();

			address.word = ((std::rand() % ADDRESS_LENGTH(ADDRESS_MEMORY_HIGH_BEGIN, ADDRESS_MEMORY_HIGH_END))
						+ ADDRESS_MEMORY_HIGH_BEGIN);
			value.word = std::rand();

			// Test #1: Breakpoint clear action
			request.type = C65_ACTION_BREAKPOINT_CLEAR;
			ASSERT(c65_action(&request, &response) == EXIT_FAILURE);
			ASSERT(response.type == C65_ACTION_BREAKPOINT_CLEAR);

			request.type = C65_ACTION_BREAKPOINT_SET;
			request.address.word = value.word;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_BREAKPOINT_SET);

			request.type = C65_ACTION_BREAKPOINT_CLEAR;
			request.address.word = value.word;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_BREAKPOINT_CLEAR);

			// Test #2: Breakpoint set action
			request.type = C65_ACTION_BREAKPOINT_SET;
			request.address.word = value.word;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_BREAKPOINT_SET);

			request.type = C65_ACTION_BREAKPOINT_SET;
			request.address.word = value.word;
			ASSERT(c65_action(&request, &response) == EXIT_FAILURE);
			ASSERT(response.type == C65_ACTION_BREAKPOINT_SET);

			// Test #3: Cycle action
			request.type = C65_ACTION_CYCLE;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_CYCLE);
			ASSERT(!response.cycle);
			ASSERT(c65_reset() == EXIT_SUCCESS);
			ASSERT(c65_step() == EXIT_SUCCESS);
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_CYCLE);
			ASSERT(response.cycle);

			// Test #4: Interrupt pending action
			request.type = C65_ACTION_INTERRUPT_PENDING;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_INTERRUPT_PENDING);
			ASSERT(!response.data.low);
			ASSERT(c65_interrupt(C65_INTERRUPT_MASKABLE) == EXIT_SUCCESS);
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_INTERRUPT_PENDING);
			ASSERT(response.data.low);
			ASSERT(c65_interrupt(C65_INTERRUPT_NON_MASKABLE) == EXIT_SUCCESS);
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_INTERRUPT_PENDING);
			ASSERT(response.data.low);

			// Test #5: Read byte action
			ASSERT(c65_load((c65_byte_t *)&value.low, INSTRUCTION_LENGTH_BYTE, address) == EXIT_SUCCESS);

			request.type = C65_ACTION_READ_BYTE;
			request.address = address;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_READ_BYTE);
			ASSERT(response.data.low == value.low);
			ASSERT(c65_unload(address, INSTRUCTION_LENGTH_BYTE) == EXIT_SUCCESS);

			// Test #6: Read register action
			ASSERT(c65_reset() == EXIT_SUCCESS);
			request.type = C65_ACTION_READ_REGISTER;

			for(type = 0; type <= C65_REGISTER_MAX; ++type) {
				request.address.word = type;
				ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
				ASSERT(response.type == C65_ACTION_READ_REGISTER);

				switch(type) {
					case C65_REGISTER_ACCUMULATOR:
					case C65_REGISTER_INDEX_X:
					case C65_REGISTER_INDEX_Y:
						ASSERT(response.data.low == 0);
						break;
					case C65_REGISTER_PROGRAM_COUNTER:
						ASSERT(response.data.word == ADDRESS_MEMORY_HIGH_BEGIN);
						break;
					case C65_REGISTER_STACK_POINTER:
						ASSERT(response.data.word == ADDRESS_MEMORY_STACK_END);
						break;
					default:
						break;
				}
			}

			// Test #7: Read status action
			ASSERT(c65_reset() == EXIT_SUCCESS);

			request.type = C65_ACTION_READ_STATUS;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_READ_STATUS);
			status.break_instruction = true;
			status.interrupt_disable = true;
			status.unused = true;
			ASSERT(response.status.raw == status.raw);

			// Test #8: Read word action
			ASSERT(c65_load((c65_byte_t *)&value.word, INSTRUCTION_LENGTH_WORD, address) == EXIT_SUCCESS);

			request.type = C65_ACTION_READ_WORD;
			request.address = address;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_READ_WORD);
			ASSERT(response.data.word == value.word);
			ASSERT(c65_unload(address, INSTRUCTION_LENGTH_WORD) == EXIT_SUCCESS);

			// Test #9: Stack overflow action
			request.type = C65_ACTION_STACK_OVERFLOW;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_STACK_OVERFLOW);
			ASSERT(!response.data.low);

			// Test #10: Stack underflow action
			request.type = C65_ACTION_STACK_UNDERFLOW;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_STACK_UNDERFLOW);
			ASSERT(!response.data.low);

			// Test #11: Stopped action
			request.type = C65_ACTION_STOPPED;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_STOPPED);
			ASSERT(!response.data.low);

			// Test #12: Waiting action
			request.type = C65_ACTION_WAITING;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_WAITING);
			ASSERT(!response.data.low);

			// Test #13: Watch clear action
			request.type = C65_ACTION_WATCH_CLEAR;
			ASSERT(c65_action(&request, &response) == EXIT_FAILURE);
			ASSERT(response.type == C65_ACTION_WATCH_CLEAR);

			request.type = C65_ACTION_WATCH_SET;
			request.address.word = value.word;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_WATCH_SET);

			request.type = C65_ACTION_WATCH_CLEAR;
			request.address.word = value.word;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_WATCH_CLEAR);

			// Test #14: Watch set action
			request.type = C65_ACTION_WATCH_SET;
			request.address.word = value.word;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_WATCH_SET);

			request.type = C65_ACTION_WATCH_SET;
			request.address.word = value.word;
			ASSERT(c65_action(&request, &response) == EXIT_FAILURE);
			ASSERT(response.type == C65_ACTION_WATCH_SET);

			// Test #15: Write byte action
			request.type = C65_ACTION_WRITE_BYTE;
			request.address = address;
			request.data.low = value.low;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_WRITE_BYTE);

			request.type = C65_ACTION_READ_BYTE;
			request.address = address;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_READ_BYTE);
			ASSERT(response.data.low == value.low);
			ASSERT(c65_unload(address, INSTRUCTION_LENGTH_BYTE) == EXIT_SUCCESS);

			// Test #16: Write register action
			for(type = 0; type <= C65_REGISTER_MAX; ++type) {
				request.type = C65_ACTION_WRITE_REGISTER;
				request.address.word = type;

				switch(type) {
					case C65_REGISTER_ACCUMULATOR:
					case C65_REGISTER_INDEX_X:
					case C65_REGISTER_INDEX_Y:
						request.data.low = value.low;
						break;
					case C65_REGISTER_PROGRAM_COUNTER:
					case C65_REGISTER_STACK_POINTER:
						request.data.word = value.word;
						break;
					default:
						break;
				}

				ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
				ASSERT(response.type == C65_ACTION_WRITE_REGISTER);

				request.type = C65_ACTION_READ_REGISTER;
				request.address.word = type;
				ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
				ASSERT(response.type == C65_ACTION_READ_REGISTER);

				switch(type) {
					case C65_REGISTER_ACCUMULATOR:
					case C65_REGISTER_INDEX_X:
					case C65_REGISTER_INDEX_Y:
						ASSERT(response.data.low == value.low);
						break;
					case C65_REGISTER_PROGRAM_COUNTER:
						ASSERT(response.data.word == value.word);
						break;
					case C65_REGISTER_STACK_POINTER:
						ASSERT(response.data.low == value.low);
						break;
					default:
						break;
				}
			}

			// Test #17: Write status action
			request.type = C65_ACTION_WRITE_STATUS;
			request.status.raw = value.low;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_WRITE_STATUS);

			request.type = C65_ACTION_READ_STATUS;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_READ_STATUS);
			ASSERT(response.status.raw == value.low);

			// Test #18: Write word action
			request.type = C65_ACTION_WRITE_WORD;
			request.address = address;
			request.data = value;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_WRITE_WORD);

			request.type = C65_ACTION_READ_WORD;
			request.address = address;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_READ_WORD);
			ASSERT(response.data.word == value.word);
			ASSERT(c65_unload(address, INSTRUCTION_LENGTH_WORD) == EXIT_SUCCESS);

			TRACE_EXIT();
		}

		void
		runtime::test_cleanup(void)
		{
			TRACE_ENTRY();

			c65_cleanup();

			TRACE_EXIT();
		}

		void
		runtime::test_event_handler(void)
		{
			int type = 0;

			TRACE_ENTRY();

			// Test #1: Valid event
			for(; type <= C65_EVENT_MAX; ++type) {
				ASSERT(c65_event_handler(type, nullptr) == EXIT_SUCCESS);
				ASSERT(c65_event_handler(type, c65_event_handler) == EXIT_SUCCESS);
			}

			// Test #2: Invalid event
			ASSERT(c65_event_handler(type, nullptr) == EXIT_FAILURE);
			ASSERT(c65_event_handler(type, c65_event_handler) == EXIT_FAILURE);

			TRACE_EXIT();
		}

		void
		runtime::test_interrupt(void)
		{
			int type = 0;

			TRACE_ENTRY();

			// Test #1: Valid interrupt
			for(; type <= C65_INTERRUPT_MAX; ++type) {
				ASSERT(c65_interrupt(type) == EXIT_SUCCESS);
			}

			// Test #2: Invalid interrupt
			ASSERT(c65_interrupt(type) == EXIT_FAILURE);

			TRACE_EXIT();
		}

		void
		runtime::test_load(void)
		{
			std::vector<c65_byte_t> data;
			c65_address_t address = {}, length = {};

			TRACE_ENTRY();

			length.word = ADDRESS_LENGTH(ADDRESS_MEMORY_HIGH_BEGIN, ADDRESS_MEMORY_HIGH_END);
			data.resize(length.word, MEMORY_ZERO);

			for(address.word = 0; address.word < length.word; ++address.word) {
				data.at(address.word) = std::rand();
			}

			// Test #1: Valid load
			address.word = ADDRESS_MEMORY_HIGH_BEGIN;
			ASSERT(c65_load((c65_byte_t *)&data[0], length.word, address) == EXIT_SUCCESS);

			for(; address.word <= ADDRESS_MEMORY_HIGH_END; ++address.word) {
				c65_action_t request = {}, response = {};

				request.type = C65_ACTION_READ_BYTE;
				request.address = address;
				ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
				ASSERT(response.data.low == data.at(address.word - ADDRESS_MEMORY_HIGH_BEGIN));
			}

			// Test #2: Invalid load
			try {
				address.word = ADDRESS_RANDOM;
				c65_load((c65_byte_t *)&data[0], INSTRUCTION_LENGTH_BYTE, address);
				ASSERT(false);
			} catch(...) { }

			try {
				address.word = ADDRESS_KEY;
				c65_load((c65_byte_t *)&data[0], INSTRUCTION_LENGTH_BYTE, address);
				ASSERT(false);
			} catch(...) { }

			address.word = ADDRESS_MEMORY_HIGH_BEGIN;
			ASSERT(c65_unload(address, length.word) == EXIT_SUCCESS);

			TRACE_EXIT();
		}

		void
		runtime::test_reset(void)
		{
			TRACE_ENTRY();

			ASSERT(c65_reset() == EXIT_SUCCESS);

			TRACE_EXIT();
		}

		void
		runtime::test_step(void)
		{
			c65_byte_t value;
			c65_action_t request = {}, response = {};

			TRACE_ENTRY();

			request.type = C65_ACTION_READ_BYTE;
			request.address.word = ADDRESS_RANDOM;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			value = response.data.low;

			// Test #1: Step instruction
			ASSERT(c65_step() == EXIT_SUCCESS);

			// Test #2: Check random value
			request.type = C65_ACTION_READ_BYTE;
			request.address.word = ADDRESS_RANDOM;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);

			try {
				ASSERT(response.data.low != value);
			} catch(...) { }

			TRACE_EXIT();
		}

		void
		runtime::test_unload(void)
		{
			c65_address_t address = {}, length = {};

			TRACE_ENTRY();

			// Test #1: Valid unload
			address.word = ADDRESS_MEMORY_HIGH_BEGIN;
			length.word = ADDRESS_LENGTH(ADDRESS_MEMORY_HIGH_BEGIN, ADDRESS_MEMORY_HIGH_END);
			ASSERT(c65_unload(address, length.word) == EXIT_SUCCESS);

			// Test #2: Invalid unload
			try {
				address.word = ADDRESS_RANDOM;
				c65_unload(address, INSTRUCTION_LENGTH_BYTE);
				ASSERT(false);
			} catch(...) { }

			try {
				address.word = ADDRESS_KEY;
				c65_unload(address, INSTRUCTION_LENGTH_BYTE);
				ASSERT(false);
			} catch(...) { }

			TRACE_EXIT();
		}
	}
}
