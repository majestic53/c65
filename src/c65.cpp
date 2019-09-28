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
#include "../include/system/processor.h"
#include "../include/system/video.h"
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
						case C65_ACTION_READ_BYTE:
							result = action_read_byte(request, response);
							break;
						case C65_ACTION_READ_REGISTER:
							result = action_read_word(request, response);
							break;
						case C65_ACTION_READ_STATUS:
							result = action_read_status(request, response);
							break;
						case C65_ACTION_READ_WORD:
							result = action_read_word(request, response);
							break;
						case C65_ACTION_WRITE_BYTE:
							result = action_write_byte(request, response);
							break;
						case C65_ACTION_WRITE_REGISTER:
							result = action_write_register(request, response);
							break;
						case C65_ACTION_WRITE_STATUS:
							result = action_write_status(request, response);
							break;
						case C65_ACTION_WRITE_WORD:
							result = action_write_word(request, response);
							break;
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

			int action_read_byte(
				__in const c65_action_t *request,
				__in c65_action_t *response
				)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY_FORMAT("Request=%p, Response=%p", request, response);

				response->data.word = read(request->address);

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			int action_read_register(
				__in const c65_action_t *request,
				__in c65_action_t *response
				)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY_FORMAT("Request=%p, Response=%p", request, response);

				response->data = m_processor.read_register(request->address.word);

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			int action_read_status(
				__in const c65_action_t *request,
				__in c65_action_t *response
				)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY_FORMAT("Request=%p, Response=%p", request, response);

				response->status = m_processor.read_status();

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			int action_read_word(
				__in const c65_action_t *request,
				__in c65_action_t *response
				)
			{
				c65_address_t address;
				int result = EXIT_SUCCESS;

				TRACE_ENTRY_FORMAT("Request=%p, Response=%p", request, response);

				address = request->address;
				response->data.low = read(address);
				++address.word;
				response->data.high = read(address);

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			int action_write_byte(
				__in const c65_action_t *request,
				__in c65_action_t *response
				)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY_FORMAT("Request=%p, Response=%p", request, response);

				write(request->address, request->data.low);

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			int action_write_register(
				__in const c65_action_t *request,
				__in c65_action_t *response
				)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY_FORMAT("Request=%p, Response=%p", request, response);

				m_processor.write_register(request->address.word, request->data);

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			int action_write_status(
				__in const c65_action_t *request,
				__in c65_action_t *response
				)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY_FORMAT("Request=%p, Response=%p", request, response);

				m_processor.write_status(request->status);

				TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
				return result;
			}

			int action_write_word(
				__in const c65_action_t *request,
				__in c65_action_t *response
				)
			{
				c65_address_t address;
				int result = EXIT_SUCCESS;

				TRACE_ENTRY_FORMAT("Request=%p, Response=%p", request, response);

				address = request->address;
				write(address, response->data.low);
				++address.word;
				write(address, response->data.high);

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
					m_processor.interrupt(type);
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
					uint32_t index;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Runtime load request", "[%u(%04x)]%p, %u(%04x)",
						length.word, length.word, data, base.word, base.word);

					initialize();

					for(index = 0; index < length.word; ++index) {
						c65_address_t address;

						address.word = ((index + length.word) & UINT16_MAX);
						switch(address.word) {
							case ADDRESS_MEMORY_HIGH_BEGIN ... ADDRESS_MEMORY_HIGH_END:
							case ADDRESS_MEMORY_STACK_BEGIN ... ADDRESS_MEMORY_STACK_END:
							case ADDRESS_MEMORY_ZERO_PAGE_BEGIN ... ADDRESS_MEMORY_ZERO_PAGE_END:
							case ADDRESS_PROCESSOR_MASKABLE_BEGIN ... ADDRESS_PROCESSOR_MASKABLE_END:
							case ADDRESS_PROCESSOR_NON_MASKABLE_BEGIN ... ADDRESS_PROCESSOR_NON_MASKABLE_END:
							case ADDRESS_PROCESSOR_RESET_BEGIN ... ADDRESS_PROCESSOR_RESET_END:
							case ADDRESS_VIDEO_BEGIN ... ADDRESS_VIDEO_END:
								write(address, data[index]);
								break;
							default:
								TRACE_MESSAGE_FORMAT(LEVEL_WARNING, "Attempted to load into read-only memory",
									"%u(%04x)", address.word, address.word);
								break;
						}
					}
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
					m_processor.reset(*this);
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
					int cycle = 0;
					uint32_t begin = 0, current = 0;

					TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime run request");

					initialize();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime loop entry");

					for(;;) {
						float frequency, rate;
						uint32_t end = SDL_GetTicks();

						rate = (end - begin);
						if(rate >= MILLISECONDS_PER_SECOND) {
							rate = (current - ((rate - MILLISECONDS_PER_SECOND) / (float)FRAMES_PER_SECOND));
							rate = ((rate > 0.f) ? rate : 0.f);

							TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Runtime framerate", "%.01f", rate);

#ifndef NDEBUG
							m_video.frame_rate(rate);
#endif // NDEBUG
							begin = end;
							current = 0;
						}

						if(!poll()) {
							TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime loop exiting");
							break;
						}

						cycle = (CYCLES_PER_FRAME - cycle);
						while(cycle > 0) {
							m_random = std::rand();
							cycle -= m_processor.step(*this);
						}

						m_video.render();

						frequency = (SDL_GetTicks() - end);
						if(frequency < FRAME_RATE) {
							SDL_Delay(FRAME_RATE - frequency);
						}

						++current;
					}

					TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime loop exit");
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

					m_random = std::rand();
					m_processor.step(*this);
					m_video.render();
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
				__in c65_address_t length
				)
			{
				int result = EXIT_SUCCESS;

				TRACE_ENTRY();

				try {
					uint32_t index;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Runtime unload request", "%u(%04x), %u(%04x)",
						base.word, base.word, length.word, length.word);

					initialize();

					for(index = base.word; index < (base.word + length.word); ++index) {
						c65_address_t address;

						address.word = (index & UINT16_MAX);
						switch(address.word) {
							case ADDRESS_MEMORY_HIGH_BEGIN ... ADDRESS_MEMORY_HIGH_END:
							case ADDRESS_MEMORY_STACK_BEGIN ... ADDRESS_MEMORY_STACK_END:
							case ADDRESS_MEMORY_ZERO_PAGE_BEGIN ... ADDRESS_MEMORY_ZERO_PAGE_END:
								write(address, MEMORY_FILL);
								break;
							case ADDRESS_PROCESSOR_MASKABLE_BEGIN ... ADDRESS_PROCESSOR_MASKABLE_END:
							case ADDRESS_PROCESSOR_NON_MASKABLE_BEGIN ... ADDRESS_PROCESSOR_NON_MASKABLE_END:
							case ADDRESS_PROCESSOR_RESET_BEGIN ... ADDRESS_PROCESSOR_RESET_END:
							case ADDRESS_VIDEO_BEGIN ... ADDRESS_VIDEO_END:
								write(address, MEMORY_ZERO);
								break;
							default:
								TRACE_MESSAGE_FORMAT(LEVEL_WARNING, "Attempted to unload from read-only memory",
									"%u(%04x)", address.word, address.word);
								break;
						}
					}
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
				m_key(0),
				m_memory(c65::system::memory::instance()),
				m_processor(c65::system::processor::instance()),
				m_random(0),
				m_video(c65::system::video::instance())
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

				std::srand(std::time(nullptr));
				m_key = 0;
				m_random = 0;

				m_memory.initialize();
				m_processor.initialize();
				m_video.initialize();

				TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime initialized");

				TRACE_EXIT();
			}

			c65_byte_t on_read(
				__in c65_address_t address
				) const override
			{
				c65_byte_t result = MEMORY_ZERO;

				TRACE_ENTRY_FORMAT("Address=%u(%04x)", address.word, address.word);

				switch(address.word) {
					case ADDRESS_KEY:
						result = m_key;
						break;
					case ADDRESS_MEMORY_HIGH_BEGIN ... ADDRESS_MEMORY_HIGH_END:
					case ADDRESS_MEMORY_STACK_BEGIN ... ADDRESS_MEMORY_STACK_END:
					case ADDRESS_MEMORY_ZERO_PAGE_BEGIN ... ADDRESS_MEMORY_ZERO_PAGE_END:
						result = m_memory.read(address);
						break;
					case ADDRESS_PROCESSOR_MASKABLE_BEGIN ... ADDRESS_PROCESSOR_MASKABLE_END:
					case ADDRESS_PROCESSOR_NON_MASKABLE_BEGIN ... ADDRESS_PROCESSOR_NON_MASKABLE_END:
					case ADDRESS_PROCESSOR_RESET_BEGIN ... ADDRESS_PROCESSOR_RESET_END:
						result = m_processor.read(address);
						break;
					case ADDRESS_RANDOM:
						result = m_random;
						break;
					case ADDRESS_VIDEO_BEGIN ... ADDRESS_VIDEO_END:
						result = m_video.read(address);
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

				m_video.uninitialize();
				m_processor.uninitialize();
				m_memory.uninitialize();

				m_random = 0;
				m_key = 0;

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
					case ADDRESS_PROCESSOR_MASKABLE_BEGIN ... ADDRESS_PROCESSOR_MASKABLE_END:
					case ADDRESS_PROCESSOR_NON_MASKABLE_BEGIN ... ADDRESS_PROCESSOR_NON_MASKABLE_END:
					case ADDRESS_PROCESSOR_RESET_BEGIN ... ADDRESS_PROCESSOR_RESET_END:
						m_processor.write(address, value);
						break;
					case ADDRESS_VIDEO_BEGIN ... ADDRESS_VIDEO_END:
						m_video.write(address, value);
						break;
					default:
						THROW_C65_RUNTIME_EXCEPTION_FORMAT(C65_RUNTIME_EXCEPTION_ADDRESS_INVALID,
							"%u(%04x)", address.word, address.word);
				}

				TRACE_EXIT();
			}

			bool poll(void)
			{
				bool result = true;
				SDL_Event event = {};

				TRACE_ENTRY();

				while(SDL_PollEvent(&event)) {

					switch(event.type) {
						case SDL_KEYDOWN:
						case SDL_KEYUP:

							if(!event.key.repeat) {
								m_key = SDL_GetKeyFromScancode(event.key.keysym.scancode);
							}
							break;
						case SDL_QUIT:
							result = false;
							break;
						default:
							break;
					}

					if(!result) {
						break;
					}
				}

				TRACE_EXIT_FORMAT("Result=%x", result);
				return result;
			}

			std::string m_error;

			c65_byte_t m_key;

			c65::system::memory &m_memory;

			c65::system::processor &m_processor;

			c65_byte_t m_random;

			c65::system::video &m_video;
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
	__in c65_address_t length
	)
{
	int result;

	TRACE_ENTRY_FORMAT("Base=%u(%04x), Length=%u(%04x)", base.word, base.word, length.word, length.word);

	result = c65::runtime::instance().unload(base, length);

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
