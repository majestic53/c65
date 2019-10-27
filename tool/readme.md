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
c65 [args] [path]

-b|--base           <address>           Define base address
-d|--debug                              Enable debug mode
-h|--help                               Display help information
-n|--no-gui                             Hide window
-q|--quiet                              Enable quiet mode
-v|--version                            Display version information
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

The following example shows how to launch C65 with an input file:

```
$ ./bin/c65 file.bin
```

The following example shows how to launch C65 with an input file, loaded at a specific offset:

```
$ ./bin/c65 -b 600 file.bin
```

#### System keys

The following system keys are available:

|Key|Description       |
|---|------------------|
|Esc|Power off emulator|
|F1 |Reset emulator    |

#### Debug Mode

The following arguments are available:

```
b|break             <address>           Set breakpoint
bc|break-clear      <address>           Clear breakpoints
bl|break-list                           List breakpoints
c|cycle                                 Display cycle information
da|disassemble      <address> <count>   Disassemble memory contents
d|dump              <address> <count>   Dump memory contents
q|exit                                  Exit console
?|help                                  Display help information
i|interrupt         <type>              Interrupt processor
l|load              <path> <address>    Load data into memory
p|processor                             Display processor information
rb|read-byte        <address>           Read byte from memory
rr|read-register    <register>          Read register value
rs|read-status                          Read status value
rw|read-word        <address>           Read word from memory
rt|reset                                Reset processor
r|run                                   Run processor
st|stack                                Dump stack contents
s|step                                  Step processor
u|unload            <address> <length>  Unload data from memory
v|version                               Display version information
w|watch             <address>           Set watch
wc|watch-clear      <address>           Clear watches
wl|watch-list                           List watches
wb|write-byte       <address> <value>   Write byte to memory
wr|write-register   <register> <value>  Write register value
ws|write-status     <value>             Write status value
ww|write-word       <address> <value>   Write word to memory
```

#### Examples

The following example shows how to read a byte from memory at address 0x0600:

```
(c65) rb 0600

-OR-

(c65) read-byte 0600
```

The following example shows how to write the value 0xff into the accumulator register:

```
(c65) wr a ff

-OR-

(c65) write-register a ff
```
