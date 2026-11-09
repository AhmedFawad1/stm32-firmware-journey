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
DAY6
| Breakpoint | A marker on a line; the CPU stops just *before* that line runs. The F401 has 6 hardware breakpoints. |
| Halted | CPU stopped by the debugger. Only now does VS Code read variables and registers. |
| Continue (F5) | Let the CPU run until the next breakpoint. |
| Step Over (F10) | Run the current line, including any function it calls, then stop on the next line. |
| Step Into (F11) | If the line calls a function, go inside it and stop at its first line. |
| Step Out (Shift+F11) | Finish the current function and stop back where it was called. |
| Call stack | The chain of functions that led to the current line (who called whom). |
| Variables panel | Shows local and global variables automatically when halted; right-click → Set Value edits them. |
| Watch panel | A list of variables or expressions you choose to track; updates at every halt. |
| Set Value | Writes a new value into RAM while halted. Lost on reset; to make it permanent, edit the code, rebuild and flash. |
| Live Watch | Cortex-Debug feature that reads values while the CPU runs, without halting (see W9). |
| USER CODE BEGIN/END | CubeMX markers. Code between them survives regeneration; code outside is deleted. |
| Arm GNU Toolchain (arm-none-eabi-gcc) | 15.2.Rel1 | Cross-compiling for Cortex-M |
| CMake | 4.4.4 | Configure step (generates build.ninja) |
| Ninja | 1.13.2 | Build step (runs compiler + linker) |
| STM32CubeMX | 6.18.1 | Generates init code, startup file, linker script, CMake files |
| STM32CubeProgrammer CLI | 2.23.0 | Flashing from the terminal |
| xPack OpenOCD | 0.12.0 | GDB server for debugging |
| VS Code + Cortex-Debug | 1.145.1 | Debugging UI |
 
### Build
```bash
cmake --preset Debug          # configure: creates build/Debug/ and writes build.ninja
cmake --build --preset Debug  # build: Ninja compiles + links -> build/Debug/blinky.elf
```
Changed the compiler or moved the project folder? Delete `build/` and configure again (CMake caches absolute paths).
 
### Flash
```bash
STM32_Programmer_CLI -c port=SWD -w build/Debug/blinky.elf -v -rst
```
`-c` connect over SWD, `-w` write, `-v` verify (read back and compare), `-rst` reset so the program starts.
All options must be in **one** command: each run of the CLI disconnects when it exits, so a separate `-w` would have no connection.
 
### Debug
Chain: VS Code + Cortex-Debug → arm-none-eabi-gdb → OpenOCD (GDB server, port 3333) → ST-Link → SWD → STM32.
OpenOCD turns gdb commands (breakpoint, read variable) into SWD operations.
 
Key parts of `.vscode/launch.json`:
```json
"servertype": "openocd",
"configFiles": ["interface/stlink.cfg", "target/stm32f4x.cfg"],
"runToEntryPoint": "main"
```
F5 starts debugging and stops at `main()`.
 
### Gotchas I hit
Format: symptom → cause → fix
 
- `DEV_USB_COMM_ERR` when connecting → old ST-Link firmware (V2J28M18) → upgraded to V2J48M35 with the CubeProgrammer GUI
- "connection to target must be established first" → (your words)
- "GDB Server Quit Unexpectedly" → (your words)
- Flash command ran but nothing changed on the board → (your words)
- My code disappeared after regenerating in CubeMX (or would have) → (your words)