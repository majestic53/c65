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

#include "../../../include/system/memory.h"
#include "../../include/system/memory.h"
#include "./memory_type.h"

namespace c65 {

	namespace test {

		namespace system {

			memory::memory(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			memory::~memory(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			void
			memory::on_run(
				__in bool quiet
				)
			{
				TRACE_ENTRY_FORMAT("Quiet=%x", quiet);

				EXECUTE_TEST(test_read, quiet);
				EXECUTE_TEST(test_write, quiet);

				TRACE_EXIT();
			}

			void
			memory::on_setup(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			void
			memory::on_teardown(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			void
			memory::test_read(void)
			{
				c65_address_t address = {};

				TRACE_ENTRY();

				c65::system::memory &instance = c65::system::memory::instance();

				instance.initialize();

				do {

					switch(address.word) {

						// Test #1: Valid range
						case ADDRESS_MEMORY_HIGH_BEGIN ... ADDRESS_MEMORY_HIGH_END:
							ASSERT(instance.read(address) == MEMORY_FILL);
							break;
						case ADDRESS_MEMORY_STACK_BEGIN ... ADDRESS_MEMORY_STACK_END:
						case ADDRESS_MEMORY_ZERO_PAGE_BEGIN ... ADDRESS_MEMORY_ZERO_PAGE_END:
							ASSERT(instance.read(address) == MEMORY_ZERO);
							break;

						// Test #2: Invalid range
						default:

							try {
								instance.read(address);
								ASSERT(false);
							} catch(...) { }

							break;
					}

					++address.word;
				} while(address.word);

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			memory::test_write(void)
			{
				c65_address_t address = {};
				c65_byte_t value = std::rand();

				TRACE_ENTRY();

				c65::system::memory &instance = c65::system::memory::instance();

				instance.initialize();

				do {

					switch(address.word) {

						// Test #1: Valid range
						case ADDRESS_MEMORY_HIGH_BEGIN ... ADDRESS_MEMORY_HIGH_END:
							instance.write(address, value);
							ASSERT(instance.read(address) == value);
							break;
						case ADDRESS_MEMORY_STACK_BEGIN ... ADDRESS_MEMORY_STACK_END:
						case ADDRESS_MEMORY_ZERO_PAGE_BEGIN ... ADDRESS_MEMORY_ZERO_PAGE_END:
							instance.write(address, value);
							ASSERT(instance.read(address) == value);
							break;

						// Test #2: Invalid range
						default:

							try {
								instance.write(address, value);
								ASSERT(false);
							} catch(...) { }

							break;
					}

					++address.word;
				} while(address.word);

				instance.uninitialize();

				TRACE_EXIT();
			}
		}
	}
}
