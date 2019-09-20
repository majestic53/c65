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

#ifndef C65_COMMON_INTERRUPT_H_
#define C65_COMMON_INTERRUPT_H_

static const std::string INTERRUPT_STR[] = {
	"IRQ", // C65_INTERRUPT_MASKABLE
	"NMI", // C65_INTERRUPT_NON_MASKABLE
	};

#define INTERRUPT_STRING(_TYPE_) \
	(((_TYPE_) > C65_INTERRUPT_MAX) ? STRING_UNKNOWN : \
		STRING(INTERRUPT_STR[_TYPE_]))

#endif // C65_COMMON_INTERRUPT_H_
