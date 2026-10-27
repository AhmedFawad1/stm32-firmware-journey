## Cross Compiler
- A cross compiler is compiler that runs on a separate cpu architecture and can produce a machine code that can run on entirely different cpu architecture.

## Compile
- Compile is a process that converts all the .c files into .o file separately.
- This process replaces references of function calls from another file with an empty space or a text that would be replaced by the actual memory address of that function during link process.

## Link
- In a link process all the object files are combined together to produce a .elf file and assign all memory addresses

## Tool Chain
- A tool chain is a set of programs that turns c source code into machine code that can be understood and run on a controller like stm32.
- It also contains tool to load and run the code.