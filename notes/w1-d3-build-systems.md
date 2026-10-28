# W1 D3: Build systems (Make / CMake), revision notes

Date: 2026-10-28

## 1. Why build systems exist
- **Written recipe**: the same correct compile/link commands every time, for everyone who clones the repo.
- **Incremental builds**: only out-of-date work is redone.
- Edit one `.c` file → **1 compile** (that file → `.o`) + **1 link** of **all** `.o` files into the final program.
  The link always uses the whole set, because the output is one combined image and addresses can shift.

## 2. How Make decides what to rebuild
```make
uart.o: uart.c uart.h
	arm-none-eabi-gcc -c uart.c -o uart.o
```
- `target: prerequisites`, then a **Tab-indented** recipe.
- The recipe runs if the target is missing or **any prerequisite is newer** than the target (timestamps).
- Editing `uart.h` must rebuild `uart.o`: `#include` pastes the header into `uart.c` during preprocessing,
  so old values (e.g. a buffer size) are baked into the old `.o`.
- Weak point: if a header isn't listed in the rule, Make silently uses a stale `.o`.

## 3. CMake = build system generator
```
CMakeLists.txt → cmake (configure) → build.ninja / Makefile → ninja / make (build) → .o/.obj → program
  (you write)                          (generated, never edit)
```
- Tracks header dependencies **automatically**, so no header lists are needed.
- Works across operating systems and build tools.
- **Git:** commit `CMakeLists.txt`; ignore `build/` (generated output).

## 4. Minimal CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.20)
project(hello C)
add_executable(hello main.c)
```
| Line | What it does | If deleted |
| --- | --- | --- |
| `cmake_minimum_required` | Oldest CMake version this file is written for | CMake warns and guesses behaviour rules (policies) |
| `project(hello C)` | Names the project, enables the C language only | CMake warns and enables C **and** C++ |
| `add_executable(hello main.c)` | Target `hello` built from `main.c` (`hello.exe` on Windows, `.elf` for STM32) | Nothing gets built |

More files: `add_executable(hello main.c greet.c)`. Headers don't need listing.

## 5. Commands
| When | Command |
| --- | --- |
| First time, or after deleting `build/` | `cmake -S . -B build -G "Ninja"` |
| Every build after that (even after editing CMakeLists.txt) | `cmake --build build` |
| See every step (Ninja overwrites its progress line) | `cmake --build build -v` |
| Run the program (PowerShell) | `.\build\hello.exe` |

- `-S` = source folder, `-B` = build folder, `-G` = generator. All three go in **one** command.
- `-B` (capital) configures; `--build` builds. Flags are case-sensitive.
- To change the generator: delete `build/` first (the generator is cached in `CMakeCache.txt`).

## 6. What I observed
- Rebuild with no changes → `ninja: no work to do.`
- After editing `main.c` → `[1/2] Building C object CMakeFiles/hello.dir/main.c.obj`, `[2/2] Linking C executable hello.exe`.
- **The generator decides the compiler:** Visual Studio → MSVC `cl.exe`; Ninja → gcc found on PATH (MSYS2 `ucrt64`).

## 7. Mistakes I made (and the fix)
- Ran `-S`, `-B`, `-G` as separate commands → in-source build + default Visual Studio generator.
  Symptom: "Selecting Windows SDK…" / "Building for: Visual Studio". Fix: delete generated files, one command.
- Wrote `"ninja"`. The generator name is `"Ninja"`.
- Thought I must add `greet.h` to the Makefile. The Makefile is generated; headers are tracked automatically.
- Typed `add_executables`. It's `add_executable`.

## 8. Self-quiz (answer without looking, then check above)
1. You edit only `uart.c` in a 30-file project. What exactly re-runs?
2. What three parts make up a Make rule, and what decides whether the recipe runs?
3. Why must a header change rebuild the `.c` files that include it?
4. Why is CMake called a "generator"? What does it generate?
5. Which goes in Git: `CMakeLists.txt`, `build.ninja`, or both? Why?
6. What is an out-of-source build and why is it useful?
7. Write the configure and build commands from memory.
8. Explain each line of the minimal `CMakeLists.txt`.
9. Your configure output says "Building for: Visual Studio 17 2022". What went wrong?
10. Why did the Ninja build use gcc while the Visual Studio build used MSVC?
