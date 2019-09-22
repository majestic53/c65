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

#ifndef C65_COMMON_ADDRESS_H_
#define C65_COMMON_ADDRESS_H_

#define ADDRESS_INPUT 0x00ff

#define ADDRESS_MEMORY_HIGH_BEGIN 0x0600
#define ADDRESS_MEMORY_HIGH_END 0xfff9
#define ADDRESS_MEMORY_STACK_BEGIN 0x0100
#define ADDRESS_MEMORY_STACK_END 0x01ff
#define ADDRESS_MEMORY_ZERO_PAGE_BEGIN 0x0000
#define ADDRESS_MEMORY_ZERO_PAGE_END 0x00fd

#define ADDRESS_PROCESSOR_MASKABLE 0xfffe
#define ADDRESS_PROCESSOR_NONMASKABLE 0xfffa
#define ADDRESS_PROCESSOR_RESET 0xfffc

#define ADDRESS_RANDOM 0x00fe

#define ADDRESS_VIDEO_BEGIN 0x0200
#define ADDRESS_VIDEO_END 0x05ff

#define ADDRESS_LENGTH(_BEGIN_, _END_) \
	(((_END_) + 1) - (_BEGIN_))

#endif // C65_COMMON_ADDRESS_H_
