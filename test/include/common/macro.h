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

#ifndef C65_TEST_COMMON_MACRO_H_
#define C65_TEST_COMMON_MACRO_H_

#define ASSERT(_CONDITION_) { \
	if(!(_CONDITION_)) { \
		THROW_EXCEPTION("Failed assertion", "%s", AS_STRING(_CONDITION_)); \
	} \
	}

#define EXECUTE_TEST(_TEST_, _QUIET_) { \
	if(((TRACE) == LEVEL_VERBOSE) && !(_QUIET_)) { \
		std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << AS_STRING(_TEST_) << LEVEL_COLOR(LEVEL_NONE) << std::endl; \
	} \
	TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE, "Executing test", "%s", AS_STRING(_TEST_)); \
	(_TEST_)(); \
	}

#endif // C65_TEST_COMMON_MACRO_H_
