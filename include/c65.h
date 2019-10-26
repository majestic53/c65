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
	C65_ACTION_BREAKPOINT_CLEAR = 0, /* Breakpoint clear */
	C65_ACTION_BREAKPOINT_SET, /* Breakpoint set */
	C65_ACTION_CYCLE, /* Cycle count */
	C65_ACTION_INTERRUPT_PENDING, /* Interrupt pending state */
	C65_ACTION_READ_BYTE, /* Read memory byte */
	C65_ACTION_READ_REGISTER, /* Read processor register */
	C65_ACTION_READ_STATUS, /* Read processor status */
	C65_ACTION_READ_WORD, /* Read memory word */
	C65_ACTION_STACK_OVERFLOW, /* Stack overflow status */
	C65_ACTION_STACK_UNDERFLOW, /* Stack underflow status */
	C65_ACTION_STOPPED, /* Stop status */
	C65_ACTION_WAITING, /* Wait status */
	C65_ACTION_WATCH_CLEAR, /* Watch clear */
	C65_ACTION_WATCH_SET, /* Watch set */
	C65_ACTION_WINDOW_SHOW, /* Show window */
	C65_ACTION_WRITE_BYTE, /* Write memory byte */
	C65_ACTION_WRITE_REGISTER, /* Write processor register */
	C65_ACTION_WRITE_STATUS, /* Write processor status */
	C65_ACTION_WRITE_WORD, /* Write memory word */
};

#define C65_ACTION_MAX C65_ACTION_WRITE_WORD

enum {
	C65_EVENT_BREAKPOINT = 0, /* Breakpoint event */
	C65_EVENT_ILLEGAL_INSTRUCTION, /* Illegal instruction event */
	C65_EVENT_INTERRUPT_ENTRY, /* Interrupt entry event */
	C65_EVENT_INTERRUPT_EXIT, /* Interrupt exit event */
	C65_EVENT_STACK_OVERFLOW, /* Stack overflow event */
	C65_EVENT_STACK_UNDERFLOW, /* Stack underflow event */
	C65_EVENT_STOP_ENTRY, /* Stop entry event */
	C65_EVENT_STOP_EXIT, /* Stop exit event */
	C65_EVENT_SUBROUTINE_ENTRY, /* Subroutine entry event */
	C65_EVENT_SUBROUTINE_EXIT, /* Subroutine exit event */
	C65_EVENT_WAIT_ENTRY, /* Wait entry event */
	C65_EVENT_WAIT_EXIT, /* Wait exit event */
	C65_EVENT_WATCH, /* Memory watch event */
};

#define C65_EVENT_MAX C65_EVENT_WATCH

enum {
	C65_INTERRUPT_NON_MASKABLE = 0, /* Non-maskable interrupt (NMI) */
	C65_INTERRUPT_MASKABLE, /* Maskable interrupt (IRQ) */
};

#define C65_INTERRUPT_MAX C65_INTERRUPT_MASKABLE

enum {
	C65_REGISTER_ACCUMULATOR = 0, /* Accumulator register (A) */
	C65_REGISTER_INDEX_X, /* Index-x register (X) */
	C65_REGISTER_INDEX_Y, /* Index-y register (Y) */
	C65_REGISTER_PROGRAM_COUNTER, /* Program counter register (PC) */
	C65_REGISTER_STACK_POINTER, /* Stack pointer register (SP) */
};

#define C65_REGISTER_MAX C65_REGISTER_STACK_POINTER

typedef uint8_t c65_byte_t; /* Byte type */
typedef uint16_t c65_word_t; /* Word type */
typedef uint32_t c65_dword_t; /* Double-word type */

 /* Address structure */
typedef union {

	struct {
		c65_byte_t low; /* Lower byte */
		c65_byte_t high; /* Upper byte */
	};

	c65_word_t word; /* Raw word */
} __attribute__((packed)) c65_address_t;

 /* Status structure */
typedef union {

	struct {
		c65_byte_t carry : 1; /* Carry flag (C) */
		c65_byte_t zero : 1; /* Zero flag (Z) */
		c65_byte_t interrupt_disable : 1; /* Interrupt disable flag (I) */
		c65_byte_t decimal_mode : 1; /* Decimal mode flag (D) */
		c65_byte_t break_instruction : 1; /* Break instruction flag (B) */
		c65_byte_t unused : 1; /* Unused flag (-) */
		c65_byte_t overflow : 1; /* Overflow flag (V) */
		c65_byte_t negative : 1; /* Negative flag (N) */
	};

	c65_byte_t raw; /* Raw byte */
} __attribute__((packed)) c65_status_t;

typedef c65_address_t c65_register_t; /* Register structure */

 /* Action structure */
typedef struct {
	int type; /* Action type */
	c65_address_t address; /* Action address */

	union {
		c65_dword_t cycle; /* Action cycle */
		c65_register_t data; /* Action data */
		c65_status_t status; /* Action status */
	};
} __attribute__((packed)) c65_action_t;

typedef c65_action_t c65_event_t; /* Event structure */

/**
 * Event handler
 * @param event Event structure
 */
typedef void (*c65_event_hdlr)(const c65_event_t *event);

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
 * Register emulator event handler
 * @param type Event type
 * @param handler Event handler
 * @return EXIT_SUCCESS on success
 */
int c65_event_handler(int type, c65_event_hdlr handler);

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
int c65_load(const c65_byte_t *data, c65_dword_t length, c65_address_t base);

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
int c65_unload(c65_address_t base, c65_dword_t length);

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
