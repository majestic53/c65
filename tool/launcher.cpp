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

								if(!m_path.empty()) {
									size_t length;

									length = load(m_path, m_base);

									if(!m_quiet) {
										std::cout << std::endl << LEVEL_COLOR(LEVEL_VERBOSE)
												<< STRING_COLUMN_SHORT() << "Path" << m_path
											<< std::endl << STRING_COLUMN_SHORT() << "Base"
												<< m_base.word << "("
												<< STRING_HEXIDECIMAL(c65_word_t, m_base.word) << ")"
											<< std::endl << STRING_COLUMN_SHORT() << "Length"
												<< STRING_FLOAT(length / (float)std::kilo::num)
												<< " KB (" << length << " bytes)"
											<< LEVEL_COLOR(LEVEL_NONE) << std::endl << std::endl;
									}
								}

								result = (m_debug ? debug() : run());
								if(result != EXIT_SUCCESS) {
									THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(
										C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
								}

								if(!m_quiet) {
									std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << "Exiting."
										<< LEVEL_COLOR(LEVEL_NONE) << std::endl;
								}
							}
						} catch(c65::type::exception &exc) {

							if(!m_quiet) {
								std::cerr << LEVEL_COLOR(LEVEL_ERROR) << exc.to_string() << LEVEL_COLOR(LEVEL_NONE)
									<< std::endl;
							}

							result = EXIT_FAILURE;
						} catch(std::exception &exc) {

							if(!m_quiet) {
								std::cerr << LEVEL_COLOR(LEVEL_ERROR) << exc.what() << LEVEL_COLOR(LEVEL_NONE)
									<< std::endl;
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
					m_base({}),
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

				static char **command_completion(
					__in const char *input,
					__in int begin,
					__in int end
					)
				{
					char **result;

					TRACE_ENTRY_FORMAT("Input=%p, Begin=%i, End=%i", input, begin, end);

					rl_attempted_completion_over = true;
					result = rl_completion_matches(input, c65::tool::launcher::command_generator);

					TRACE_EXIT_FORMAT("Result=%p", result);
					return result;
				}

				static char *command_generator(
					__in const char *input,
					__in int state
					)
				{
					char *result = nullptr;
					static size_t completion = 0;
					static std::vector<std::string> completions;

					TRACE_ENTRY_FORMAT("Input=%p, State=%i", input, state);

					if(!state) {
						int type;
						std::string in(input);

						completions.clear();
						completion = 0;

						for(type = 0; type <= ACTION_MAX; ++type) {
							std::string out = ACTION_LONG_STRING(type);

							if((out.size() >= in.size()) && !out.compare(0, in.size(), in)) {
								completions.push_back(out);
							}
						}

						for(type = 0; type <= ACTION_MAX; ++type) {
							std::string out = ACTION_SHORT_STRING(type);

							if((out.size() >= in.size()) && !out.compare(0, in.size(), in)) {
								completions.push_back(out);
							}
						}

						for(type = 0; type <= C65_INTERRUPT_MAX; ++type) {
							std::string out = ACTION_INTERRUPT_STRING(type);

							if((out.size() >= in.size()) && !out.compare(0, in.size(), in)) {
								completions.push_back(out);
							}
						}

						for(type = 0; type <= C65_REGISTER_MAX; ++type) {
							std::string out = ACTION_REGISTER_STRING(type);

							if((out.size() >= in.size()) && !out.compare(0, in.size(), in)) {
								completions.push_back(out);
							}
						}
					}

					if(completion < completions.size()) {
						result = strdup(completions.at(completion++).c_str());
					}

					TRACE_EXIT_FORMAT("Result=%p", result);
					return result;
				}

				int debug(void)
				{
					int result = EXIT_SUCCESS;

					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Launcher console request");

					if(!m_quiet) {
						result = debug_event_register();
					}

					if(result == EXIT_SUCCESS) {
						result = debug_console();
					}

					TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
					return result;
				}

				void debug_action_breakpoint(
					__in const std::vector<std::string> &arguments
					)
				{
					std::stringstream stream;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					stream << std::hex << arguments.front();
					stream >> request.address.word;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Breakpoint", "%u(%04x)",
						request.address.word, request.address.word);

					if(m_breakpoint.find(request.address.word) != m_breakpoint.end()) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_BREAKPOINT_DUPLICATE,
							"%u(%04x)", request.address.word, request.address.word);
					}

					request.type = C65_ACTION_BREAKPOINT_SET;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					m_breakpoint.insert(request.address.word);

					TRACE_EXIT();
				}

				void debug_action_breakpoint_clear(
					__in const std::vector<std::string> &arguments
					)
				{
					std::stringstream stream;
					c65_action_t request = {}, response = {};
					std::set<c65_word_t>::iterator breakpoint;

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					stream << std::hex << arguments.front();
					stream >> request.address.word;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Breakpoint-Clear", "%u(%04x)",
						request.address.word, request.address.word);

					breakpoint = m_breakpoint.find(request.address.word);
					if(breakpoint == m_breakpoint.end()) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_BREAKPOINT_NOT_FOUND,
							"%u(%04x)", request.address.word, request.address.word);
					}

					request.type = C65_ACTION_BREAKPOINT_CLEAR;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					m_breakpoint.erase(breakpoint);

					TRACE_EXIT();
				}

				void debug_action_breakpoint_list(void)
				{
					size_t index = 0;
					std::stringstream result;
					std::set<c65_word_t>::const_iterator breakpoint;

					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Breakpoint-List");

					result << "[" << m_breakpoint.size() << " breakpoints]" << std::endl;

					for(breakpoint = m_breakpoint.begin(); breakpoint != m_breakpoint.end(); ++breakpoint, ++index) {
						result << std::endl << "[" << index << STRING_COLUMN_SHORT() << "]"
							<< STRING_HEXIDECIMAL(c65_word_t, *breakpoint);
					}

					std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << result.str() << LEVEL_COLOR(LEVEL_NONE) << std::endl;

					TRACE_EXIT();
				}

				void debug_action_cycle(void)
				{
					c65_action_t request = {}, response = {};

					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Cycle");

					request.type = C65_ACTION_CYCLE;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << response.cycle << LEVEL_COLOR(LEVEL_NONE) << std::endl;

					TRACE_EXIT();
				}

				void debug_action_disassemble(
					__in const std::vector<std::string> &arguments
					)
				{
					c65_word_t count;
					c65_address_t address;
					std::stringstream result, stream;

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					stream << std::hex << arguments.front();
					stream >> address.word;

					stream.clear();
					stream.str(std::string());
					stream << std::dec << arguments.back();
					stream >> count;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Disassemble", "%u(%04x), %u", address.word, address.word,
						count);

					result << "[" << STRING_HEXIDECIMAL(c65_word_t, address.word) << ", " << count << " instructions]"
						<< std::endl;

					while(count--) {
						result << std::endl << disassemble(address);
					}

					std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << result.str() << LEVEL_COLOR(LEVEL_NONE) << std::endl;

					TRACE_EXIT();
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
						<< std::endl << std::endl << "      ";

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
					std::stringstream stream;

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					stream << std::hex << arguments.back();
					stream >> address.word;

					load(arguments.front(), address);

					TRACE_EXIT();
				}

				void debug_action_processor(void)
				{
					int flag = FLAG_MAX;
					std::stringstream result, stream;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Processor");

					request.type = C65_ACTION_CYCLE;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					result << STRING_COLUMN() << "Cycle" << response.cycle;

					request.type = C65_ACTION_READ_STATUS;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					request.type = C65_ACTION_STOPPED;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					result << std::endl << STRING_COLUMN() << "Stopped" << STRING_COLUMN() << response.data.word
						<< (response.data.word ? "true" : "false");

					request.type = C65_ACTION_WAITING;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					result << std::endl << STRING_COLUMN() << "Waiting" << STRING_COLUMN() << response.data.word
						<< (response.data.word ? "true" : "false");

					request.type = C65_ACTION_INTERRUPT_PENDING;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					result << std::endl << STRING_COLUMN() << "Interrupted" << STRING_COLUMN() << response.data.word
						<< (response.data.word ? "true" : "false");

					request.type = C65_ACTION_READ_STATUS;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					stream << STRING_HEXIDECIMAL(c65_byte_t, response.status.raw);
					result << std::endl << STRING_COLUMN() << "Status" << STRING_COLUMN() << stream.str();

					for(; flag >= 0; flag--) {
						result << FLAG_STRING(MASK_CHECK(response.status.raw, flag) ? flag : FLAG_UNUSED);
					}

					request.type = C65_ACTION_READ_REGISTER;
					request.address.word = C65_REGISTER_STACK_POINTER;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					stream.clear();
					stream.str(std::string());
					stream << STRING_HEXIDECIMAL(c65_word_t, response.data.word);
					result << std::endl << STRING_COLUMN() << "Stack-Pointer" << STRING_COLUMN() << stream.str();

					request.type = C65_ACTION_STACK_OVERFLOW;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					result << STACK_STRING(response.data.word ? STACK_OVERFLOW : STACK_NONE);

					request.type = C65_ACTION_STACK_UNDERFLOW;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					result << STACK_STRING(response.data.word ? STACK_UNDERFLOW : STACK_NONE);

					request.type = C65_ACTION_READ_REGISTER;
					request.address.word = C65_REGISTER_PROGRAM_COUNTER;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					stream.clear();
					stream.str(std::string());
					stream << STRING_HEXIDECIMAL(c65_word_t, response.data.word);
					result << std::endl << STRING_COLUMN() << "Program-Counter" << STRING_COLUMN() << stream.str()
						<< (int)response.data.word;

					request.type = C65_ACTION_READ_REGISTER;
					request.address.word = C65_REGISTER_ACCUMULATOR;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					stream.clear();
					stream.str(std::string());
					stream << STRING_HEXIDECIMAL(c65_byte_t, response.data.low);
					result << std::endl << STRING_COLUMN() << "Accumulator" << STRING_COLUMN() << stream.str()
						<< (int)response.data.low;

					request.type = C65_ACTION_READ_REGISTER;
					request.address.word = C65_REGISTER_INDEX_X;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					stream.clear();
					stream.str(std::string());
					stream << STRING_HEXIDECIMAL(c65_byte_t, response.data.low);
					result << std::endl << STRING_COLUMN() << "Index-X" << STRING_COLUMN() << stream.str()
						<< (int)response.data.low;

					request.type = C65_ACTION_READ_REGISTER;
					request.address.word = C65_REGISTER_INDEX_Y;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					stream.clear();
					stream.str(std::string());
					stream << STRING_HEXIDECIMAL(c65_byte_t, response.data.low);
					result << std::endl << STRING_COLUMN() << "Index-Y" << STRING_COLUMN() << stream.str()
						<< (int)response.data.low;

					std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << result.str() << LEVEL_COLOR(LEVEL_NONE) << std::endl;

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

				void debug_action_stack(void)
				{
					c65_word_t depth;
					std::stringstream result;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Stack");

					request.type = C65_ACTION_READ_REGISTER;
					request.address.word = C65_REGISTER_STACK_POINTER;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					depth = (ADDRESS_MEMORY_STACK_END - response.data.word);

					result << "[" << STRING_HEXIDECIMAL(c65_word_t, response.data.word) << ", " << (int)depth << " depth";

					request.type = C65_ACTION_STACK_OVERFLOW;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					if(response.data.word) {
						result << ", Overflow";
					}

					request.type = C65_ACTION_STACK_UNDERFLOW;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					if(response.data.word) {
						result << ", Underflow";
					}

					result << "]" << std::endl;

					request.type = C65_ACTION_READ_BYTE;

					while(depth) {
						request.address.word = (ADDRESS_MEMORY_STACK_END - depth + 1);

						if(c65_action(&request, &response) != EXIT_SUCCESS) {
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL,
								"%s", c65_error());
						}

						result << std::endl << "[" << STRING_HEXIDECIMAL(c65_word_t, request.address.word)
							<< STRING_COLUMN_SHORT() << "]" << STRING_HEXIDECIMAL(c65_byte_t, response.data.low);

						--depth;
					}

					std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << result.str()	<< LEVEL_COLOR(LEVEL_NONE) << std::endl;

					TRACE_EXIT();
				}

				void debug_action_step(void)
				{
					int flag = FLAG_MAX;
					bool stopped, waiting;
					c65_address_t address;
					std::stringstream result, stream;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Step");

					request.type = C65_ACTION_STOPPED;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL,
							"%s", c65_error());
					}

					stopped = response.data.word;

					request.type = C65_ACTION_WAITING;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL,
							"%s", c65_error());
					}

					waiting = response.data.word;

					if(stopped) {
						result << "Stopped";
					} else if(waiting) {
						result << "Waiting";
					} else {
						request.type = C65_ACTION_READ_REGISTER;
						request.address.word = C65_REGISTER_PROGRAM_COUNTER;

						if(c65_action(&request, &response) != EXIT_SUCCESS) {
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL,
								"%s", c65_error());
						}

						address.word = response.data.word;
						stream << disassemble(address);

						if(c65_step() != EXIT_SUCCESS) {
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL,
								"%s", c65_error());
						}

						result << stream.str() << STRING_COLUMN_SHORT() << " " << "A=";

						request.address.word = C65_REGISTER_ACCUMULATOR;

						if(c65_action(&request, &response) != EXIT_SUCCESS) {
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL,
								"%s", c65_error());
						}

						result << STRING_HEXIDECIMAL(c65_byte_t, response.data.low);
						request.address.word = C65_REGISTER_INDEX_X;

						if(c65_action(&request, &response) != EXIT_SUCCESS) {
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL,
								"%s", c65_error());
						}

						result << ", X=" << STRING_HEXIDECIMAL(c65_byte_t, response.data.low);
						request.address.word = C65_REGISTER_INDEX_Y;

						if(c65_action(&request, &response) != EXIT_SUCCESS) {
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL,
								"%s", c65_error());
						}

						result << ", Y=" << STRING_HEXIDECIMAL(c65_byte_t, response.data.low);
						request.address.word = C65_REGISTER_STACK_POINTER;

						if(c65_action(&request, &response) != EXIT_SUCCESS) {
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL,
								"%s", c65_error());
						}

						result << ", SP=" << STRING_HEXIDECIMAL(c65_word_t, response.data.word);

						request.type = C65_ACTION_READ_STATUS;

						if(c65_action(&request, &response) != EXIT_SUCCESS) {
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL,
								"%s", c65_error());
						}

						result << ", P=" << STRING_HEXIDECIMAL(c65_byte_t, response.status.raw) << " [";

						for(; flag >= 0; flag--) {
							result << FLAG_STRING(MASK_CHECK(response.status.raw, flag) ? flag : FLAG_UNUSED);
						}

						result << "]";
					}

					if(!m_quiet) {
						std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << result.str() << LEVEL_COLOR(LEVEL_NONE) << std::endl;
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

				void debug_action_watch(
					__in const std::vector<std::string> &arguments
					)
				{
					std::stringstream stream;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					stream << std::hex << arguments.front();
					stream >> request.address.word;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Watch", "%u(%04x)", request.address.word, request.address.word);

					if(m_watch.find(request.address.word) != m_watch.end()) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_WATCH_DUPLICATE,
							"%u(%04x)", request.address.word, request.address.word);
					}

					request.type = C65_ACTION_WATCH_SET;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					m_watch.insert(request.address.word);

					TRACE_EXIT();
				}

				void debug_action_watch_clear(
					__in const std::vector<std::string> &arguments
					)
				{
					std::stringstream stream;
					std::set<c65_word_t>::iterator watch;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY_FORMAT("Argument[%u]=%p", arguments.size(), &arguments);

					stream << std::hex << arguments.front();
					stream >> request.address.word;

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Watch-Clear", "%u(%04x)",
						request.address.word, request.address.word);

					watch = m_watch.find(request.address.word);
					if(watch == m_watch.end()) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_WATCH_NOT_FOUND,
							"%u(%04x)", request.address.word, request.address.word);
					}

					request.type = C65_ACTION_WATCH_CLEAR;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					m_watch.erase(watch);

					TRACE_EXIT();
				}

				void debug_action_watch_list(void)
				{
					size_t index = 0;
					std::stringstream result;
					std::set<c65_word_t>::const_iterator watch;

					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Watch-List");

					result << "[" << m_watch.size() << " watches]" << std::endl;

					for(watch = m_watch.begin(); watch != m_watch.end(); ++watch, ++index) {
						result << std::endl << "[" << index << STRING_COLUMN_SHORT() << "]"
							<< STRING_HEXIDECIMAL(c65_word_t, *watch);
					}

					std::cout << LEVEL_COLOR(LEVEL_VERBOSE) << result.str() << LEVEL_COLOR(LEVEL_NONE) << std::endl;

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

				int debug_console(void)
				{
					bool complete = false;
					int result = EXIT_SUCCESS;

					TRACE_ENTRY();

					rl_attempted_completion_function = c65::tool::launcher::command_completion;
					stifle_history(HISTORY_MAX);

					while(!complete) {

						try {
							int type;
							char *input;
							std::string action;
							std::stringstream stream;
							std::vector<std::string> arguments;

							input = readline(PROMPT);
							if(input) {

								if(std::strlen(input)) {
									add_history(input);
									m_command = input;
								}

								free(input);
								input = nullptr;
							}

							stream << m_command;

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
									C65_TOOL_LAUNCHER_EXCEPTION_ARGUMENT_MISMATCH, "%s", STRING(m_command));
							}

							switch(type) {
								case ACTION_BREAKPOINT:
									debug_action_breakpoint(arguments);
									break;
								case ACTION_BREAKPOINT_CLEAR:
									debug_action_breakpoint_clear(arguments);
									break;
								case ACTION_BREAKPOINT_LIST:
									debug_action_breakpoint_list();
									break;
								case ACTION_CYCLE:
									debug_action_cycle();
									break;
								case ACTION_DISASSEMBLE:
									debug_action_disassemble(arguments);
									break;
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
								case ACTION_PROCESSOR:
									debug_action_processor();
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
								case ACTION_STACK:
									debug_action_stack();
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
								case ACTION_WATCH:
									debug_action_watch(arguments);
									break;
								case ACTION_WATCH_CLEAR:
									debug_action_watch_clear(arguments);
									break;
								case ACTION_WATCH_LIST:
									debug_action_watch_list();
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
						} catch(c65::type::exception &exc) {
							std::cerr << LEVEL_COLOR(LEVEL_ERROR) << exc.to_string() << LEVEL_COLOR(LEVEL_NONE)
								<< std::endl;
						} catch(std::exception &exc) {
							std::cerr << LEVEL_COLOR(LEVEL_ERROR) << exc.what() << LEVEL_COLOR(LEVEL_NONE)
								<< std::endl;
						}
					}

					TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
					return result;
				}

				static void debug_event(
					__in c65_event_t *event
					)
				{
					TRACE_ENTRY_FORMAT("Event=%p", event);

					if(event) {
						std::stringstream result;

						result << EVENT_STRING(event->type) << " event";

						switch(event->type) {
							case C65_EVENT_BREAKPOINT:
							case C65_EVENT_INTERRUPT_ENTRY:
							case C65_EVENT_INTERRUPT_EXIT:
							case C65_EVENT_STACK_OVERFLOW:
							case C65_EVENT_STACK_UNDERFLOW:
							case C65_EVENT_STOP_ENTRY:
							case C65_EVENT_STOP_EXIT:
							case C65_EVENT_SUBROUTINE_ENTRY:
							case C65_EVENT_SUBROUTINE_EXIT:
							case C65_EVENT_WAIT_ENTRY:
							case C65_EVENT_WAIT_EXIT:
								result << ": " << STRING_HEXIDECIMAL(c65_word_t, event->address.word);
								break;
							case C65_EVENT_ILLEGAL_INSTRUCTION:
							case C65_EVENT_WATCH:
								result << ": " << STRING_HEXIDECIMAL(c65_word_t, event->address.word) << ", "
									<< STRING_HEXIDECIMAL(c65_byte_t, event->data.low);
								break;
							default:
								break;
						}

						std::cout << LEVEL_COLOR(LEVEL_WARNING) << result.str() << LEVEL_COLOR(LEVEL_NONE)
							<< std::endl;
					}

					TRACE_EXIT();
				}

				int debug_event_register(void)
				{
					int result = EXIT_SUCCESS, type = 0;

					TRACE_ENTRY();

					for(; type <= C65_EVENT_MAX; ++type) {

						result = c65_event_handler(type, (c65_event_hdlr)&c65::tool::launcher::debug_event);
						if(result != EXIT_SUCCESS) {
							break;
						}
					}

					TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
					return result;
				}

				std::string disassemble(
					__inout c65_address_t &address
					)
				{
					size_t pad;
					std::vector<c65_byte_t> data;
					std::stringstream result, stream;
					std::vector<c65_byte_t>::iterator byte;
					c65_action_t request = {}, response = {};

					TRACE_ENTRY_FORMAT("Address=%u(%04x)", address.word, address.word);

					stream.clear();
					stream.str(std::string());
					stream << STRING_HEXIDECIMAL(c65_word_t, address.word);
					result << STRING_COLUMN_SHORT() << stream.str();

					request.type = C65_ACTION_READ_BYTE;
					request.address = address;
					++address.word;

					if(c65_action(&request, &response) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL,
							"%s", c65_error());
					}

					data.push_back(response.data.low);
					const instruction_t &instruction = INSTRUCTION(data.back());

					stream.clear();
					stream.str(std::string());
					stream << INSTRUCTION_MODE_STRING(instruction.mode);
					result << INSTRUCTION_STRING(instruction.type) << " " << STRING_COLUMN_SHORT() << stream.str();

					switch(instruction.length) {
						case INSTRUCTION_LENGTH_BYTE:
							request.type = C65_ACTION_READ_BYTE;
							request.address = address;
							++address.word;

							if(c65_action(&request, &response) != EXIT_SUCCESS) {
								THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(
									C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
							}

							data.push_back(response.data.low);

							stream.clear();
							stream.str(std::string());
							stream << STRING_HEXIDECIMAL(c65_byte_t, response.data.low);

							if(instruction.mode == INSTRUCTION_MODE_RELATIVE) {
								c65_word_t offset = (address.word + (int8_t)response.data.low);
								stream << " (" << STRING_HEXIDECIMAL(c65_word_t, offset) << ")";
							}

							result << STRING_COLUMN_SHORT() << stream.str();
							break;
						case INSTRUCTION_LENGTH_WORD:
							request.type = C65_ACTION_READ_WORD;
							request.address = address;
							address.word += INSTRUCTION_LENGTH_WORD;

							if(c65_action(&request, &response) != EXIT_SUCCESS) {
								THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(
									C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
							}

							data.push_back(response.data.low);
							data.push_back(response.data.high);

							stream.clear();
							stream.str(std::string());

							if(instruction.mode == INSTRUCTION_MODE_ZERO_PAGE_RELATIVE) {
								c65_word_t offset = (address.word + (int8_t)response.data.high);

								stream << STRING_HEXIDECIMAL(c65_byte_t, response.data.low) << ", "
									<< STRING_HEXIDECIMAL(c65_byte_t, response.data.high)
									<< " (" << STRING_HEXIDECIMAL(c65_word_t, offset) << ")";
							} else {
								stream << STRING_HEXIDECIMAL(c65_word_t, response.data.word);
							}

							result << STRING_COLUMN_SHORT() << stream.str();
							break;
						default:
							result << STRING_COLUMN_SHORT() << " ";
							break;
					}

					result << "[" << (int)instruction.cycle << "] {";

					for(byte = data.begin(); byte != data.end(); ++byte) {
						result << " " << STRING_HEXIDECIMAL(c65_byte_t, *byte);
					}

					result << " }";

					for(pad = data.size(); pad < (INSTRUCTION_LENGTH_WORD + 1); ++pad) {
						result << "   ";
					}

					TRACE_EXIT();
					return result.str();
				}

				size_t load(
					__in const std::string &path,
					__in c65_address_t base
					)
				{
					size_t result;
					c65::type::buffer data;

					TRACE_ENTRY_FORMAT("Path[%u]=%s, Base=%u(%04x)", path.size(), STRING(path), base.word, base.word);

					data.load(path);
					result = data.size();

					TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Load", "%s, %.02f KB (%u bytes), %u(%04x)",
						STRING(path), data.size() / (float)std::kilo::num, data.size(),
						base.word, base.word);

					if(c65_load((c65_byte_t *)&data[0], data.size(), base) != EXIT_SUCCESS) {
						THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_INTERNAL, "%s", c65_error());
					}

					TRACE_EXIT_FORMAT("Result=%u", result);
					return result;
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

					m_base = {};
					m_breakpoint.clear();
					m_command.clear();
					m_debug = false;
					m_help = false;
					m_path.clear();
					m_quiet = false;
					m_version = false;
					m_watch.clear();

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

						if(ARGUMENT_SUPPORTED(*argument)) {
							std::stringstream stream;

							switch(ARGUMENT_TYPE(*argument)) {
								case ARGUMENT_BASE:

									if(++argument == arguments.end()) {
										THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(
											C65_TOOL_LAUNCHER_EXCEPTION_ARGUMENT_UNDEFINED,
											"%s", STRING(*argument));
									}

									stream << std::hex << *argument;
									stream >> m_base.word;
									break;
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
										C65_TOOL_LAUNCHER_EXCEPTION_ARGUMENT_INVALID,
										"%s", STRING(*argument));
							}
						} else if(m_path.empty()) {
							m_path = *argument;
						} else {
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_ARGUMENT_REDEFINED,
								"%s", STRING(*argument));
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
								<< STRING_COLUMN() << ARGUMENT_PARAMETER_STRING(argument)
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

				c65_address_t m_base;

				std::set<c65_word_t> m_breakpoint;

				std::string m_command;

				bool m_debug;

				bool m_help;

				std::string m_path;

				bool m_quiet;

				bool m_version;

				std::set<c65_word_t> m_watch;
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

	TRACE_ENABLE();

	TRACE_ENTRY_FORMAT("Argument[%i]=%p", argc, argv);

	result = c65::tool::launcher::instance().run(argc, argv);

	TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);

	TRACE_DISABLE();

	return result;
}
