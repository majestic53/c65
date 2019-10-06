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
			c65_register_t value = {};
			c65_address_t address = {};
			c65_action_t request = {}, response = {};

			TRACE_ENTRY();

			address.word = ((std::rand() % ADDRESS_LENGTH(ADDRESS_MEMORY_HIGH_BEGIN, ADDRESS_MEMORY_HIGH_END))
						+ ADDRESS_MEMORY_HIGH_BEGIN);
			value.word = std::rand();

			// Test #1: Read byte action
			ASSERT(c65_load((c65_byte_t *)&value.low, COMMAND_LENGTH_BYTE, address) == EXIT_SUCCESS);
			request.type = C65_ACTION_READ_BYTE;
			request.address = address;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_READ_BYTE);
			ASSERT(response.data.low == value.low);
			ASSERT(c65_unload(address, COMMAND_LENGTH_BYTE) == EXIT_SUCCESS);

			// Test #2: Read register action
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

			// Test #3: Read status action
			ASSERT(c65_reset() == EXIT_SUCCESS);
			request.type = C65_ACTION_READ_STATUS;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_READ_STATUS);
			ASSERT(response.status.raw == (MASK(FLAG_BREAK_INSTRUCTION) | MASK(FLAG_INTERRUPT_DISABLE) | MASK(FLAG_UNUSED)));

			// Test #4: Read word action
			ASSERT(c65_load((c65_byte_t *)&value.word, COMMAND_LENGTH_WORD, address) == EXIT_SUCCESS);
			request.type = C65_ACTION_READ_WORD;
			request.address = address;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_READ_WORD);
			ASSERT(response.data.word == value.word);
			ASSERT(c65_unload(address, COMMAND_LENGTH_WORD) == EXIT_SUCCESS);

			// Test #5: Write byte action
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
			ASSERT(c65_unload(address, COMMAND_LENGTH_BYTE) == EXIT_SUCCESS);

			// Test #6: Write register action
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
					case C65_REGISTER_STACK_POINTER:
						ASSERT(response.data.word == value.word);
						break;
					default:
						break;
				}
			}

			// Test #7: Write status action
			request.type = C65_ACTION_WRITE_STATUS;
			request.status.raw = value.low;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_WRITE_STATUS);

			request.type = C65_ACTION_READ_STATUS;
			ASSERT(c65_action(&request, &response) == EXIT_SUCCESS);
			ASSERT(response.type == C65_ACTION_READ_STATUS);
			ASSERT(response.status.raw == value.low);

			// Test #8: Write word action
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
			ASSERT(c65_unload(address, COMMAND_LENGTH_WORD) == EXIT_SUCCESS);

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
				c65_load((c65_byte_t *)&data[0], COMMAND_LENGTH_BYTE, address);
				ASSERT(false);
			} catch(...) { }

			try {
				address.word = ADDRESS_KEY;
				c65_load((c65_byte_t *)&data[0], COMMAND_LENGTH_BYTE, address);
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
			TRACE_ENTRY();

			ASSERT(c65_step() == EXIT_SUCCESS);

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
				c65_unload(address, COMMAND_LENGTH_BYTE);
				ASSERT(false);
			} catch(...) { }

			try {
				address.word = ADDRESS_KEY;
				c65_unload(address, COMMAND_LENGTH_BYTE);
				ASSERT(false);
			} catch(...) { }

			TRACE_EXIT();
		}
	}
}
