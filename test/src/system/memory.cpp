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
			memory::on_run(void)
			{
				TRACE_ENTRY();

				test_read();
				test_write();

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
				TRACE_ENTRY();

				// TODO

				TRACE_EXIT();
			}

			void
			memory::test_write(void)
			{
				TRACE_ENTRY();

				// TODO

				TRACE_EXIT();
			}
		}
	}
}
