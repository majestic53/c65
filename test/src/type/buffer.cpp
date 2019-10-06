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

#include "../../../include/type/buffer.h"
#include "../../include/type/buffer.h"
#include "./buffer_type.h"

namespace c65 {

	namespace test {

		namespace type {

			buffer::buffer(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			buffer::~buffer(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			void
			buffer::create_file(
				__in const std::string &path,
				__in_opt const std::vector<c65_byte_t> &data
				)
			{
				std::ofstream file;

				TRACE_ENTRY_FORMAT("Path[%u]=%s, Data[%u]=%p", path.size(), STRING(path), data.size(), &data);

				file = std::ofstream(STRING(path), std::ios::binary | std::ios::out | std::ios::trunc);
				if(!file) {
					THROW_C65_TEST_TYPE_BUFFER_EXCEPTION_FORMAT(C65_TEST_TYPE_BUFFER_EXCEPTION_FILE_INVALID,
						"%s", STRING(path));
				}

				if(!data.empty()) {
					file.write((char *)&data[0], data.size());
				}

				file.close();

				TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Created file", "%s, %.02f KB (%u bytes)", STRING(path),
					data.size() / (float)std::kilo::num, data.size());

				TRACE_EXIT();
			}

			void
			buffer::destroy_file(
				__in const std::string &path
				)
			{
				TRACE_ENTRY_FORMAT("Path[%u]=%s", path.size(), STRING(path));

				std::remove(STRING(path));

				TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Destroyed file", "%s", STRING(path));

				TRACE_EXIT();
			}

			void
			buffer::on_run(void)
			{
				TRACE_ENTRY();

				test_load_file();

				TRACE_EXIT();
			}

			void
			buffer::on_setup(void)
			{
				int type = 0;

				TRACE_ENTRY();

				for(; type <= FILE_MAX; ++type) {
					create_file(FILE_STRING(type), FILE_DATA(type));
				}

				TRACE_EXIT();
			}

			void
			buffer::on_teardown(void)
			{
				int type = 0;

				TRACE_ENTRY();

				for(; type <= FILE_MAX; ++type) {
					destroy_file(FILE_STRING(type));
				}

				TRACE_EXIT();
			}

			void
			buffer::test_load_file(void)
			{
				size_t index = 0;
				c65::type::buffer buffer;
				std::vector<c65_byte_t> data;

				TRACE_ENTRY();

				// Test #1: Empty file
				buffer.load(FILE_STRING(FILE_EMPTY));
				ASSERT(buffer.empty());
				buffer.clear();

				// Test #2: Non-empty file
				data = FILE_DATA(FILE_FULL);
				buffer.load(FILE_STRING(FILE_FULL));

				ASSERT(buffer.size() == data.size());

				for(; index < data.size(); ++index) {
					ASSERT(buffer.at(index) == data.at(index));
				}

				buffer.clear();

				// Test #3: Invalid file
				try {
					buffer.load(FILE_INVALID);
					ASSERT(false);
				} catch(...) {
					buffer.clear();
				}

				TRACE_EXIT();
			}
		}
	}
}
