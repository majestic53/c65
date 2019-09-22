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

#include "../include/system/memory.h"
#include "./c65_type.h"

namespace c65 {

	class runtime :
			public c65::interface::bus,
			public c65::interface::singleton<c65::runtime> {

		public:

			~runtime(void)
			{
				TRACE_ENTRY();

				TRACE_MESSAGE(LEVEL_INFORMATION, C65 " unloaded");

				TRACE_EXIT();
			}

			int action(
				__in const c65_action_t *request,
				__in c65_action_t *response
				)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY_FORMAT("Request=%p, Response=%p", request, response);

				try {

					if(!request || !response) {
						THROW_C65_RUNTIME_EXCEPTION_FORMAT(C65_RUNTIME_EXCEPTION_ACTION_INVALID,
							"%p, %p", request, response);
					}

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Runtime action request", "%i(%s)",
						request->type, INTERRUPT_STRING(request->type));

					initialize();
					std::memset(response, 0, sizeof(*response));

					switch(request->type) {
						case C65_ACTION_NOP:
							break;

						// TODO: ADD ADDITIONAL ACTIONS

						default:
							THROW_C65_RUNTIME_EXCEPTION_FORMAT(C65_RUNTIME_EXCEPTION_ACTION_INVALID,
								"%i(%s)", request->type, INTERRUPT_STRING(request->type));
					}

					response->type = request->type;
				} catch(c65::type::exception &exc) {
					m_error = exc.to_string();
					result = EXIT_FAILURE;
				} catch(std::exception &exc) {
					m_error = exc.what();
					result = EXIT_FAILURE;
				}

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			const char *error(void)
			{
				const char *result;

				TRACE_ENTRY();

				result = STRING(m_error);

				TRACE_EXIT_FORMAT("Result=%p", result);
				return result;
			}

			int interrupt(
				__in int type
				)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY_FORMAT("Type=%i(%s)", type, INTERRUPT_STRING(type));

				try {
					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Runtime interrupt request", "%i(%s)",
						type, INTERRUPT_STRING(type));

					initialize();

					// TODO: INTERRUPT PROCESSOR

				} catch(c65::type::exception &exc) {
					m_error = exc.to_string();
					result = EXIT_FAILURE;
				} catch(std::exception &exc) {
					m_error = exc.what();
					result = EXIT_FAILURE;
				}

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			int load(
				__in const c65_byte_t *data,
				__in c65_address_t length,
				__in c65_address_t base
				)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY_FORMAT("Data[%u(%04x)]=%p, Base=%u(%04x)", length.word, length.word, data, base.word, base.word);

				try {
					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Runtime load request", "[%u(%04x)]%p, %u(%04x)",
						length.word, length.word, data, base.word, base.word);

					initialize();

					// TODO: LOAD MEMORY OF LENGTH AT BASE

				} catch(c65::type::exception &exc) {
					m_error = exc.to_string();
					result = EXIT_FAILURE;
				} catch(std::exception &exc) {
					m_error = exc.what();
					result = EXIT_FAILURE;
				}

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			int reset(void)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY();

				try {
					TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime reset request");

					initialize();

					// TODO: RESET SUBSYSTEMS

				} catch(c65::type::exception &exc) {
					m_error = exc.to_string();
					result = EXIT_FAILURE;
				} catch(std::exception &exc) {
					m_error = exc.what();
					result = EXIT_FAILURE;
				}

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			int run(void)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY();

				try {
					TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime run request");

					initialize();

					// TODO: RUN SUBSYSTEMS

				} catch(c65::type::exception &exc) {
					m_error = exc.to_string();
					result = EXIT_FAILURE;
				} catch(std::exception &exc) {
					m_error = exc.what();
					result = EXIT_FAILURE;
				}

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			int step(void)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY();

				try {
					TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime step request");

					initialize();

					// TODO: STEP SUBSYSTEMS

				} catch(c65::type::exception &exc) {
					m_error = exc.to_string();
					result = EXIT_FAILURE;
				} catch(std::exception &exc) {
					m_error = exc.what();
					result = EXIT_FAILURE;
				}

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			int unload(
				__in c65_address_t base,
				__in c65_address_t offset
				)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY();

				try {
					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Runtime unload request", "%u(%04x), %u(%04x)",
						base.word, base.word, offset.word, offset.word);

					initialize();

					// TODO: UNLOAD MEMORY FROM BASE TO OFFSET

				} catch(c65::type::exception &exc) {
					m_error = exc.to_string();
					result = EXIT_FAILURE;
				} catch(std::exception &exc) {
					m_error = exc.what();
					result = EXIT_FAILURE;
				}

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

		protected:

			friend class c65::interface::singleton<c65::runtime>;

			runtime(void) :
				m_memory(c65::system::memory::instance())
			{
				TRACE_ENTRY();

				TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, C65 " loaded", "%s", VERSION_STRING());

				TRACE_EXIT();
			}

			runtime(
				__in const runtime &other
				) = delete;

			runtime &operator=(
				__in const runtime &other
				) = delete;

			void on_initialize(void) override
			{
				SDL_version version = {};

				TRACE_ENTRY();

				TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime initializing");

				if(SDL_Init(SDL_FLAGS)) {
					THROW_C65_RUNTIME_EXCEPTION_FORMAT(C65_RUNTIME_EXCEPTION_EXTERNAL, "SDL_Init failed! %s", SDL_GetError());
				}

				SDL_GetVersion(&version);

				TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "SDL loaded", "%i.%i.%i", version.major, version.minor, version.patch);

				m_memory.initialize();

				// TODO: INITIALIZE SUBSYSTEMS

				TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime initialized");

				TRACE_EXIT();
			}

			c65_byte_t on_read(
				__in c65_address_t address
				) const override
			{
				c65_byte_t result = MEMORY_FILL;

				TRACE_ENTRY_FORMAT("Address=%u(%04x)", address.word, address.word);

				switch(address.word) {
					case ADDRESS_INPUT:

						// TODO: READ FROM INPUT SUBSYSTEM

						break;
					case ADDRESS_MEMORY_HIGH_BEGIN ... ADDRESS_MEMORY_HIGH_END:
					case ADDRESS_MEMORY_STACK_BEGIN ... ADDRESS_MEMORY_STACK_END:
					case ADDRESS_MEMORY_ZERO_PAGE_BEGIN ... ADDRESS_MEMORY_ZERO_PAGE_END:
						result = m_memory.read(address);
						break;
					case ADDRESS_PROCESSOR_MASKABLE:
					case ADDRESS_PROCESSOR_NONMASKABLE:
					case ADDRESS_PROCESSOR_RESET:

						// TODO: READ FROM PROCESSOR SUBSYSTEM

						break;
					case ADDRESS_RANDOM:

						// TODO: READ FROM RANDOM SUBSYSTEM

						break;
					case ADDRESS_VIDEO_BEGIN ... ADDRESS_VIDEO_END:

						// TODO: READ FROM VIDEO SUBSYSTEM

						break;
					default:
						THROW_C65_RUNTIME_EXCEPTION_FORMAT(C65_RUNTIME_EXCEPTION_ADDRESS_INVALID,
							"%u(%04x)", address.word, address.word);
				}

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			void on_uninitialize(void) override
			{
				TRACE_ENTRY();

				TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime uninitializing");

				// TODO: UNINITIALIZE SUBSYSTEMS

				m_memory.uninitialize();

				SDL_Quit();

				TRACE_MESSAGE(LEVEL_INFORMATION, "SDL unloaded");

				TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime uninitialized");

				TRACE_EXIT();
			}

			void on_write(
				__in c65_address_t address,
				__in c65_byte_t value
				) override
			{
				TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address.word, address.word, value, value);

				switch(address.word) {
					case ADDRESS_MEMORY_HIGH_BEGIN ... ADDRESS_MEMORY_HIGH_END:
					case ADDRESS_MEMORY_STACK_BEGIN ... ADDRESS_MEMORY_STACK_END:
					case ADDRESS_MEMORY_ZERO_PAGE_BEGIN ... ADDRESS_MEMORY_ZERO_PAGE_END:
						m_memory.write(address, value);
						break;
					case ADDRESS_PROCESSOR_MASKABLE:
					case ADDRESS_PROCESSOR_NONMASKABLE:
					case ADDRESS_PROCESSOR_RESET:

						// TODO: WRITE TO PROCESSOR SUBSYSTEM

						break;
					case ADDRESS_VIDEO_BEGIN ... ADDRESS_VIDEO_END:

						// TODO: WRITE TO VIDEO SUBSYSTEM

						break;
					default:
						THROW_C65_RUNTIME_EXCEPTION_FORMAT(C65_RUNTIME_EXCEPTION_ADDRESS_INVALID,
							"%u(%04x)", address.word, address.word);
				}

				TRACE_EXIT();
			}

			std::string m_error;

			c65::system::memory &m_memory;
	};
}

int
c65_action(
	__in const c65_action_t *request,
	__in c65_action_t *response
	)
{
	int result;

	TRACE_ENTRY_FORMAT("Request=%p, Response=%p", request, response);

	result = c65::runtime::instance().action(request, response);

	TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
	return result;
}

void
c65_cleanup(void)
{
	TRACE_ENTRY();

	c65::runtime::instance().uninitialize();

	TRACE_EXIT();
}

const char *
c65_error(void)
{
	const char *result;

	TRACE_ENTRY();

	result = c65::runtime::instance().error();

	TRACE_EXIT_FORMAT("Result=%p", result);
	return result;
}

int
c65_interrupt(
	__in int type
	)
{
	int result;

	TRACE_ENTRY_FORMAT("Type=%i(%s)", type, INTERRUPT_STRING(type));

	result = c65::runtime::instance().interrupt(type);

	TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
	return result;
}

int
c65_load(
	__in const c65_byte_t *data,
	__in c65_address_t length,
	__in c65_address_t base
	)
{
	int result;

	TRACE_ENTRY_FORMAT("Data[%u(%04x)]=%p, Base=%u(%04x)", length.word, length.word, data, base.word, base.word);

	result = c65::runtime::instance().load(data, length, base);

	TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
	return result;
}

int
c65_reset(void)
{
	int result;

	TRACE_ENTRY();

	result = c65::runtime::instance().reset();

	TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
	return result;
}

int
c65_run(void)
{
	int result;

	TRACE_ENTRY();

	result = c65::runtime::instance().run();

	TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
	return result;
}

int
c65_step(void)
{
	int result;

	TRACE_ENTRY();

	result = c65::runtime::instance().step();

	TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
	return result;
}

int
c65_unload(
	__in c65_address_t base,
	__in c65_address_t offset
	)
{
	int result;

	TRACE_ENTRY_FORMAT("Base=%u(%04x), Offset=%u(%04x)", base.word, base.word, offset.word, offset.word);

	result = c65::runtime::instance().unload(base, offset);

	TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
	return result;
}

const char *
c65_version(void)
{
	const char *result;

	TRACE_ENTRY();

	result = VERSION_STRING();

	TRACE_EXIT_FORMAT("Result=%p", result);
	return result;
}
