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

#ifndef C65_COMMON_COLOR_H_
#define C65_COMMON_COLOR_H_

enum {
	COLOR_BLACK = 0,
	COLOR_WHITE,
	COLOR_RED,
	COLOR_CYAN,
	COLOR_PURPLE,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_YELLOW,
	COLOR_ORANGE,
	COLOR_BROWN,
	COLOR_LIGHT_RED,
	COLOR_DARK_GREY,
	COLOR_GREY,
	COLOR_LIGHT_GREEN,
	COLOR_LIGHT_BLUE,
	COLOR_LIGHT_GREY,
};

#define COLOR_MAX COLOR_LIGHT_GREY

typedef union {

	struct {
		uint8_t blue;
		uint8_t green;
		uint8_t red;
		uint8_t alpha;
	};

	uint32_t raw;
} __attribute__((packed)) color_t;

static const color_t COL[] = {
	{{ 0, 0, 0, 255 }}, // COLOR_BLACK
	{{ 255, 255, 255, 255 }}, // COLOR_WHITE
	{{ 0, 0, 255, 255 }}, // COLOR_RED
	{{ 255, 255, 0, 255 }}, // COLOR_CYAN
	{{ 255, 0, 255, 255 }}, // COLOR_PURPLE
	{{ 0, 255, 0, 255 }}, // COLOR_GREEN
	{{ 255, 0, 0, 255 }}, // COLOR_BLUE
	{{ 0, 255, 255, 255 }}, // COLOR_YELLOW
	{{ 0, 127, 255, 255 }}, // COLOR_ORANGE
	{{ 0, 60, 90, 255 }}, // COLOR_BROWN
	{{ 20, 60, 255, 255 }}, // COLOR_LIGHT_RED
	{{ 40, 40, 40, 255 }}, // COLOR_DARK_GREY
	{{ 100, 100, 100, 255 }}, // COLOR_GREY
	{{ 0, 255, 127, 255 }}, // COLOR_LIGHT_GREEN
	{{ 255, 127, 0, 255 }}, // COLOR_LIGHT_BLUE
	{{ 160, 160, 160, 255 }}, // COLOR_LIGHT_GREY
	};

#define COLOR(_TYPE_) \
	(((_TYPE_) > COLOR_MAX) ? COL[COLOR_BLACK] : \
		COL[_TYPE_])

#endif // C65_COMMON_COLOR_H_
