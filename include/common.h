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

#ifndef C65_COMMON_H_
#define C65_COMMON_H_

#include <SDL2/SDL.h>
#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <ratio>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef __in
#define __in
#endif // __in
#ifndef __in_opt
#define __in_opt
#endif // __in_opt
#ifndef __inout
#define __inout
#endif // __inout
#ifndef __inout_opt
#define __inout_opt
#endif // __inout_opt
#ifndef __out
#define __out
#endif // __out
#ifndef __out_opt
#define __out_opt
#endif // __out_opt

#include "./common/action.h"
#include "./common/address.h"
#include "./common/bit.h"
#include "./common/color.h"
#include "./common/event.h"
#include "./common/exception.h"
#include "./common/flag.h"
#include "./common/instruction.h"
#include "./common/interrupt.h"
#include "./common/level.h"
#include "./common/mask.h"
#include "./common/memory.h"
#include "./common/register.h"
#include "./common/stack.h"
#include "./common/string.h"
#include "./common/trace.h"
#include "./common/version.h"

#endif // C65_COMMON_H_
