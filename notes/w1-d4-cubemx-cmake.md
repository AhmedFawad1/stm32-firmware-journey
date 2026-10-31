# W1 D4: STM32CubeMX project generation with CMake output

Date: 2026-10-31

**Goal:** generate a blinky project in CubeMX as a CMake project and build a `.elf` from the terminal, without CubeIDE.

## The flow

CubeMX writes → CMake organises → arm-none-eabi-gcc compiles → the linker produces the `.elf`

- **CubeMX** is a code generator, not a compiler or IDE. It writes the files, then its job is done.
- The **linker** step (called by gcc at the end) is what actually writes the `.elf`.

## What CubeMX generated

| File | Purpose |
| --- | --- |
| `Core/Src/main.c`, `Core/Inc/main.h` | My application + init code (HAL) |
| `startup_stm32f446xx.s` | Code that runs before `main()` (I write my own in Week 3) |
| `STM32F446XX_FLASH.ld` | Linker script: where flash and RAM are |
| `CMakeLists.txt` | Build description (like D3, but bigger) |
| `cmake/gcc-arm-none-eabi.cmake` | Toolchain file: tells CMake to use the ARM compiler |
| `CMakePresets.json` | Saved shortcuts for the long cmake commands |
| `Drivers/STM32F4xx_HAL_Driver/` | HAL: chip-level functions (`HAL_Delay`, `HAL_GPIO_TogglePin`) |
| `Drivers/BSP/STM32F4xx-Nucleo/` | BSP: board-specific helpers (`BSP_LED_Toggle(LED2)`) |

## Toolchain file

- Sets the compiler: `CMAKE_C_COMPILER` = `arm-none-eabi-gcc` (prefix `arm-none-eabi-` + `gcc`).
- Sets CPU flags: `-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard`.
- Must be read at **configure** time, because configure writes `build.ninja`, which contains the exact compiler command lines. Ninja only executes them.
- CMake caches the compiler and absolute paths in the build folder. If the toolchain or the folder path changes → **delete `build/` and configure again.**
- "Check for working C compiler ... skipped": compiling a test program works on my PC, but **running** it can't, because the output is Cortex-M4 code and there is no OS on the target (`none` in `arm-none-eabi`).

## Presets

- **Configure presets** → step 1 (`cmake` writes `build.ninja`). **Build presets** → step 2 (Ninja runs the recipes).
- `"hidden": true` preset (`default`) = a base the others inherit from (generator, toolchain file, build folder). Not run directly.

```
cmake --preset Debug              # configure → build/Debug/build.ninja
cmake --build --preset Debug      # build → build/Debug/first_stm32_project.elf
```

## Results

- First build: `[22/22]`, FLASH 7216 B / 512 KB, RAM 1592 B / 128 KB (matches the F446RE).
- After adding blink code: `[2/2]` (compile `main.c` + link only), FLASH 7392 B.

Blink code goes inside `while (1)` between `/* USER CODE BEGIN 3 */` and `/* USER CODE END 3 */`. Code outside the USER CODE markers is overwritten when CubeMX regenerates.

```c
BSP_LED_Toggle(LED2);
HAL_Delay(500);
```

## Headers vs .c files

- `#include` the **header** (declaration), never the `.c` file.
- The compiler checks every call against the declaration; the linker fills in the address from the separately compiled `.o`.
- Including a `.c` file → the function gets compiled twice → "multiple definition" linker error.
- `main.c` includes `main.h`, which already includes the HAL header, so `HAL_Delay` is available.

## What broke / what I learned

- First picked the **STM32C031C6** (Cortex-M0+, 12 KB RAM) instead of my planned board. Started again from the **Board Selector** with NUCLEO-F446RE. Board not bought yet (Monday).
- LED names differ per board: LD2 on F446RE (BSP calls it `LED2`), LD4 on C031C6. Both on PA5.
- Project folder had a space (`CubeMX Projects`) → renamed to `CubeMX_Projects`, deleted `build/`, reconfigured.
- Called `BSP_LED_Toggle()` with no argument → compile error. Fix: pass a `Led_TypeDef` value (`LED2`).
- Couldn't find `HAL_Delay` in the BSP header: it's a **HAL** function, declared in the HAL header.
- Reading GCC errors: scroll up to the first `error:` line; `note:` lines are only extra context.

## Still to do

- D5: flash the `.elf` to the board and see it blink.
