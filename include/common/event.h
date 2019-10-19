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

#ifndef C65_EVENT_H_
#define C65_EVENT_H_

static const std::string EVENT_STR[] = {
	"Breakpoint", // C65_EVENT_BREAKPOINT
	"Illegal-Instruction", // C65_EVENT_ILLEGAL_INSTRUCTION
	"Interrupt-Entry", // C65_EVENT_INTERRUPT_ENTRY
	"Interrupt-Exit", // C65_EVENT_INTERRUPT_EXIT
	"Stack-Overflow", // C65_EVENT_STACK_OVERFLOW
	"Stack-Underflow", // C65_EVENT_STACK_UNDERFLOW
	"Stop-Entry", // C65_EVENT_STOP_ENTRY
	"Stop-Exit", // C65_EVENT_STOP_EXIT
	"Subroutine-Entry", // C65_EVENT_SUBROUTINE_ENTRY
	"Subroutine-Exit", // C65_EVENT_SUBROUTINE_EXIT
	"Wait-Entry", // C65_EVENT_WAIT_ENTRY
	"Wait-Exit", // C65_EVENT_WAIT_EXIT
	"Watch", // C65_EVENT_WATCH
	};

#define EVENT_STRING(_TYPE_) \
	(((_TYPE_) > C65_EVENT_MAX) ? STRING_UNKNOWN : \
		STRING(EVENT_STR[_TYPE_]))

#endif // C65_EVENT_H_
