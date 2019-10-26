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

#ifndef C65_SYSTEM_VIDEO_TYPE_H_
#define C65_SYSTEM_VIDEO_TYPE_H_

#include "../../include/type/exception.h"

namespace c65 {

	namespace system {

		#define C65_SYSTEM_VIDEO_HEADER "[C65::SYSTEM::VIDEO]"
#ifndef NDEBUG
		#define C65_SYSTEM_VIDEO_EXCEPTION_HEADER C65_SYSTEM_VIDEO_HEADER " "
#else
		#define C65_SYSTEM_VIDEO_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			C65_SYSTEM_VIDEO_EXCEPTION_ADDRESS_INVALID = 0,
			C65_SYSTEM_VIDEO_EXCEPTION_EXTERNAL,
			C65_SYSTEM_VIDEO_EXCEPTION_PIXEL_INVALID,
		};

		#define C65_SYSTEM_VIDEO_EXCEPTION_MAX C65_SYSTEM_VIDEO_EXCEPTION_PIXEL_INVALID

		static const std::string C65_SYSTEM_VIDEO_EXCEPTION_STR[] = {
			C65_SYSTEM_VIDEO_EXCEPTION_HEADER "Invalid address", // C65_SYSTEM_VIDEO_EXCEPTION_ADDRESS_INVALID
			C65_SYSTEM_VIDEO_EXCEPTION_HEADER "External exception", // C65_SYSTEM_VIDEO_EXCEPTION_EXTERNAL
			C65_SYSTEM_VIDEO_EXCEPTION_HEADER "Invalid pixel", // C65_SYSTEM_VIDEO_EXCEPTION_PIXEL_INVALID
			};

		#define C65_SYSTEM_VIDEO_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > C65_SYSTEM_VIDEO_EXCEPTION_MAX) ? STRING_UNKNOWN : \
				STRING(C65_SYSTEM_VIDEO_EXCEPTION_STR[_TYPE_]))

		#define THROW_C65_SYSTEM_VIDEO_EXCEPTION(_EXCEPT_) \
			THROW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THORW_C65_SYSTEM_VIDEO_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION(C65_SYSTEM_VIDEO_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

		#define BACKGROUND_COLOR COLOR_BLACK

		#define DISPLAY_SHOWN true

		#define SDL_RENDERER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
		#define SDL_RENDERER_QUALITY "0"

		#define SDL_WINDOW_FLAGS (SDL_WINDOW_RESIZABLE)

		#define WINDOW_SCALE 12
#ifndef NDEBUG
		#define WINDOW_TITLE C65 " " VERSION_STRING()
#else
		#define WINDOW_TITLE C65
#endif // NDEBUG
		#define WINDOW_WIDTH 32
	}
}

#endif // C65_SYSTEM_VIDEO_TYPE_H_
