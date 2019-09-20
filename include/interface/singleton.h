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

#ifndef C65_INTERFACE_SINGLETON_H_
#define C65_INTERFACE_SINGLETON_H_

#include "../type/trace.h"

namespace c65 {

	namespace interface {

		template <class T> class singleton {

			public:

				~singleton(void)
				{
					TRACE_ENTRY();
					TRACE_EXIT();
				}

				void initialize(void)
				{
					TRACE_ENTRY();

					if(!m_initialized) {
						on_initialize();
						m_initialized = true;
					}

					TRACE_EXIT();
				}

				static T &instance(void)
				{
					TRACE_ENTRY();

					static T result;

					TRACE_EXIT_FORMAT("Result=%p", &result);
					return result;
				}

				void uninitialize(void)
				{
					TRACE_ENTRY();

					if(m_initialized) {
						m_initialized = false;
						on_uninitialize();
					}

					TRACE_EXIT();
				}

			protected:

				singleton(void) :
					m_initialized(false)
				{
					TRACE_ENTRY();
					TRACE_EXIT();
				}

				singleton(
					__in const singleton &other
					) = delete;

				singleton &operator=(
					__in const singleton &other
					) = delete;

				virtual void on_initialize(void) = 0;

				virtual void on_uninitialize(void) = 0;

				bool m_initialized;
		};
	}
}

#endif // C65_INTERFACE_SINGLETON_H_
