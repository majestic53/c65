C65 Launcher
=

Table of Contents
=

1. [Usage](https://github.com/majestic53/c65/tree/master/tool#usage) -- How to use the project

Usage
=

### Interface

Launch from the project root directory:

```
$ ./bin/c65 [ARGS]
```

#### Arguments

The following arguments are available:

```
c65 [args]

-d|--debug              Enable debug mode
-h|--help               Display help information
-q|--quiet              Enable quiet mode
-v|--version            Display version information
```

#### Examples

The following example shows how to launch C65:

```
$ ./bin/c65
```

The following example shows how to launch C65 in debug mode:

```
$ ./bin/c65 -d
```

#### Debug Mode

The following arguments are available:

```
q|exit                                          Exit console
?|help                                          Display help information
i|interrupt             <type>                  Interrupt processor
l|load                  <path> <address>        Load data into memory
rb|read-byte            <address>               Read byte from memory
rr|read-register        <register>              Read register value
rs|read-status                                  Read status value
rw|read-word            <address>               Read word from memory
rt|reset                                        Reset processor
r|run                                           Run processor
s|step                                          Step processor
u|unload                <address> <length>      Unload data from memory
v|version                                       Display version information
wb|write-byte           <address> <value>       Write byte to memory
wr|write-register       <register> <value>      Write register value
ws|write-status         <value>                 Write status value
ww|write-word           <address> <value>       Write word to memory
```

#### Examples

The following example shows how to read a byte from memory at address 0x0600:

```
% rb 0600

-OR-

% read-byte 0600
```

The following example shows how to write the value 0xff into the accumulator register:

```
% wr a ff

-OR-

% write-register a ff
```
