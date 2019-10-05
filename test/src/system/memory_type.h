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

#ifndef C65_TEST_SYSTEM_MEMORY_TYPE_H_
#define C65_TEST_SYSTEM_MEMORY_TYPE_H_

#include "../../../include/type/exception.h"

namespace c65 {

	namespace test {

		namespace system {

			#define C65_TEST_SYSTEM_MEMORY_HEADER "[C65::TEST::SYSTEM::MEMORY]"
#ifndef NDEBUG
			#define C65_TEST_SYSTEM_MEMORY_EXCEPTION_HEADER C65_TEST_SYSTEM_MEMORY_HEADER " "
#else
			#define C65_TEST_SYSTEM_MEMORY_EXCEPTION_HEADER
#endif // NDEBUG

			enum {
				C65_TEST_SYSTEM_MEMORY_EXCEPTION_NONE = 0,
			};

			#define C65_TEST_SYSTEM_MEMORY_EXCEPTION_MAX C65_TEST_SYSTEM_MEMORY_EXCEPTION_NONE

			static const std::string C65_TEST_SYSTEM_MEMORY_EXCEPTION_STR[] = {
				C65_TEST_SYSTEM_MEMORY_EXCEPTION_HEADER "", // C65_TEST_SYSTEM_MEMORY_EXCEPTION_NONE
				};

			#define C65_TEST_SYSTEM_MEMORY_EXCEPTION_STRING(_TYPE_) \
				(((_TYPE_) > C65_TEST_SYSTEM_MEMORY_EXCEPTION_MAX) ? STRING_UNKNOWN : \
					STRING(C65_TEST_SYSTEM_MEMORY_EXCEPTION_STR[_TYPE_]))

			#define THROW_C65_TEST_SYSTEM_MEMORY_EXCEPTION(_EXCEPT_) \
				THROW_C65_TEST_SYSTEM_MEMORY_EXCEPTION_FORMAT(_EXCEPT_, "", "")
			#define THROW_C65_TEST_SYSTEM_MEMORY_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
				THROW_EXCEPTION(C65_TEST_SYSTEM_MEMORY_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
		}
	}
}

#endif // C65_TEST_SYSTEM_MEMORY_TYPE_H_
