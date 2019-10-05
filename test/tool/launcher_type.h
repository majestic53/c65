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

#ifndef C65_TEST_TOOL_LAUNCHER_TYPE_H_
#define C65_TEST_TOOL_LAUNCHER_TYPE_H_

#include "../../include/type/exception.h"

namespace c65 {

	namespace test {

		namespace tool {

			#define C65_TEST_TOOL_LAUNCHER_HEADER "[C65::TEST::TOOL::LAUNCHER]"
#ifndef NDEBUG
			#define C65_TEST_TOOL_LAUNCHER_EXCEPTION_HEADER C65_TEST_TOOL_LAUNCHER_HEADER " "
#else
			#define C65_TEST_TOOL_LAUNCHER_EXCEPTION_HEADER
#endif // NDEBUG

			enum {
				C65_TEST_TOOL_LAUNCHER_EXCEPTION_ARGUMENT_INVALID = 0,
			};

			#define C65_TEST_TOOL_LAUNCHER_EXCEPTION_MAX C65_TEST_TOOL_LAUNCHER_EXCEPTION_ARGUMENT_INVALID

			static const std::string C65_TEST_TOOL_LAUNCHER_EXCEPTION_STR[] = {
				C65_TEST_TOOL_LAUNCHER_EXCEPTION_HEADER "Invalid argument", // C65_TEST_TOOL_LAUNCHER_EXCEPTION_ARGUMENT_INVALID
				};

			#define C65_TEST_TOOL_LAUNCHER_EXCEPTION_STRING(_TYPE_) \
				(((_TYPE_) > C65_TEST_TOOL_LAUNCHER_EXCEPTION_MAX) ? STRING_UNKNOWN : \
					STRING(C65_TEST_TOOL_LAUNCHER_EXCEPTION_STR[_TYPE_]))

			#define THROW_C65_TEST_TOOL_LAUNCHER_EXCEPTION(_EXCEPT_) \
				THROW_C65_TEST_TOOL_LAUNCHER_EXCEPTION_FORMAT(_EXCEPT_, "", "")
			#define THROW_C65_TEST_TOOL_LAUNCHER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
				THROW_EXCEPTION(C65_TEST_TOOL_LAUNCHER_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

			enum {
				ARGUMENT_HELP = 0,
				ARGUMENT_QUIET,
				ARGUMENT_VERSION,
			};

			#define ARGUMENT_DELIMITER "-"

			#define ARGUMENT_MAX ARGUMENT_VERSION
			#define ARGUMENT_MIN 1

			static const std::string ARGUMENT_DESCRIPTION_STR[] = {
				"Display help information", // ARGUMENT_HELP
				"Enable quiet mode", // ARGUMENT_QUIET
				"Display version information", // ARGUMENT_VERSION
				};

			#define ARGUMENT_DESCRIPTION_STRING(_TYPE_) \
				(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
					STRING(ARGUMENT_DESCRIPTION_STR[_TYPE_]))

			static const std::string ARGUMENT_LONG_STR[] = {
				ARGUMENT_DELIMITER ARGUMENT_DELIMITER "help", // ARGUMENT_HELP
				ARGUMENT_DELIMITER ARGUMENT_DELIMITER "quiet", // ARGUMENT_QUIET
				ARGUMENT_DELIMITER ARGUMENT_DELIMITER "version", // ARGUMENT_VERSION
				};

			#define ARGUMENT_LONG_STRING(_TYPE_) \
				(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
					STRING(ARGUMENT_LONG_STR[_TYPE_]))

			static const std::string ARGUMENT_SHORT_STR[] = {
				ARGUMENT_DELIMITER "h", // ARGUMENT_HELP
				ARGUMENT_DELIMITER "q", // ARGUMENT_QUIET
				ARGUMENT_DELIMITER "v", // ARGUMENT_VERSION
				};

			#define ARGUMENT_SHORT_STRING(_TYPE_) \
				(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
					STRING(ARGUMENT_SHORT_STR[_TYPE_]))

			static const std::map<std::string, int> ARGUMENT_MAP = {
				std::make_pair(ARGUMENT_LONG_STRING(ARGUMENT_HELP), ARGUMENT_HELP), // ARGUMENT_HELP
				std::make_pair(ARGUMENT_LONG_STRING(ARGUMENT_QUIET), ARGUMENT_QUIET), // ARGUMENT_QUIET
				std::make_pair(ARGUMENT_LONG_STRING(ARGUMENT_VERSION), ARGUMENT_VERSION), // ARGUMENT_VERSION
				std::make_pair(ARGUMENT_SHORT_STRING(ARGUMENT_HELP), ARGUMENT_HELP), // ARGUMENT_HELP
				std::make_pair(ARGUMENT_SHORT_STRING(ARGUMENT_QUIET), ARGUMENT_QUIET), // ARGUMENT_QUIET
				std::make_pair(ARGUMENT_SHORT_STRING(ARGUMENT_VERSION), ARGUMENT_VERSION), // ARGUMENT_VERSION
				};

			#define ARGUMENT_SUPPORTED(_ARGUMENT_) \
				(ARGUMENT_MAP.find(_ARGUMENT_) != ARGUMENT_MAP.end())
			#define ARGUMENT_TYPE(_ARGUMENT_) \
				ARGUMENT_MAP.find(_ARGUMENT_)->second

			#define C65_TEST_SUITE C65 " Test Suite"

			#define USAGE "c65-test [args]"
		}
	}
}

#endif // C65_TEST_TOOL_LAUNCHER_TYPE_H_
