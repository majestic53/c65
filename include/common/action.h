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

#ifndef C65_COMMON_ACTION_H_
#define C65_COMMON_ACTION_H_

static const std::string ACTION_STR[] = {
	"Breakpoint-Clear", // C65_ACTION_BREAKPOINT_CLEAR
	"Breakpoint-Set", // C65_ACTION_BREAKPOINT_SET
	"Cycle", // C65_ACTION_CYCLE
	"Interrupt-Pending", // C65_ACTION_INTERRUPT_PENDING
	"Read-Byte", // C65_ACTION_READ_BYTE
	"Read-Register", // C65_ACTION_READ_REGISTER
	"Read-Status", // C65_ACTION_READ_STATUS
	"Read-Word", // C65_ACTION_READ_WORD
	"Stack-Overflow", // C65_ACTION_STACK_OVERFLOW
	"Stack-Underflow", // C65_ACTION_STACK_UNDERFLOW
	"Stopped", // C65_ACTION_STOPPED
	"Waiting", // C65_ACTION_WAITING
	"Watch-Clear", // C65_ACTION_WATCH_CLEAR
	"Watch-Set", // C65_ACTION_WATCH_SET
	"Write-Byte", // C65_ACTION_WRITE_BYTE
	"Write-Register", // C65_ACTION_WRITE_REGISTER
	"Write-Status", // C65_ACTION_WRITE_STATUS
	"Write-Word", // C65_ACTION_WRITE_WORD
	};

#define ACTION_STRING(_TYPE_) \
	(((_TYPE_) > C65_ACTION_MAX) ? STRING_UNKNOWN : \
		STRING(ACTION_STR[_TYPE_]))

#endif // C65_COMMON_ACTION_H_
