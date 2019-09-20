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

#ifndef C65_INTERFACE_BUS_H_
#define C65_INTERFACE_BUS_H_

#include "../type/trace.h"
#include "../c65.h"

namespace c65 {

	namespace interface {

		class bus {

			public:

				c65_byte_t read(
					__in c65_address_t address
					)
				{
					c65_byte_t result;

					TRACE_ENTRY_FORMAT("Address=%u(%04x)", address.word, address.word);

					result = on_read(address);

					TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
					return result;
				}

				void write(
					__in c65_address_t address,
					__in c65_byte_t value
					)
				{
					TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address.word, address.word, value, value);

					on_write(address, value);

					TRACE_EXIT();
				}

			protected:

				virtual c65_byte_t on_read(
					__in c65_address_t address
					) = 0;

				virtual void on_write(
					__in c65_address_t address,
					__in c65_byte_t value
					) = 0;
		};
	}
}

#endif // C65_INTERFACE_BUS_H_
