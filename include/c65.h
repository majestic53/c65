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

	// Read memory byte
	C65_ACTION_READ_BYTE = 0,

	// Read processor register
	C65_ACTION_READ_REGISTER,

	// Read processor status
	C65_ACTION_READ_STATUS,

	// Read memory word
	C65_ACTION_READ_WORD,

	// Write memory byte
	C65_ACTION_WRITE_BYTE,

	// Write processor register
	C65_ACTION_WRITE_REGISTER,

	// Write processor status
	C65_ACTION_WRITE_STATUS,

	// Write memory word
	C65_ACTION_WRITE_WORD,
};

#define C65_ACTION_MAX C65_ACTION_WRITE_WORD

enum {

	// Non-maskable interrupt (NMI)
	C65_INTERRUPT_NON_MASKABLE = 0,

	// Maskable interrupt (IRQ)
	C65_INTERRUPT_MASKABLE,
};

#define C65_INTERRUPT_MAX C65_INTERRUPT_MASKABLE

enum {

	// Accumulator register (A)
	C65_REGISTER_ACCUMULATOR = 0,

	// Index-x register (X)
	C65_REGISTER_INDEX_X,

	// Index-y register (Y)
	C65_REGISTER_INDEX_Y,

	// Program counter register (PC)
	C65_REGISTER_PROGRAM_COUNTER,

	// Stack pointer register (SP)
	C65_REGISTER_STACK_POINTER,
};

#define C65_REGISTER_MAX C65_REGISTER_STACK_POINTER

// Byte type
typedef uint8_t c65_byte_t;

// Word type
typedef uint16_t c65_word_t;

typedef union {

	struct {

		// Low byte
		c65_byte_t low;

		// High byte
		c65_byte_t high;
	};

	// Word
	c65_word_t word;
} __attribute__((packed)) c65_address_t;

typedef union {

	struct {
		c65_byte_t carry : 1;
		c65_byte_t zero : 1;
		c65_byte_t interrupt_disable : 1;
		c65_byte_t decimal_mode : 1;
		c65_byte_t break_instruction : 1;
		c65_byte_t unused : 1;
		c65_byte_t overflow : 1;
		c65_byte_t negative : 1;
	};

	c65_byte_t raw;
} __attribute__((packed)) c65_status_t;

typedef c65_address_t c65_register_t;

typedef struct {

	// Action type
	int type;

	// Action address
	c65_address_t address;

	union {

		// Action data
		c65_register_t data;

		// Action status
		c65_status_t status;
	};
} __attribute__((packed)) c65_action_t;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**********************************************
 * RUNTIME
 **********************************************/

/**
 * Perform emulator action
 * @param request Action requested
 * @param response Action response
 * @return EXIT_SUCCESS on success
 */
int c65_action(const c65_action_t *request, c65_action_t *response);

/**
 * Cleanup emulator
 */
void c65_cleanup(void);

/**
 * Interrupt emulator
 * @param type Interrupt type
 * @return EXIT_SUCCESS on success
 */
int c65_interrupt(int type);

/**
 * Load emulator memory
 * @param data Data pointer
 * @param length Data length
 * @param base Base address
 * @return EXIT_SUCCESS on success
 */
int c65_load(const c65_byte_t *data, c65_word_t length, c65_address_t base);

/**
 * Reset emulator
 * @return EXIT_SUCCESS on success
 */
int c65_reset(void);

/**
 * Run emulator
 * @return EXIT_SUCCESS on success
 */
int c65_run(void);

/**
 * Step emulator
 * @return EXIT_SUCCESS on success
 */
int c65_step(void);

/**
 * Unload emulator memory
 * @param base Base address
 * @param length Byte length
 * @return EXIT_SUCCESS on success
 */
int c65_unload(c65_address_t base, c65_word_t length);

/**********************************************
 * MISC.
 **********************************************/

/**
 * Retrieve emulator error
 * @return Emulator error as string
 */
const char *c65_error(void);

/**
 * Retrieve emulator version
 * @return Emulator version as string
 */
const char *c65_version(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // C65_H_
