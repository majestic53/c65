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

#ifndef C65_TYPE_H_
#define C65_TYPE_H_

#include "../include/type/exception.h"

namespace c65 {

	#define C65_RUNTIME_HEADER "[C65::RUNTIME]"
#ifndef NDEBUG
	#define C65_RUNTIME_EXCEPTION_HEADER C65_RUNTIME_HEADER " "
#else
	#define C65_RUNTIME_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		C65_RUNTIME_EXCEPTION_ACTION_INVALID = 0,
		C65_RUNTIME_EXCEPTION_ADDRESS_INVALID,
		C65_RUNTIME_EXCEPTION_BREAKPOINT_INVALID,
		C65_RUNTIME_EXCEPTION_EVENT_INVALID,
		C65_RUNTIME_EXCEPTION_EXTERNAL,
		C65_RUNTIME_EXCEPTION_WATCH_INVALID,
	};

	#define C65_RUNTIME_EXCEPTION_MAX C65_RUNTIME_EXCEPTION_WATCH_INVALID

	static const std::string C65_RUNTIME_EXCEPTION_STR[] = {
		C65_RUNTIME_EXCEPTION_HEADER "Invalid action", // C65_RUNTIME_EXCEPTION_ACTION_INVALID
		C65_RUNTIME_EXCEPTION_HEADER "Invalid address", // C65_RUNTIME_EXCEPTION_ADDRESS_INVALID
		C65_RUNTIME_EXCEPTION_HEADER "Invalid breakpoint", // C65_RUNTIME_EXCEPTION_BREAKPOINT_INVALID
		C65_RUNTIME_EXCEPTION_HEADER "Invalid event", // C65_RUNTIME_EXCEPTION_EVENT_INVALID
		C65_RUNTIME_EXCEPTION_HEADER "External exception", // C65_RUNTIME_EXCEPTION_EXTERNAL
		C65_RUNTIME_EXCEPTION_HEADER "Invalid watch", // C65_RUNTIME_EXCEPTION_WATCH_INVALID
		};

	#define C65_RUNTIME_EXCEPTION_STRING(_TYPE_) \
		(((_TYPE_) > C65_RUNTIME_EXCEPTION_MAX) ? STRING_UNKNOWN : \
			STRING(C65_RUNTIME_EXCEPTION_STR[_TYPE_]))

	#define THROW_C65_RUNTIME_EXCEPTION(_EXCEPT_) \
		THROW_C65_RUNTIME_EXCEPTION_FORMAT(_EXCEPT_, "", "")
	#define THROW_C65_RUNTIME_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION(C65_RUNTIME_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

	#define CYCLES_PER_FRAME 500

	#define FRAMES_PER_SECOND 60
	#define FRAME_RATE (MILLISECONDS_PER_SECOND / (float)FRAMES_PER_SECOND)

	#define KEY_BREAK SDL_SCANCODE_ESCAPE
	#define KEY_RESET SDL_SCANCODE_F1

	#define MILLISECONDS_PER_SECOND std::kilo::num

	#define SDL_FLAGS (SDL_INIT_VIDEO)
}

#endif // C65_TYPE_H_
