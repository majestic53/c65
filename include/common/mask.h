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

#ifndef C65_COMMON_MASK_H_
#define C65_COMMON_MASK_H_

#define MASK(_BIT_) \
	(1 << (_BIT_))
#define MASK_CLEAR(_MASK_, _BIT_) \
	((_MASK_) &= ~MASK(_BIT_))
#define MASK_CHECK(_MASK_, _BIT_) \
	(((_MASK_) & MASK(_BIT_)) == MASK(_BIT_))
#define MASK_SET(_MASK_, _BIT_) \
	((_MASK_) |= MASK(_BIT_))
#define MASK_SET_CONDITIONAL(_CONDITION_, _MASK_, _BIT_) \
	((_CONDITION_) ? MASK_SET(_MASK_, _BIT_) : MASK_CLEAR(_MASK_, _BIT_))

#endif // C65_COMMON_MASK_H_
