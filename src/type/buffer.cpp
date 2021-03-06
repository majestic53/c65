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

#include "../../include/type/buffer.h"
#include "../../include/type/trace.h"
#include "./buffer_type.h"

namespace c65 {

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
		buffer::load(
			__in const std::string &path
			)
		{
			int length;
			std::ifstream file;

			TRACE_ENTRY_FORMAT("Path[%u]=%s", path.size(), STRING(path));

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "File loading", "%s", STRING(path));

			file.open(STRING(path), std::ios::binary | std::ios::in);
			if(!file) {
				THROW_C65_TYPE_BUFFER_EXCEPTION_FORMAT(C65_TYPE_BUFFER_EXCEPTION_FILE_NOT_FOUND, "%s", STRING(path));
			}

			file.seekg(0, std::ios::end);
			length = file.tellg();
			file.seekg(0, std::ios::beg);

			if(length > 0) {
				std::vector<c65_byte_t>::resize(length);
				file.read((char *)&((*this)[0]), std::vector<c65_byte_t>::size());
			} else if(!length) {
				std::vector<c65_byte_t>::clear();
			}

			file.close();

			if(length < 0) {
				THROW_C65_TYPE_BUFFER_EXCEPTION_FORMAT(C65_TYPE_BUFFER_EXCEPTION_FILE_MALFORMED, "%s", STRING(path));
			}

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "File loaded", "%.02f KB (%u bytes)", length / (float)std::kilo::num, length);

			TRACE_EXIT();
		}
	}
}
