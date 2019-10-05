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

#ifndef C65_TEST_TYPE_BUFFER_TYPE_H_
#define C65_TEST_TYPE_BUFFER_TYPE_H_

#include "../../../include/type/exception.h"

namespace c65 {

	namespace test {

		namespace type {

			#define C65_TEST_TYPE_BUFFER_HEADER "[C65::TEST::TYPE::BUFFER]"
#ifndef NDEBUG
			#define C65_TEST_TYPE_BUFFER_EXCEPTION_HEADER C65_TEST_TYPE_BUFFER_HEADER " "
#else
			#define C65_TEST_TYPE_BUFFER_EXCEPTION_HEADER
#endif // NDEBUG

			enum {
				C65_TEST_TYPE_BUFFER_EXCEPTION_FILE_INVALID = 0,
			};

			#define C65_TEST_TYPE_BUFFER_EXCEPTION_MAX C65_TEST_TYPE_BUFFER_EXCEPTION_FILE_INVALID

			static const std::string C65_TEST_TYPE_BUFFER_EXCEPTION_STR[] = {
				C65_TEST_TYPE_BUFFER_EXCEPTION_HEADER "Invalid file", // C65_TEST_TYPE_BUFFER_EXCEPTION_FILE_INVALID
				};

			#define C65_TEST_TYPE_BUFFER_EXCEPTION_STRING(_TYPE_) \
				(((_TYPE_) > C65_TEST_TYPE_BUFFER_EXCEPTION_MAX) ? STRING_UNKNOWN : \
					STRING(C65_TEST_TYPE_BUFFER_EXCEPTION_STR[_TYPE_]))

			#define THROW_C65_TEST_TYPE_BUFFER_EXCEPTION(_EXCEPT_) \
				THROW_C65_TEST_TYPE_BUFFER_EXCEPTION_FORMAT(_EXCEPT_, "", "")
			#define THROW_C65_TEST_TYPE_BUFFER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
				THROW_EXCEPTION(C65_TEST_TYPE_BUFFER_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

			enum {
				FILE_EMPTY = 0,
				FILE_FULL,
			};

			#define FILE_INVALID "./file_invalid.bin"

			#define FILE_MAX FILE_FULL

			static const std::vector<c65_byte_t> FILE_DAT[] = {
				{}, // FILE_EMPTY
				{ 't', 'e', 's', 't' }, // FILE_FULL
				};

			#define FILE_DATA(_TYPE_) \
				(((_TYPE_) > FILE_MAX) ? FILE_DAT[FILE_EMPTY] : \
					FILE_DAT[_TYPE_])

			static const std::string FILE_STR[] = {
				"./file_empty.bin", // FILE_EMPTY
				"./file_full.bin", // FILE_FULL
				};

			#define FILE_STRING(_TYPE_) \
				(((_TYPE_) > FILE_MAX) ? STRING_UNKNOWN : \
					STRING(FILE_STR[_TYPE_]))
		}
	}
}

#endif // C65_TEST_TYPE_BUFFER_TYPE_H_
