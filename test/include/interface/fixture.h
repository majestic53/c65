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

#ifndef C65_TEST_INTERFACE_FIXTURE_H_
#define C65_TEST_INTERFACE_FIXTURE_H_

#include "../../../include/type/exception.h"
#include "../common.h"

namespace c65 {

	namespace test {

		namespace interface {

			class fixture {

				public:

					int run(
						__inout std::string &error
						)
					{
						int result = TEST_INCONCLUSIVE;

						TRACE_ENTRY_FORMAT("Error=%p", &error);

						try {
							on_run();
							result = TEST_SUCCESS;
						} catch(c65::type::exception &exc) {
							error = exc.to_string();
							result = TEST_FAILURE;
						} catch(std::exception &exc) {
							error = exc.what();
							result = TEST_FAILURE;
						}

						TRACE_EXIT_FORMAT("Result=%i(%s)", result, TEST_STRING(result));
						return result;
					}

					int setup(
						__inout std::string &error
						)
					{
						int result = TEST_INCONCLUSIVE;

						TRACE_ENTRY_FORMAT("Error=%p", &error);

						try {
							on_setup();
							result = TEST_SUCCESS;
						} catch(c65::type::exception &exc) {
							error = exc.to_string();
							result = TEST_FAILURE;
						} catch(std::exception &exc) {
							error = exc.what();
							result = TEST_FAILURE;
						}

						TRACE_EXIT_FORMAT("Result=%i(%s)", result, TEST_STRING(result));
						return result;
					}

					int teardown(
						__inout std::string &error
						)
					{
						int result = TEST_INCONCLUSIVE;

						TRACE_ENTRY_FORMAT("Error=%p", &error);

						try {
							on_teardown();
							result = TEST_SUCCESS;
						} catch(c65::type::exception &exc) {
							error = exc.to_string();
							result = TEST_FAILURE;
						} catch(std::exception &exc) {
							error = exc.what();
							result = TEST_FAILURE;
						}

						TRACE_EXIT_FORMAT("Result=%i(%s)", result, TEST_STRING(result));
						return result;
					}

				protected:

					virtual void on_run(void) = 0;

					virtual void on_setup(void) = 0;

					virtual void on_teardown(void) = 0;
			};
		}
	}
}

#endif // C65_TEST_INTERFACE_FIXTURE_H_
