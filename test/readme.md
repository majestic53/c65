C65 Test Suite
=

Regression test suite for the C65 project

Table of Contents
=

1. [About](https://github.com/majestic53/c65/tree/master/test#about) -- About the project
2. [Building](https://github.com/majestic53/c65/tree/master/test#building) -- How to build the project

About
=

This test suite performs regression testing during build-time. The following tests are available:

|Test                  |Count|Description               |
|----------------------|-----|--------------------------|
|c65::runtime          |8    |C65 runtime tests         |
|c65::system::memory   |2    |C65 memory system tests   |
|c65::system::processor|36   |C65 processor system tests|
|c65::type::buffer     |1    |C65 buffer type tests     |

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
