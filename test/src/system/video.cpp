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

#include "../../../include/system/video.h"
#include "../../include/system/video.h"
#include "./video_type.h"

namespace c65 {

	namespace test {

		namespace system {

			video::video(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			video::~video(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			void
			video::on_run(
				__in bool quiet
				)
			{
				TRACE_ENTRY_FORMAT("Quiet=%x", quiet);

				EXECUTE_TEST(test_clear, quiet);
				EXECUTE_TEST(test_read, quiet);
				EXECUTE_TEST(test_write, quiet);

				TRACE_EXIT();
			}

			void
			video::on_setup(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			void
			video::on_teardown(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			void
			video::test_clear(void)
			{
				c65_address_t address = {};

				TRACE_ENTRY();

				c65::system::video &instance = c65::system::video::instance();

				instance.initialize();

				for(address.word = ADDRESS_VIDEO_BEGIN; address.word <= ADDRESS_VIDEO_END; ++address.word) {
					instance.write(address, BACKGROUND_COLOR + 1);
				}

				// Test #1: Clear video RAM
				instance.clear();

				for(address.word = ADDRESS_VIDEO_BEGIN; address.word <= ADDRESS_VIDEO_END; ++address.word) {
					ASSERT(instance.read(address) == BACKGROUND_COLOR);
				}

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			video::test_read(void)
			{
				c65_address_t address = {};

				TRACE_ENTRY();

				c65::system::video &instance = c65::system::video::instance();

				instance.initialize();

				// Test #1: Valid range
				for(address.word = ADDRESS_VIDEO_BEGIN; address.word <= ADDRESS_VIDEO_END; ++address.word) {
					ASSERT(instance.read(address) == BACKGROUND_COLOR);
				}

				// Test #2: Invalid range
				try {
					instance.read(address);
					ASSERT(false);
				} catch(...) { }

				instance.uninitialize();

				TRACE_EXIT();
			}

			void
			video::test_write(void)
			{
				c65_address_t address = {};
				c65_byte_t value = (std::rand() % COLOR_MAX);

				TRACE_ENTRY();

				c65::system::video &instance = c65::system::video::instance();

				instance.initialize();

				// Test #1: Valid range
				for(address.word = ADDRESS_VIDEO_BEGIN; address.word <= ADDRESS_VIDEO_END; ++address.word) {
					instance.write(address, value);
					ASSERT(instance.read(address) == value);
				}

				// Test #2: Invalid range
				try {
					instance.write(address, value);
					ASSERT(false);
				} catch(...) { }

				instance.uninitialize();

				TRACE_EXIT();
			}
		}
	}
}
