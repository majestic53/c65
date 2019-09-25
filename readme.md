C65
=

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Build Status](https://travis-ci.com/majestic53/c65.svg?branch=master)](https://travis-ci.com/majestic53/c65)

A small 65C02 based emulator, written in C++

Table of Contents
=

1. [About](https://github.com/majestic53/c65#about) -- About the project
2. [Usage](https://github.com/majestic53/c65#usage) -- How to use the project
3. [Building](https://github.com/majestic53/c65#building) -- How to build the project
4. [Changelog](https://github.com/majestic53/c65#changelog) -- Log of project changes

About
=

C65 emulates the 65C02 microprocessor in a small sandbox environment.

Usage
=

#### Interface

The project is implemented in C++ and exposes a simple C-style API, described in ```include/c65.h``` and ```lib/libc65.a```:

##### Runtime Routines

|Name         |Description                |Signature                                                                           |
|-------------|---------------------------|------------------------------------------------------------------------------------|
|c65_action   |Perform action in emulator |```int c65_action(const c65_action_t *request, c65_action_t *response)```           |
|c65_cleanup  |Cleanup the emulator       |```void c65_cleanup(void)```                                                        |
|c65_interrupt|Interrupt the emulator     |```int c65_interrupt(int type)```                                                   |
|c65_load     |Load data into the emulator|```int c65_load(const c65_byte_t *data, c65_address_t length, c65_address_t base)```|
|c65_reset    |Reset the emulator         |```int c65_reset(void)```                                                           |
|c65_run      |Run the emulator           |```int c65_run(void)```                                                             |
|c65_step     |Step the emulator          |```int c65_step(void)```                                                            |
|c65_unload   |Unload data from emulator  |```int c65_unload(c65_address_t base, c65_address_t length)```                      |

##### Misc. Routines

|Name       |Description                        |Signature                          |
|-----------|-----------------------------------|-----------------------------------|
|c65_error  |Retrieve the last error as a string|```const char *c65_error(void)```  |
|c65_version|Retrieve the version as a string   |```const char *c65_version(void)```|

For an example usage of this API, see the launcher tool, under ```tool/```

Building
=

__NOTE__: Tested under MacOS with the GCC/Clang C++ compilers. Modification to the makefiles might be required to build with a different OS/compiler.

### Building from Makefile

#### Arguments

Run the following commands from the projects root directory:

```
$ export CXX=<COMPILER>
$ make <BUILD_TYPE>
```

Where COMPILER corrisponds to either ```clang++``` or ```g++```

Where BUILD_TYPE corrisponds to either ```release``` or ```debug```

#### Optional Arguments

To build with tracing enabled, run the following command from the project's root directory:

```
$ make TRACE=<LEVEL>
```

Where LEVEL corrisponds to the tracing verbosity (0=Off, 1=Error, 2=Warnings, 3=Information, 4=Verbose)

#### Examples

The following example shows how to build the release project, under the Clang C++ compiler:

```
$ export CXX=clang++
$ make release
```

The following example shows how to build the debug project with informational tracing, under the GCC C++ compiler:

```
$ export CXX=gcc
$ make TRACE=3
```

Changelog
=

Version 0.1.1939
-

*Updated: 9/24/2019*

* Added system processor class
* Minor code cleanup

*Updated: 9/23/2019*

* Added system video class

Version 0.1.1938
-

*Updated: 9/22/2019*

* Added system memory class
* Added runtime action support

*Updated: 9/20/2019*

* Added buffer type class

*Updated: 9/19/2019*

* Initial commit

*Changelog truncated (see file history for full log)*
