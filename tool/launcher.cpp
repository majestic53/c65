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

#include <readline/history.h>
#include <readline/readline.h>
#include "../include/interface/singleton.h"
#include "../include/type/buffer.h"
#include "../include/c65.h"
#include "./launcher_type.h"

namespace c65 {

	namespace tool {

		class launcher :
				public c65::interface::singleton<c65::tool::launcher> {

			public:

				~launcher(void)
				{
					TRACE_ENTRY();
					TRACE_EXIT();
				}

				int run(
					__in int argc,
					__in const char *argv[]
					)
				{
					int result = EXIT_SUCCESS;

					TRACE_ENTRY_FORMAT("Argument[%i]=%p", argc, argv);

					if(argc >= ARGUMENT_MIN) {

						try {
							initialize();
							parse(std::vector<std::string>(argv + 1, argv + argc));

							if(m_help) {
								std::cout << usage(true) << std::endl;
							} else if(m_version) {
								std::cout << version(false) << std::endl;
							} else {

								if(!m_quiet) {
									std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << version(true)
										<< LEVEL_COLOR(LEVEL_NONE) << std::endl;
								}

								result = (m_debug ? debug() : run());
								if(result != EXIT_SUCCESS) {
									THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(
										C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
								}

								if(!m_quiet) {
									std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << "Exiting." << std::endl
										<< LEVEL_COLOR(LEVEL_NONE);
								}
							}
						} catch(c65::type::exception &exc) {

							if(!m_quiet) {
								std::cerr << LEVEL_COLOR(LEVEL_ERROR) << "Error: " << exc.to_string() << std::endl
									<< LEVEL_COLOR(LEVEL_NONE);
							}

							result = EXIT_FAILURE;
						} catch(std::exception &exc) {

							if(!m_quiet) {
								std::cerr << LEVEL_COLOR(LEVEL_ERROR) << "Error: " << exc.what() << std::endl
									<< LEVEL_COLOR(LEVEL_NONE);
							}

							result = EXIT_FAILURE;
						}
					} else {
						std::cout << usage(false) << std::endl;
					}

					uninitialize();

					TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
					return result;
				}

			protected:

				friend class c65::interface::singleton<c65::tool::launcher>;

				launcher(void) :
					m_debug(false),
					m_help(false),
					m_quiet(false),
					m_version(false)
				{
					TRACE_ENTRY();
					TRACE_EXIT();
				}

				launcher(
					__in const launcher &other
					) = delete;

				launcher &operator=(
					__in const launcher &other
					) = delete;

				int debug(void)
				{
					bool complete = false;
					int result = EXIT_SUCCESS;

					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Launcher console request");

					stifle_history(HISTORY_MAX);

					while(!complete) {

						try {
							int type;
							char *input;
							std::string action;
							std::stringstream stream;
							std::vector<std::string> arguments;

							input = readline(PROMPT);
							if(!input) {
								continue;
							}

							add_history(input);
							stream << input;

							if(stream.str().empty()) {
								continue;
							}

							stream >> action;

							while(stream) {
								arguments.push_back(std::string());
								stream >> arguments.back();

								if(arguments.back().empty()) {
									arguments.erase(arguments.end() - 1);
								}
							}

							if(!ACTION_SUPPORTED(action)) {
								THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(
									C65_TOOL_LAUNCHER_EXCEPTION_ACTION_INVALID, "%s", STRING(action));
							}

							type = ACTION_TYPE(action);
							if(arguments.size() != ACTION_ARGUMENT_LENGTH(type)) {
								THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(
									C65_TOOL_LAUNCHER_EXCEPTION_ARGUMENT_MISMATCH, "%s", input);
							}

							switch(type) {
								case ACTION_DUMP:
									debug_action_dump(arguments);
									break;
								case ACTION_EXIT:
									complete = true;
									break;
								case ACTION_HELP:
									debug_action_help();
									break;
								case ACTION_INTERRUPT:
									debug_action_interrupt(arguments);
									break;
								case ACTION_LOAD:
									debug_action_load(arguments);
									break;
								case ACTION_READ_BYTE:
									debug_action_read_byte(arguments);
									break;
								case ACTION_READ_REGISTER:
									debug_action_read_register(arguments);
									break;
								case ACTION_READ_STATUS:
									debug_action_read_status();
									break;
								case ACTION_READ_WORD:
									debug_action_read_word(arguments);
									break;
								case ACTION_RESET:
									debug_action_reset();
									break;
								case ACTION_RUN:
									debug_action_run();
									break;
								case ACTION_STEP:
									debug_action_step();
									break;
								case ACTION_UNLOAD:
									debug_action_unload(arguments);
									break;
								case ACTION_VERSION:
									debug_action_version();
									break;
								case ACTION_WRITE_BYTE:
									debug_action_write_byte(arguments);
									break;
								case ACTION_WRITE_REGISTER:
									debug_action_write_register(arguments);
									break;
								case ACTION_WRITE_STATUS:
									debug_action_write_status(arguments);
									break;
								case ACTION_WRITE_WORD:
									debug_action_write_word(arguments);
									break;
								default:
									THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(
										C65_TOOL_LAUNCHER_EXCEPTION_ACTION_INVALID,
										"%s", STRING(action));
							}

							free(input);
						} catch(c65::type::exception &exc) {
							std::cerr << LEVEL_COLOR(LEVEL_ERROR) << "Error: " << exc.to_string() << std::endl
								<< LEVEL_COLOR(LEVEL_NONE);
						} catch(std::exception &exc) {
							std::cerr << LEVEL_COLOR(LEVEL_ERROR) << "Error: " << exc.what() << std::endl
								<< LEVEL_COLOR(LEVEL_NONE);
						}
					}

					TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
					return result;
				}

				void debug_action_dump(
					__in const std::vector<std::string> &arguments
					)
				{
					std::string buffer;
					c65_address_t address;
					c65_word_t count, end, offset;
					std::stringstream result, stream;

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					stream << std::hex << arguments.front();
					stream >> address.word;

					stream.clear();
					stream.str(std::string());
					stream << std::dec << arguments.back();
					stream >> count;

					offset = (address.word % BLOCK_WIDTH);
					if(offset) {
						address.word -= offset;
					}

					end = (address.word + (count * BLOCK_WIDTH));

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Dump", "%u(%04x), %u(%04x)", address.word, address.word,
						end - 1, end - 1);

					offset = (end - address.word);
					result << "[" << STRING_HEXIDECIMAL(c65_word_t, address.word) << "-"
						<< STRING_HEXIDECIMAL(c65_word_t, end - 1) << "] -- "
						<< STRING_FLOAT(offset / (float)std::kilo::num) << " KB (" << offset << " bytes)"
						<< std::endl << "      ";

					for(offset = 0; offset < BLOCK_WIDTH; ++offset) {
						result << " " << STRING_HEXIDECIMAL(c65_byte_t, offset);
					}

					result << std::endl << "      ";

					for(offset = 0; offset < BLOCK_WIDTH; ++offset) {
						result << " --";
					}

					for(; address.word != end; ++address.word) {
						char value;
						c65_action_t request = {}, response = {};

						if(!(address.word % BLOCK_WIDTH)) {

							if(!buffer.empty()) {
								result << "   " << buffer;
								buffer.clear();
							}

							result << std::endl << STRING_HEXIDECIMAL(c65_word_t, address.word) << " |";
						}

						request.type = C65_ACTION_READ_BYTE;
						request.address = address;

						TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Request", "%i(%s), %u(%04x)",
							request.type, ACTION_STRING(request.type), request.address.word, request.address.word);

						if(c65_action(&request, &response) != EXIT_SUCCESS) {
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s",
								c65_error());
						}

						TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Response", "%i(%s), %u(%02x)",
							response.type, ACTION_STRING(response.type), response.data.low, response.data.low);

						value = (char)response.data.low;
						buffer += ((std::isprint(value) && !std::isspace(value)) ? value : CHARACTER_FILL);

						result << " " << STRING_HEXIDECIMAL(c65_byte_t, response.data.low);
					}

					if(!buffer.empty()) {
						result << "   " << buffer;
						buffer.clear();
					}

					std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << result.str() << LEVEL_COLOR(LEVEL_NONE) << std::endl;

					TRACE_EXIT();
				}

				void debug_action_help(void)
				{
					int type = 0;
					std::stringstream result;

					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Help");

					for(; type <= ACTION_MAX; ++type) {
						std::stringstream stream;

						stream << ACTION_SHORT_STRING(type) << "|" << ACTION_LONG_STRING(type);

						if(type) {
							result << std::endl;
						}

						result << STRING_COLUMN() << stream.str() << STRING_COLUMN() << ACTION_ARGUMENT_STRING(type)
							<< ACTION_DESCRIPTION_STRING(type);
					}

					std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << result.str() << LEVEL_COLOR(LEVEL_NONE) << std::endl;

					TRACE_EXIT();
				}

				void debug_action_interrupt(
					__in const std::vector<std::string> &arguments
					)
				{
					int type;

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					if(!ACTION_INTERRUPT_SUPPORTED(arguments.front())) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERRUPT_INVALID, "%s",
							STRING(arguments.front()));
					}

					type = ACTION_INTERRUPT_TYPE(arguments.front());

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Interrupt", "%i(%s)", type, INTERRUPT_STRING(type));

					if(c65_interrupt(type) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_EXIT();
				}

				void debug_action_load(
					__in const std::vector<std::string> &arguments
					)
				{
					c65_address_t address;
					c65::type::buffer data;
					std::stringstream stream;

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					data.load(arguments.front());

					stream << std::hex << arguments.back();
					stream >> address.word;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Load", "%s, %.02f KB (%u bytes), %u(%04x)",
						STRING(arguments.front()), data.size() / (float)std::kilo::num, data.size(),
						address.word, address.word);

					if(c65_load((c65_byte_t *)&data[0], data.size(), address) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_EXIT();
				}

				void debug_action_read_byte(
					__in const std::vector<std::string> &arguments
					)
				{
					std::stringstream stream;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					request.type = C65_ACTION_READ_BYTE;
					stream << std::hex << arguments.front();
					stream >> request.address.word;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Request", "%i(%s), %u(%04x)",
						request.type, ACTION_STRING(request.type), request.address.word, request.address.word);

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Response", "%i(%s), %u(%02x)",
						response.type, ACTION_STRING(response.type), response.data.low, response.data.low);

					std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << STRING_HEXIDECIMAL(c65_byte_t, response.data.low)
						<< LEVEL_COLOR(LEVEL_NONE) << std::endl;

					TRACE_EXIT();
				}

				void debug_action_read_register(
					__in const std::vector<std::string> &arguments
					)
				{
					int type;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					request.type = C65_ACTION_READ_REGISTER;

					if(!ACTION_REGISTER_SUPPORTED(arguments.front())) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_REGISTER_INVALID, "%s",
							STRING(arguments.front()));
					}

					type = ACTION_REGISTER_TYPE(arguments.front());
					request.address.word = type;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Request", "%i(%s), %i(%s)",
						request.type, ACTION_STRING(request.type), request.address.word,
						REGISTER_STRING(request.address.word));

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					switch(type) {
						case C65_REGISTER_ACCUMULATOR:
						case C65_REGISTER_INDEX_X:
						case C65_REGISTER_INDEX_Y:
							TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Response", "%i(%s), %u(%02x)",
								response.type, ACTION_STRING(response.type), response.data.low, response.data.low);

							std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << STRING_HEXIDECIMAL(c65_byte_t, response.data.low)
								<< LEVEL_COLOR(LEVEL_NONE) << std::endl;
							break;
						case C65_REGISTER_PROGRAM_COUNTER:
						case C65_REGISTER_STACK_POINTER:
							TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Response", "%i(%s), %u(%04x)",
								response.type, ACTION_STRING(response.type), response.data.word,
								response.data.word);

							std::cout << LEVEL_COLOR(LEVEL_VERBOSE)
								<< STRING_HEXIDECIMAL(c65_word_t, response.data.word) << LEVEL_COLOR(LEVEL_NONE)
								<< std::endl;
							break;
						default:
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_REGISTER_INVALID,
								"%i(%s)", type, REGISTER_STRING(type));
					}

					TRACE_EXIT();
				}

				void debug_action_read_status(void)
				{
					int flag = (FLAG_MAX - 1);
					c65_action_t request = {}, response = {};

					TRACE_ENTRY();

					request.type = C65_ACTION_READ_STATUS;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Request", "%i(%s)", request.type, ACTION_STRING(request.type));

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Response", "%i(%s), %u(%02x)",
						response.type, ACTION_STRING(response.type), response.status.raw, response.status.raw);

					std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << STRING_HEXIDECIMAL(c65_byte_t, response.status.raw) << " [";

					for(; flag >= 0; flag--) {
						std::cout << FLAG_STRING(MASK_CHECK(response.status.raw, flag) ? flag : FLAG_UNUSED);
					}

					std::cout << "]" << LEVEL_COLOR(LEVEL_NONE) << std::endl;

					TRACE_EXIT();
				}

				void debug_action_read_word(
					__in const std::vector<std::string> &arguments
					)
				{
					std::stringstream stream;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					request.type = C65_ACTION_READ_WORD;
					stream << std::hex << arguments.front();
					stream >> request.address.word;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Request", "%i(%s), %u(%04x)",
						request.type, ACTION_STRING(request.type), request.address.word, request.address.word);

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Response", "%i(%s), %u(%04x)",
						response.type, ACTION_STRING(response.type), response.data.word, response.data.word);

					std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << STRING_HEXIDECIMAL(c65_word_t, response.data.word)
						<< LEVEL_COLOR(LEVEL_NONE) << std::endl;

					TRACE_EXIT();
				}

				void debug_action_reset(void)
				{
					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Reset");

					if(c65_reset() != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_EXIT();
				}

				void debug_action_run(void)
				{
					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Run");

					if(c65_run() != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_EXIT();
				}

				void debug_action_step(void)
				{
					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Step");

					if(c65_step() != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_EXIT();
				}

				void debug_action_unload(
					__in const std::vector<std::string> &arguments
					)
				{
					c65_word_t length;
					c65_address_t address;
					std::stringstream stream;

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					stream << std::hex << arguments.front();
					stream >> address.word;

					stream.clear();
					stream.str(std::string());
					stream << std::hex << arguments.back();
					stream >> length;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Unload", "%u(%04x), %u(%04x)", address.word, address.word,
						length, length);

					if(c65_unload(address, length) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_EXIT();
				}

				void debug_action_version(void)
				{
					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Version");

					std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << c65_version() << LEVEL_COLOR(LEVEL_NONE) << std::endl;

					TRACE_EXIT();
				}

				void debug_action_write_byte(
					__in const std::vector<std::string> &arguments
					)
				{
					std::stringstream stream;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					request.type = C65_ACTION_WRITE_BYTE;
					stream << std::hex << arguments.front();
					stream >> request.address.word;

					stream.clear();
					stream.str(std::string());
					stream << std::hex << arguments.back();
					stream >> request.data.word;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Request", "%i(%s), %u(%04x), %u(%02x)",
						request.type, ACTION_STRING(request.type), request.address.word, request.address.word,
						request.data.low, request.data.low);

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Response", "%i(%s)", response.type, ACTION_STRING(response.type));

					TRACE_EXIT();
				}

				void debug_action_write_register(
					__in const std::vector<std::string> &arguments
					)
				{
					int type;
					std::stringstream stream;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					request.type = C65_ACTION_WRITE_REGISTER;

					if(!ACTION_REGISTER_SUPPORTED(arguments.front())) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_REGISTER_INVALID, "%s",
							STRING(arguments.front()));
					}

					type = ACTION_REGISTER_TYPE(arguments.front());
					request.address.word = type;
					stream << std::hex << arguments.back();
					stream >> request.data.word;

					switch(type) {
						case C65_REGISTER_ACCUMULATOR:
						case C65_REGISTER_INDEX_X:
						case C65_REGISTER_INDEX_Y:
							TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Request", "%i(%s), %i(%s), %u(%02x)",
								request.type, ACTION_STRING(request.type), request.address.word,
								REGISTER_STRING(request.address.word), request.data.low, request.data.low);
							break;
						case C65_REGISTER_PROGRAM_COUNTER:
						case C65_REGISTER_STACK_POINTER:
							TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Request", "%i(%s), %i(%s), %u(%04x)",
								request.type, ACTION_STRING(request.type), request.address.word,
								REGISTER_STRING(request.address.word), request.data.word, request.data.word);
							break;
						default:
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_REGISTER_INVALID,
								"%i(%s)", type, REGISTER_STRING(type));
					}

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Response", "%i(%s)", response.type, ACTION_STRING(response.type));

					TRACE_EXIT();
				}

				void debug_action_write_status(
					__in const std::vector<std::string> &arguments
					)
				{
					uint16_t value;
					std::stringstream stream;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					request.type = C65_ACTION_WRITE_STATUS;
					stream << std::hex << arguments.front();
					stream >> value;

					request.status.raw = value;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Request", "%i(%s), %u(%02x)",
						request.type, ACTION_STRING(request.type), request.status.raw, request.status.raw);

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Response", "%i(%s)", response.type, ACTION_STRING(response.type));

					TRACE_EXIT();
				}

				void debug_action_write_word(
					__in const std::vector<std::string> &arguments
					)
				{
					std::stringstream stream;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					request.type = C65_ACTION_WRITE_WORD;
					stream << std::hex << arguments.front();
					stream >> request.address.word;

					stream.clear();
					stream.str(std::string());
					stream << std::hex << arguments.back();
					stream >> request.data.word;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Request", "%i(%s), %u(%04x), %u(%04x)",
						request.type, ACTION_STRING(request.type), request.address.word, request.address.word,
						request.data.word, request.data.word);

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Response", "%i(%s)", response.type, ACTION_STRING(response.type));

					TRACE_EXIT();
				}

				void on_initialize(void) override
				{
					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Launcher initializing");

					if(c65_reset() != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(
							C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_MESSAGE(LEVEL_INFORMATION, "Launcher initialized");

					TRACE_EXIT();
				}

				void on_uninitialize(void) override
				{
					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Launcher uninitializing");

					c65_cleanup();

					m_debug = false;
					m_help = false;
					m_quiet = false;
					m_version = false;

					TRACE_MESSAGE(LEVEL_INFORMATION, "Launcher uninitialized");

					TRACE_EXIT();
				}

				void parse(
					__in const std::vector<std::string> &arguments
					)
				{
					std::vector<std::string>::const_iterator argument;

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					for(argument = arguments.begin(); argument != arguments.end(); ++argument) {

						if(!ARGUMENT_SUPPORTED(*argument)) {
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_ARGUMENT_INVALID,
								"%s", STRING(*argument));
						}

						switch(ARGUMENT_TYPE(*argument)) {
							case ARGUMENT_DEBUG:
								m_debug = true;
								break;
							case ARGUMENT_HELP:
								m_help = true;
								break;
							case ARGUMENT_QUIET:
								m_quiet = true;
								break;
							case ARGUMENT_VERSION:
								m_version = true;
								break;
							default:
								THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(
									C65_TOOL_LAUNCHER_EXCEPTION_ARGUMENT_INVALID, "%s", STRING(*argument));
						}
					}

					TRACE_EXIT();
				}

				int run(void)
				{
					int result;

					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Launcher runtime request");

					result = c65_run();

					TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
					return result;
				}

				std::string usage(
					__in bool verbose
					) const
				{
					std::stringstream result;

					TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

					if(verbose) {
						result << version(true) << std::endl << std::endl;
					}

					result << USAGE;

					if(verbose) {
						int argument = 0;

						result << std::endl;

						for(; argument <= ARGUMENT_MAX; ++argument) {
							std::stringstream stream;

							stream << ARGUMENT_SHORT_STRING(argument) << "|" << ARGUMENT_LONG_STRING(argument);

							result << std::endl << STRING_COLUMN() << stream.str()
								<< ARGUMENT_DESCRIPTION_STRING(argument);
						}
					}

					TRACE_EXIT();
					return result.str();
				}

				std::string version(
					__in bool verbose
					) const
				{
					std::stringstream result;

					TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

					if(verbose) {
						result << C65 << " ";
					}

					result << c65_version();

					if(verbose) {
						result << std::endl << C65_NOTICE;
					}

					TRACE_EXIT();
					return result.str();
				}

				bool m_debug;

				bool m_help;

				bool m_quiet;

				bool m_version;
		};
	}
}

int
main(
	__in int argc,
	__in const char *argv[]
	)
{
	int result;

	TRACE_ENTRY_FORMAT("Argument[%i]=%p", argc, argv);

	result = c65::tool::launcher::instance().run(argc, argv);

	TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
	return result;
}
