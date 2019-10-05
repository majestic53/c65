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

#ifndef C65_TEST_TYPE_BUFFER_H_
#define C65_TEST_TYPE_BUFFER_H_

#include "../interface/fixture.h"

namespace c65 {

	namespace test {

		namespace type {

			class buffer :
					public c65::test::interface::fixture {

				public:

					buffer(void);

					~buffer(void);

				protected:

					buffer(
						__in const buffer &other
						) = delete;

					buffer &operator=(
						__in const buffer &other
						) = delete;

					void create_file(
						__in const std::string &path,
						__in_opt const std::vector<c65_byte_t> &data = std::vector<c65_byte_t>()
						);

					void destroy_file(
						__in const std::string &path
						);

					void on_run(void) override;

					void on_setup(void) override;

					void on_teardown(void) override;

					void test_load_file(void);
			};
		}
	}
}

#endif // C65_TEST_TYPE_BUFFER_H_
