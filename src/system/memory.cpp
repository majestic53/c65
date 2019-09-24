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

#include "../../include/system/memory.h"
#include "./memory_type.h"

namespace c65 {

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
		memory::on_initialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Memory initializing");

			m_high.resize(ADDRESS_LENGTH(ADDRESS_MEMORY_HIGH_BEGIN, ADDRESS_MEMORY_HIGH_END), MEMORY_FILL);
			m_stack.resize(ADDRESS_LENGTH(ADDRESS_MEMORY_STACK_BEGIN, ADDRESS_MEMORY_STACK_END), MEMORY_ZERO);
			m_zero_page.resize(ADDRESS_LENGTH(ADDRESS_MEMORY_ZERO_PAGE_BEGIN, ADDRESS_MEMORY_ZERO_PAGE_END), MEMORY_ZERO);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Memory initialized");

			TRACE_EXIT();
		}

		c65_byte_t
		memory::on_read(
			__in c65_address_t address
			) const
		{
			c65_byte_t result = MEMORY_ZERO;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address.word, address.word);

			switch(address.word) {
				case ADDRESS_MEMORY_HIGH_BEGIN ... ADDRESS_MEMORY_HIGH_END:
					result = m_high.at(address.word - ADDRESS_MEMORY_HIGH_BEGIN);
					break;
				case ADDRESS_MEMORY_STACK_BEGIN ... ADDRESS_MEMORY_STACK_END:
					result = m_stack.at(address.word - ADDRESS_MEMORY_STACK_BEGIN);
					break;
				case ADDRESS_MEMORY_ZERO_PAGE_BEGIN ... ADDRESS_MEMORY_ZERO_PAGE_END:
					result = m_zero_page.at(address.word - ADDRESS_MEMORY_ZERO_PAGE_BEGIN);
					break;
				default:
					THROW_C65_SYSTEM_MEMORY_EXCEPTION_FORMAT(C65_SYSTEM_MEMORY_EXCEPTION_ADDRESS_INVALID,
						"%u(%04x)", address.word, address.word);
			}

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		memory::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Memory uninitializing");

			m_high.clear();
			m_stack.clear();
			m_zero_page.clear();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Memory uninitialized");

			TRACE_EXIT();
		}

		void
		memory::on_write(
			__in c65_address_t address,
			__in c65_byte_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address.word, address.word, value, value);

			switch(address.word) {
				case ADDRESS_MEMORY_HIGH_BEGIN ... ADDRESS_MEMORY_HIGH_END:
					m_high.at(address.word - ADDRESS_MEMORY_HIGH_BEGIN) = value;
					break;
				case ADDRESS_MEMORY_STACK_BEGIN ... ADDRESS_MEMORY_STACK_END:
					m_stack.at(address.word - ADDRESS_MEMORY_STACK_BEGIN) = value;
					break;
				case ADDRESS_MEMORY_ZERO_PAGE_BEGIN ... ADDRESS_MEMORY_ZERO_PAGE_END:
					m_zero_page.at(address.word - ADDRESS_MEMORY_ZERO_PAGE_BEGIN) = value;
					break;
				default:
					THROW_C65_SYSTEM_MEMORY_EXCEPTION_FORMAT(C65_SYSTEM_MEMORY_EXCEPTION_ADDRESS_INVALID,
						"%u(%04x)", address.word, address.word);
			}

			TRACE_EXIT();
		}
	}
}
