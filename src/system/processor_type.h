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

#ifndef C65_SYSTEM_PROCESSOR_TYPE_H_
#define C65_SYSTEM_PROCESSOR_TYPE_H_

#include "../../include/type/exception.h"

namespace c65 {

	namespace system {

		#define C65_SYSTEM_PROCESSOR_HEADER "[C65::SYSTEM::PROCESSOR]"
#ifndef NDEBUG
		#define C65_SYSTEM_PROCESSOR_EXCEPTION_HEADER C65_SYSTEM_PROCESSOR_HEADER " "
#else
		#define C65_SYSTEM_PROCESSOR_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			C65_SYSTEM_PROCESSOR_EXCEPTION_ADDRESS_INVALID = 0,
			C65_SYSTEM_PROCESSOR_EXCEPTION_INTERRUPT_INVALID,
		};

		#define C65_SYSTEM_PROCESSOR_EXCEPTION_MAX C65_SYSTEM_PROCESSOR_EXCEPTION_INTERRUPT_INVALID

		static const std::string C65_SYSTEM_PROCESSOR_EXCEPTION_STR[] = {
			C65_SYSTEM_PROCESSOR_EXCEPTION_HEADER "Invalid address", // C65_SYSTEM_PROCESSOR_EXCEPTION_ADDRESS_INVALID
			C65_SYSTEM_PROCESSOR_EXCEPTION_HEADER "Invalid interrupt", // C65_SYSTEM_PROCESSOR_EXCEPTION_INTERRUPT_INVALID
			};

		#define C65_SYSTEM_PROCESSOR_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > C65_SYSTEM_PROCESSOR_EXCEPTION_MAX) ? STRING_UNKNOWN : \
				STRING(C65_SYSTEM_PROCESSOR_EXCEPTION_STR[_TYPE_]))

		#define THROW_C65_SYSTEM_PROCESSOR_EXCEPTION(_EXCEPT_) \
			THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_C65_SYSTEM_PROCESSOR_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION(C65_SYSTEM_PROCESSOR_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

		#define BIT_CLEAR(_MASK_, _BIT_) \
			((_MASK_) &= ~BIT_MASK(_BIT_))
		#define BIT_CHECK(_MASK_, _BIT_) \
			(((_MASK_) & BIT_MASK(_BIT_)) == BIT_MASK(_BIT_))
		#define BIT_MASK(_BIT_) \
			(1 << (_BIT_))
		#define BIT_SET(_MASK_, _BIT_) \
			((_MASK_) |= BIT_MASK(_BIT_))
	}
}

#endif // C65_SYSTEM_PROCESSOR_TYPE_H_
