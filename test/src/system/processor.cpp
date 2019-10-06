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
				c65_byte_t result = MEMORY_ZERO;

				TRACE_ENTRY_FORMAT("Address=%u(%04x)", address.word, address.word);
				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			void
			processor::on_run(void)
			{
				TRACE_ENTRY();

				test_interrupt();
				test_read();
				test_read_register();
				test_read_status();
				test_reset();
				test_step();
				test_write();
				test_write_register();
				test_write_status();

				TRACE_EXIT();
			}

			void
			processor::on_setup(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			void
			processor::on_teardown(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			void
			processor::on_write(
				__in c65_address_t address,
				__in c65_byte_t value
				)
			{
				TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address.word, address.word, value, value);
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
					c65_register_t value = {};

					instance.reset(*this);

					switch(type) {
						case C65_INTERRUPT_NON_MASKABLE:
							value.word = UINT16_MAX;
							instance.write_register(C65_REGISTER_PROGRAM_COUNTER, value);
							instance.interrupt(type);
							ASSERT(instance.step(*this) == 9);
							value.word = 0;
							ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == value.word);
							break;
						case C65_INTERRUPT_MASKABLE:

							// Test #1.a: IRQ with interrupts disabled
							value.word = UINT16_MAX;
							instance.write_register(C65_REGISTER_PROGRAM_COUNTER, value);
							instance.interrupt(type);
							ASSERT(instance.step(*this) == 2);
							ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == value.word);

							// Test #1.b: IRQ with interrupts enabled
							status.raw = (instance.read_status().raw & ~MASK(FLAG_INTERRUPT_DISABLE));
							instance.write_status(status);
							instance.interrupt(type);
							ASSERT(instance.step(*this) == 9);
							value.word = 0;
							ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == value.word);
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
				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				instance.reset(*this);
				ASSERT(instance.read_status().raw == (MASK(FLAG_BREAK_INSTRUCTION) | MASK(FLAG_INTERRUPT_DISABLE)
					| MASK(FLAG_UNUSED)));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_reset(void)
			{
				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				instance.reset(*this);
				ASSERT(instance.read_register(C65_REGISTER_ACCUMULATOR).word == 0);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_X).word == 0);
				ASSERT(instance.read_register(C65_REGISTER_INDEX_Y).word == 0);
				ASSERT(instance.read_register(C65_REGISTER_PROGRAM_COUNTER).word == ADDRESS_MEMORY_HIGH_BEGIN);
				ASSERT(instance.read_register(C65_REGISTER_STACK_POINTER).word == ADDRESS_MEMORY_STACK_END);
				ASSERT(instance.read_status().raw == (MASK(FLAG_BREAK_INSTRUCTION) | MASK(FLAG_INTERRUPT_DISABLE)
					| MASK(FLAG_UNUSED)));

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			processor::test_step(void)
			{
				TRACE_ENTRY();

				c65::system::processor &instance = c65::system::processor::instance();

				instance.initialize();

				ASSERT(instance.step(*this) == 2);

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
						case C65_REGISTER_STACK_POINTER:
							ASSERT(instance.read_register(type).word == value.word);
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