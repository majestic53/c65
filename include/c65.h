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

#ifndef C65_H_
#define C65_H_

#include <stdint.h>

#define C65_API_VERSION_1 1
#define C65_API_VERSION C65_API_VERSION_1

/**********************************************
 * TYPES
 **********************************************/

enum {
	C65_INTERRUPT_MASKABLE = 0,
	C65_INTERRUPT_NON_MASKABLE,
};

#define C65_INTERRUPT_MAX C65_INTERRUPT_NON_MASKABLE

typedef uint8_t c65_byte_t;
typedef uint16_t c65_word_t;

typedef union {

	struct {
		c65_byte_t low;
		c65_byte_t high;
	};

	c65_word_t word;
} __attribute__((packed)) c65_address_t;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**********************************************
 * RUNTIME
 **********************************************/

void c65_cleanup(void);

int c65_interrupt(int type);

int c65_load(const c65_byte_t *data, c65_address_t length, c65_address_t base);

int c65_reset(void);

int c65_run(void);

int c65_step(void);

int c65_unload(c65_address_t base, c65_address_t offset);

/**********************************************
 * MISC.
 **********************************************/

const char *c65_error(void);

const char *c65_version(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // C65_H_
