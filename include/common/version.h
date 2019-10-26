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

#ifndef C65_COMMON_VERSION_H_
#define C65_COMMON_VERSION_H_

#define C65 "C65"
#define C65_NOTICE "Copyright (C) 2019 David Jolly"

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_RELEASE "alpha"
#define VERSION_REVISION 5
#define VERSION_WEEK 1943

#define VERSION_STRING() \
	AS_STRING(VERSION_MAJOR) "." AS_STRING(VERSION_MINOR) "." AS_STRING(VERSION_WEEK) "." \
		AS_STRING(VERSION_REVISION) "-" VERSION_RELEASE

#endif // C65_COMMON_VERSION_H_
