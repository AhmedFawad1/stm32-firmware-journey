| Term | Plain meaning |
| --- | --- |
| Tool Chain | A tool chain is a set of programs that turns c source code into machine code that can be understood and run on a controller like stm32.
| Cross Compiler | A cross compiler is compiler that runs on a separate cpu architecture and can produce a machine code that can run on entirely different cpu architecture.
| Build system | Tool that stores the build recipe and re-runs only the steps whose inputs changed |
| Incremental build | Rebuilding only out-of-date outputs instead of everything |
| Make | Classic build tool; reads rules from a `Makefile` and compares file timestamps |
| Target | The file a rule produces (e.g. `uart.o`, `hello.exe`) |
| Prerequisite | A file the target depends on; if it is newer than the target, the target is rebuilt |
| Recipe | The command that builds the target; in a Makefile it must be indented with a real Tab |
| CMake | A build system *generator*: reads `CMakeLists.txt` and writes build files for Make, Ninja, Visual Studio… |
| CMakeLists.txt | The file you write that describes the project to CMake; committed to Git |
| Generator | The build tool CMake writes files for, chosen with `-G` (e.g. `"Ninja"`); case-sensitive |
| Ninja | Small, fast build tool; reads `build.ninja` |
| build.ninja | Generated recipe file Ninja reads; never edited by hand, never committed |
| Configure step | `cmake -S . -B build -G "Ninja"`: finds the compiler and generates the build files |
| Build step | `cmake --build build`: runs the generator's tool to compile and link |
| Out-of-source build | All generated files go in a separate folder (`build/`), keeping sources clean |
| In-source build | Generated files mixed into the source folder; avoid it |
| CMakeCache.txt | Generated file where CMake remembers settings (generator, compiler) for a build folder |
| Header dependency tracking | Compiler records which headers each `.c` included, so a header edit triggers a rebuild automatically |
| Native compiler | Builds programs for the same machine you build on (e.g. MSYS2 gcc → Windows `.exe`) |
| MSVC | Microsoft's C/C++ compiler (`cl.exe`), picked by the Visual Studio generator |
| .obj | Object file name CMake uses on Windows; same idea as `.o` |
| `cmake_minimum_required()` | Oldest CMake version the file is written for |
| `project()` | Names the project and declares its languages (`C` only, not C++) |
| `add_executable()` | Creates a target (program) from a list of `.c` files |
| STM32CubeMX | ST's graphical code generator: configure pins, clocks and peripherals, then it writes init code, startup file, linker script and build files |
| Toolchain file | A `.cmake` file that tells CMake which cross-compiler and CPU flags to use; read at configure time |
| CMake preset | A named, saved set of cmake options in `CMakePresets.json` (`cmake --preset Debug`); configure presets for step 1, build presets for step 2 |
| Hidden preset | A base preset (`"hidden": true`) that other presets inherit from; not run directly |
| BSP | Board Support Package: board-specific helper functions (e.g. `BSP_LED_Toggle`) built on top of the HAL |
| USER CODE section | The space between `/* USER CODE BEGIN */` and `/* USER CODE END */` comments; the only code CubeMX keeps when it regenerates |
| Declaration vs definition | Declaration (in a `.h`) tells the compiler a function's name and parameters; definition (in a `.c`) is the actual code the linker uses |
| arm-none-eabi | Target name: ARM CPU, no operating system, Embedded ABI (calling convention) |
##Day5
| Flashing | Writing your compiled program into the microcontroller's flash memory |
| Debug probe | Hardware between your PC and the chip that can program and control it (e.g. ST-Link, J-Link) |
| ST-Link | ST's debug probe; built into every Nucleo board |
| SWD | Serial Wire Debug: 2-wire link (SWDIO, SWCLK) used to program and debug Cortex-M chips |
| JTAG | Older, wider (4-5 wire) debug interface; SWD is the 2-wire Arm alternative |
| Probe firmware | The software running inside the ST-Link itself (e.g. `V2J48M35`); separate from your STM32 program |
| .elf | Output file from the linker: machine code + section addresses + symbols for debugging |
| Symbols | Names of functions and variables stored in the .elf so the debugger can show C code instead of raw addresses |
| STM32CubeProgrammer | ST's tool (GUI + CLI) for flashing and reading STM32 memory |
| OpenOCD | Open-source tool that talks to debug probes and runs a GDB server |
| GDB | GNU debugger; `arm-none-eabi-gdb` is the version for Arm microcontrollers |
| GDB server | Program (e.g. OpenOCD) that GDB connects to over a network port to control the real chip |
| Cortex-Debug | VS Code extension that starts the GDB server and GDB and gives you debug buttons |
| launch.json | VS Code file that tells the debugger what to run and how |
| Verify | Reading flash back after writing to confirm it matches the file |
| Halt | Pausing the CPU so its state can be inspected |
| Hardware breakpoint | Breakpoint implemented by the CPU's debug unit; needed for code in flash. Limited number (6 on the F401) |
| Watchpoint | Stops execution when a specific memory location is read or written (4 on the F401) |
| PC | Program Counter: register holding the address of the instruction being executed |
| MSP | Main Stack Pointer: register holding the current top of the stack |
| Reset handler | The first code the CPU runs after reset; it sets up memory, then calls `main()` |
| Device ID | Number identifying the STM32 family (0x433 = STM32F401xD/E) |
 
 