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

#ifndef C65_COMMON_STACK_H_
#define C65_COMMON_STACK_H_

enum {
	STACK_NONE = 0,
	STACK_OVERFLOW,
	STACK_UNDERFLOW,
};

#define STACK_MAX STACK_UNDERFLOW

static const std::string STACK_STR[] = {
	"-", // STACK_NONE
	"O", // STACK_OVERFLOW
	"U", // STACK_UNDERFLOW
	};

#define STACK_STRING(_TYPE_) \
	(((_TYPE_) > STACK_MAX) ? STRING_UNKNOWN : \
		STRING(STACK_STR[_TYPE_]))

#endif // C65_COMMON_STACK_H_
