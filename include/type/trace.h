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

#ifndef C65_TYPE_TRACE_H_
#define C65_TYPE_TRACE_H_

#include "../common.h"

namespace c65 {

	namespace type {

		class trace {

			public:

				static void enable(
					__in bool enabled
					);

				static void generate(
					__in int level,
					__in const std::string &prefix,
					__in const std::string &message,
					__in const std::string &file,
					__in const std::string &function,
					__in size_t line,
					__in const char *format,
					...
					);

			protected:

				static std::string timestamp(void);

				static bool g_enabled;
		};
	}
}

#endif // C65_TYPE_TRACE_H_
