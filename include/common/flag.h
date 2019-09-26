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

#ifndef C65_COMMON_FLAG_H_
#define C65_COMMON_FLAG_H_

enum {
	FLAG_CARRY = 0,
	FLAG_ZERO,
	FLAG_INTERRUPT_DISABLE,
	FLAG_DECIMAL_MODE,
	FLAG_BREAK_INSTRUCTION,
	FLAG_UNUSED,
	FLAG_OVERFLOW,
	FLAG_NEGATIVE,
};

#define FLAG_MAX FLAG_NEGATIVE

static const std::string FLAG_STR[] = {
	"C", // FLAG_CARRY
	"Z", // FLAG_ZERO
	"I", // FLAG_INTERRUPT_DISABLE
	"D", // FLAG_DECIMAL_MODE
	"B", // FLAG_BREAK_INSTRUCTION
	"-", // FLAG_UNUSED
	"V", // FLAG_OVERFLOW
	"N", // FLAG_NEGATIVE
	};

#define FLAG_STRING(_TYPE_) \
	(((_TYPE_) > FLAG_MAX) ? STRING_UNKNOWN : \
		STRING(FLAG_STR[_TYPE_]))

#endif // C65_COMMON_FLAG_H_
