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

#ifndef C65_COMMON_ACTION_H_
#define C65_COMMON_ACTION_H_

static const std::string ACTION_STR[] = {
	"Nop", // C65_ACTION_NOP
	"Read-Byte", // C65_ACTION_READ_BYTE
	"Read-Register", // C65_ACTION_READ_REGISTER
	"Read-Status", // C65_ACTION_READ_STATUS
	"Read-Word", // C65_ACTION_READ_WORD
	"Run", // C65_ACTION_RUN
	"Step", // C65_ACTION_STEP
	"Write-Byte", // C65_ACTION_WRITE_BYTE
	"Write-Register", // C65_ACTION_WRITE_REGISTER
	"Write-Status", // C65_ACTION_WRITE_STATUS
	"Write-Word", // C65_ACTION_WRITE_WORD
	};

#define ACTION_STRING(_TYPE_) \
	(((_TYPE_) > C65_ACTION_MAX) ? STRING_UNKNOWN : \
		STRING(ACTION_STR[_TYPE_]))

#endif // C65_COMMON_ACTION_H_
