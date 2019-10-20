C65
=

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Build Status](https://travis-ci.com/majestic53/c65.svg?branch=master)](https://travis-ci.com/majestic53/c65)

A small 65C02 emulator and debugger, written in C++

Table of Contents
=

1. [About](https://github.com/majestic53/c65#about) -- About the project
2. [Usage](https://github.com/majestic53/c65#usage) -- How to use the project
3. [Building](https://github.com/majestic53/c65#building) -- How to build the project
4. [Changelog](https://github.com/majestic53/c65#changelog) -- Log of project changes

About
=

C65 emulates the 65C02 microprocessor in a small sandbox environment, and is designed to be binarily compatible with programs generated using [skilldrick's Easy6502 assembler](https://skilldrick.github.io/easy6502/).

#### Features

The project sandbox has the following features:

* 1.2MHz operating frequency (20,000 cycles/frame)
* Support for all instructions/addressing-modes, as documented in the [W65C02S datasheet](http://www.westerndesigncenter.com/wdc/documentation/w65c02s.pdf)
* 65536 bytes (64 KB) of accesssible memory
* MMIO for video display output, keyboard input and random input

#### Memory Map

The project uses the following memory map. Keep in-mind that some of the memory locations, such as IO, are read-only:

![Demo](https://github.com/majestic53/c65/blob/master/docs/memory.png "C65 memory map")

|Section          |Address    |Size       |R/W|Description                                      |
|-----------------|-----------|-----------|---|-------------------------------------------------|
|IRQ vector (IV)  |FFFE - FFFF|2 bytes    |R/W|Maskable interrupt vector address                |
|Reset vector (IV)|FFFC - FFFD|2 bytes    |R/W|Reset interrupt vector address                   |
|NMI vector (IV)  |FFFA - FFFB|2 bytes    |R/W|Non-maskable interrupt vector address            |
|RAM              |0600 - FFF9|63994 bytes|R/W|General purpose memory                           |
|Video RAM (VRAM) |0200 - 05FF|1024 bytes |R/W|Video memory, where every byte represents a pixel|
|Stack            |0100 - 01FF|256 bytes  |R/W|Stack memory                                     |
|Key (IO)         |00FF       |1 byte     |R  |Last key pressed                                 |
|Random (IO)      |00FE       |1 byte     |R  |Random byte                                      |
|Zero Page        |0000 - 00FD|254 bytes  |R/W|Zero page memory                                 |

#### Input

The project supports basic IO through a series of memory-mapped (MMIO) registers at the top of zero page memory:

|Name  |Address|Description                                 |
|------|-------|--------------------------------------------|
|Key   |00FF   |The last keyboard key pressed, as ascii     |
|Random|00FE   |A random byte value re-generated every cycle|

#### Output

The project supports video output onto a 32x32 pixel screen. The pixels are stored in VRAM at addresses 0200 - 05FF, where each byte represents a pixel.

__NOTE__: Any pixels that exceed the color values in the table below will be converted to black.

The following colors are supported:

|Color      |Value|Channels (BGRA)   |
|-----------|-----|------------------|
|Black      |00   |0, 0, 0, 255      |
|White      |01   |255, 255, 255, 255|
|Red        |02   |0, 0, 255, 255    |
|Cyan       |03   |255, 255, 0, 255  |
|Purple     |04   |255, 0, 255, 255  |
|Green      |05   |0, 255, 0, 255    |
|Blue       |06   |255, 0, 0, 255    |
|Yellow     |07   |0, 255, 255, 255  |
|Orange     |08   |0, 127, 255, 255  |
|Brown      |09   |0, 60, 90, 255    |
|Light-Red  |0A   |20, 60, 255, 255  |
|Dark-Grey  |0B   |40, 40, 40, 255   |
|Grey       |0C   |100, 100, 100, 255|
|Light-Green|0D   |0, 255, 127, 255  |
|Light-Blue |0E   |255, 168, 0, 255  |
|Light-Grey |0F   |160, 160, 160, 255|

Usage
=

#### Interface

The project is implemented in C++ and exposes a simple C-style API, described in ```include/c65.h``` and ```lib/libc65.a```:

##### Runtime Routines

|Name         |Description                |Signature                                                         |
|-------------|---------------------------|------------------------------------------------------------------|
|c65_action   |Perform action in emulator |```int c65_action(const c65_action_t *, c65_action_t *)```        |
|c65_cleanup  |Cleanup the emulator       |```void c65_cleanup(void)```                                      |
|c65_interrupt|Interrupt the emulator     |```int c65_interrupt(int)```                                      |
|c65_load     |Load data into the emulator|```int c65_load(const c65_byte_t *, c65_dword_t, c65_address_t)```|
|c65_reset    |Reset the emulator         |```int c65_reset(void)```                                         |
|c65_run      |Run the emulator           |```int c65_run(void)```                                           |
|c65_step     |Step the emulator          |```int c65_step(void)```                                          |
|c65_unload   |Unload data from emulator  |```int c65_unload(c65_address_t, c65_dword_t)```                  |

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

Version 0.1.1942
-

*Updated: 10/19/2019*

* Added breakpoint/watch debug commands
* Added asynchronous event support

*Updated: 10/18/2019*

* Added jmp/jsr processor instruction handlers <s>(incomplete)</s>
* Added processor tracking of stack overflows/underflows
* Fixed bug in stack debug command

*Updated: 10/17/2019*

* Added test cases for return/transfer processor commands
* Aligned illegal nop instruction cycles/lengths with datasheet

*Updated: 10/16/2019*

* Minor code cleanup

*Updated: 10/15/2019*

* Added return/transfer processor command handlers
* Added debug stack command

*Updated: 10/14/2019*

* Added dex/dey/inx/iny/pha/php/phx/phy/pla/plp/plx/ply processor command handlers

Version 0.1.1941
-

*Updated: 10/13/2019*

* Added additional zero-page-relative addressing mode
* Added bbr/bbs/rmb/smb processor command handlers

*Updated: 10/12/2019*

* Added processor command handlers

*Updated: 10/11/2019*

* Minor code cleanup/documentation changes

*Updated: 10/10/2019*

* Added launcher load/base arguments
* Added gdb-like behavior to debug console

*Updated: 10/9/2019*

* Added processor command handler
* Minor bug fixes

*Updated: 10/8/2019*

* Added cycle, disassemble, processor debug commands

*Updated: 10/7/2019*

* Added debug console tab completion
* Added additional processor test cases

*Changelog truncated (see file history for full log)*
