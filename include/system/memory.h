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

#ifndef C65_SYSTEM_MEMORY_H_
#define C65_SYSTEM_MEMORY_H_

#include "../interface/bus.h"
#include "../interface/singleton.h"
#include "../type/buffer.h"

namespace c65 {

	namespace system {

		class memory :
				public c65::interface::bus,
				public c65::interface::singleton<c65::system::memory> {

			public:

				~memory(void);

			protected:

				friend class c65::interface::singleton<c65::system::memory>;

				memory(void);

				memory(
					__in const memory &other
					) = delete;

				memory &operator=(
					__in const memory &other
					) = delete;

				void on_initialize(void) override;

				void on_notify(
					__in const c65_event_t &event
					) const override;

				c65_byte_t on_read(
					__in c65_address_t address
					) const override;

				void on_uninitialize(void) override;

				void on_write(
					__in c65_address_t address,
					__in c65_byte_t value
					) override;

				c65::type::buffer m_high;

				c65::type::buffer m_stack;

				c65::type::buffer m_zero_page;
		};
	}
}

#endif // C65_SYSTEM_MEMORY_H_
