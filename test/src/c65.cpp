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
			test_run();
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
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		runtime::test_cleanup(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		runtime::test_interrupt(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		runtime::test_load(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		runtime::test_reset(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		runtime::test_run(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		runtime::test_step(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		runtime::test_unload(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}
	}
}
