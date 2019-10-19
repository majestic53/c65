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

				void notify(
					__in int type,
					__in c65_address_t address
					) const
				{
					c65_event_t event = {};

					TRACE_ENTRY_FORMAT("Type=%i(%s), Address=%u(%04x)", type, EVENT_STRING(type), address.word, address.word);

					event.type = type;
					event.address = address;
					on_notify(event);

					TRACE_EXIT();
				}

				void notify(
					__in int type,
					__in c65_address_t address,
					__in c65_word_t value
					) const
				{
					c65_event_t event = {};

					TRACE_ENTRY_FORMAT("Type=%i(%s), Address=%u(%04x), Value=%u(%04x)", type, EVENT_STRING(type),
						address.word, address.word, value, value);

					event.type = type;
					event.address = address;
					event.data.word = value;
					on_notify(event);

					TRACE_EXIT();
				}

				c65_byte_t read(
					__in c65_address_t address
					) const
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

				virtual void on_notify(
					__in const c65_event_t &event
					) const = 0;

				virtual c65_byte_t on_read(
					__in c65_address_t address
					) const = 0;

				virtual void on_write(
					__in c65_address_t address,
					__in c65_byte_t value
					) = 0;
		};
	}
}

#endif // C65_INTERFACE_BUS_H_
