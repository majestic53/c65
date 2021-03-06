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

#ifndef C65_TYPE_TRACE_TYPE_H_
#define C65_TYPE_TRACE_TYPE_H_

#include "../../include/common.h"

namespace c65 {

	namespace type {

		#define C65_TYPE_TRACE_HEADER "[C65::TYPE::TRACE]"
#ifndef NDEBUG
		#define C65_TYPE_TRACE_EXCEPTION_HEADER C65_TYPE_TRACE_HEADER " "
#else
		#define C65_TYPE_TRACE_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			C65_TYPE_TRACE_EXCEPTION_FORMAT_INVALID = 0,
			C65_TYPE_TRACE_EXCEPTION_TIMESTAMP_INVALID,
		};

		#define C65_TYPE_TRACE_EXCEPTION_MAX C65_TYPE_TRACE_EXCEPTION_TIMESTAMP_INVALID

		static const std::string C65_TYPE_TRACE_EXCEPTION_STR[] = {
			C65_TYPE_TRACE_EXCEPTION_HEADER "Invalid format", // C65_TYPE_TRACE_EXCEPTION_FORMAT_INVALID
			C65_TYPE_TRACE_EXCEPTION_HEADER "Invalid timestamp", // C65_TYPE_TRACE_EXCEPTION_TIMESTAMP_INVALID
			};

		#define C65_TYPE_TRACE_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > C65_TYPE_TRACE_EXCEPTION_MAX) ? STRING_UNKNOWN : \
				STRING(C65_TYPE_TRACE_EXCEPTION_STR[_TYPE_]))

		#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"
		#define TIMESTAMP_LENGTH 32
	}
}

#endif // C65_TYPE_TRACE_TYPE_H_
