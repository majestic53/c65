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

#ifndef C65_SYSTEM_PROCESSOR_H_
#define C65_SYSTEM_PROCESSOR_H_

#include "../interface/bus.h"
#include "../interface/singleton.h"

namespace c65 {

	namespace system {

		class processor :
				public c65::interface::bus,
				public c65::interface::singleton<c65::system::processor> {

			public:

				~processor(void);

				void interrupt(
					__in int type
					);

				void reset(
					__in c65::interface::bus &bus
					);

				uint8_t step(
					__in c65::interface::bus &bus
					);

			protected:

				friend class c65::interface::singleton<c65::system::processor>;

				processor(void);

				processor(
					__in const processor &other
					) = delete;

				processor &operator=(
					__in const processor &other
					) = delete;

				uint8_t execute(
					__in c65::interface::bus &bus
					);

				uint8_t execute_nop(void);

				void on_initialize(void) override;

				c65_byte_t on_read(
					__in c65_address_t address
					) const override;

				void on_uninitialize(void) override;

				void on_write(
					__in c65_address_t address,
					__in c65_byte_t value
					) override;

				c65_byte_t pop_byte(
					__in const c65::interface::bus &bus
					);

				c65_word_t pop_word(
					__in const c65::interface::bus &bus
					);

				void push_byte(
					__in c65::interface::bus &bus,
					__in c65_byte_t value
					);

				void push_word(
					__in c65::interface::bus &bus,
					__in c65_word_t value
					);

				c65_byte_t read_byte(
					__in const c65::interface::bus &bus,
					__in c65_address_t address
					) const;

				c65_word_t read_word(
					__in const c65::interface::bus &bus,
					__in c65_address_t address
					) const;

				uint8_t service(
					__in c65::interface::bus &bus
					);

				void write_byte(
					__in c65::interface::bus &bus,
					__in c65_address_t address,
					__in c65_byte_t value
					);

				void write_word(
					__in c65::interface::bus &bus,
					__in c65_address_t address,
					__in c65_word_t value
					);

				c65_register_t m_accumulator;

				c65_register_t m_index_x;

				c65_register_t m_index_y;

				int m_interrupt;

				c65_address_t m_maskable;

				c65_address_t m_non_maskable;

				c65_register_t m_program_counter;

				c65_address_t m_reset;

				c65_register_t m_stack;

				c65_flag_t m_status;

				bool m_stop;

				bool m_wait;
		};
	}
}

#endif // C65_SYSTEM_PROCESSOR_H_
