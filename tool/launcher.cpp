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
										<< std::endl << std::endl << LEVEL_COLOR(LEVEL_NONE);
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
					int result = EXIT_SUCCESS;

					TRACE_ENTRY();

					TRACE_MESSAGE(LEVEL_INFORMATION, "Launcher console request");

					// TODO: RUN DEBUG CONSOLE

					TRACE_EXIT_FORMAT("Result=%i(%x)", result, result);
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
							THROW_C65_TOOL_LAUNCHER_EXCEPTION_FORMAT(C65_TOOL_LAUNCHER_EXCEPTION_ARGUMENT_UNSUPPORTED,
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
									C65_TOOL_LAUNCHER_EXCEPTION_ARGUMENT_UNSUPPORTED, "%s", STRING(*argument));
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

					result << VERSION_STRING();

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
