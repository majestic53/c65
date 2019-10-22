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

#ifndef C65_TEST_COMMON_TEST_H_
#define C65_TEST_COMMON_TEST_H_

enum {
	TEST_SUCCESS = 0,
	TEST_FAILURE,
	TEST_INCONCLUSIVE,
};

#define TEST_MAX TEST_INCONCLUSIVE

static const std::string TEST_STR[] = {
	"Success", // TEST_PASS
	"Failure", // TEST_FAILURE
	"Inconclusive", // TEST_INCONCLUSIVE
	};

#define TEST_STRING(_TYPE_) \
	(((_TYPE_) > TEST_MAX) ? STRING_UNKNOWN : \
		STRING(TEST_STR[_TYPE_]))

typedef struct {
	c65_register_t accumulator;
	c65_register_t index_x;
	c65_register_t index_y;
	c65_register_t program_counter;
	c65_register_t stack_pointer;
	c65_status_t status;
} processor_state_t;

typedef void (*test_t)(void);

#endif // C65_TEST_COMMON_TEST_H_
