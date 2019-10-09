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

#ifndef C65_TEST_SYSTEM_PROCESSOR_H_
#define C65_TEST_SYSTEM_PROCESSOR_H_

#include "../../../include/interface/bus.h"
#include "../interface/fixture.h"

namespace c65 {

	namespace test {

		namespace system {

			class processor :
					public c65::interface::bus,
					public c65::test::interface::fixture {

				public:

					processor(void);

					~processor(void);

				protected:

					processor(
						__in const processor &other
						) = delete;

					processor &operator=(
						__in const processor &other
						) = delete;

					c65_byte_t on_read(
						__in c65_address_t address
						) const override;

					void on_run(void) override;

					void on_setup(void) override;

					void on_teardown(void) override;

					void on_write(
						__in c65_address_t address,
						__in c65_byte_t value
						) override;

					void test_interrupt(void);

					void test_interrupt_pending(void);

					void test_read(void);

					void test_read_register(void);

					void test_read_status(void);

					void test_reset(void);

					void test_step(void);

					void test_stopped(void);

					void test_waiting(void);

					void test_write(void);

					void test_write_register(void);

					void test_write_status(void);

					std::vector<c65_byte_t> m_memory;
			};
		}
	}
}

#endif // C65_TEST_SYSTEM_PROCESSOR_H_
