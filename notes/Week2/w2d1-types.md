# W2 D1: Fixed-width types, sizes and overflow

## Why `int` is dangerous in embedded code
- C only guarantees minimum sizes: `int` is at least 16 bits, `long` at least 32. The compiler for each CPU picks the real size.
- `int` is 16 bits on an 8-bit AVR and 32 bits on Cortex-M4, so the same code can overflow on one chip and work on another, sometimes with no warning.
- Registers, protocol frames (UART/I2C/SPI) and data saved to flash have exact widths. Use `<stdint.h>` types: `uint8_t`, `int16_t`, `uint32_t`, `int64_t`.
- Choose a type by the **range of values you need** (and whether they can be negative), not by what the chip supports. Example: temperature in hundredths of °C, up to 350.00 and below zero → `int32_t`.

## Measured sizes (bytes)

| Type | PC (Windows, 64-bit, gcc) | NUCLEO-F401RE (Cortex-M4, arm-none-eabi-gcc) |
| --- | --- | --- |
| `char` | 1 | 1 |
| `short` | 2 | 2 |
| `int` | 4 | 4 |
| `long` | 4 | 4 |
| `long long` | 8 | 8 |
| `float` | 4 | 4 |
| `double` | 8 | 8 |
| `uint8_t` / `uint16_t` / `uint32_t` / `uint64_t` | 1 / 2 / 4 / 8 | 1 / 2 / 4 / 8 |
| `size_t` | **8** | **4** |
| `void *` | **8** | **4** |

- Only the pointer and `size_t` differ: the Cortex-M4 has 32-bit addresses (flash 0x08000000, SP 0x20018000), so a pointer needs 4 bytes.
- `size_t` follows the pointer width because it must be able to hold the size of any object.
- `long` is 4 on both, for different reasons: Windows chose it (64-bit Linux uses 8), and on ARM it matches the 32-bit CPU.

## Ranges
For N bits: unsigned 0 … 2^N − 1, signed −2^(N−1) … 2^(N−1) − 1.

| Type | Range |
| --- | --- |
| `uint8_t` | 0 … 255 |
| `int8_t` | −128 … 127 |
| `uint16_t` | 0 … 65535 |
| `int16_t` | −32768 … 32767 |
| `uint32_t` | 0 … 4 294 967 295 |
| `int32_t` | −2 147 483 648 … 2 147 483 647 |

## Overflow
- **Unsigned overflow is defined**: it wraps modulo 2^N. **Wrap by 2^N (the number of values), not by the max value.**
  - `uint8_t`: 255 + 1 = 0, 200 + 100 = 300 − 256 = **44**, 10 − 20 = −10 + 256 = **246**
  - (My first answers were 101 and 245: I used 255 as the step instead of 256.)
- **Signed overflow is undefined behaviour**: the standard says nothing about the result. At -O2 the compiler may assume it never happens and remove your checks. Never rely on it.
- Integer promotion: `a + b` with two `uint8_t` is computed as `int` (300), then cut to 8 bits when stored in a `uint8_t`. More on this in W2 D2 (bit shifts).

## Code techniques
- `sizeof` returns `size_t`, so print it with `%zu`, not `%d`.
- Element count of an array: `sizeof(arr) / sizeof(arr[0])`. `sizeof(arr)` alone is the size in **bytes**; it only equals the count when elements are 1 byte (e.g. `uint8_t`). With a `uint32_t` array it would be 4× too big → out-of-bounds reads.
- Macro: `#define SHOW(t) printf("%-10s %zu\n", #t, sizeof(t))`
  - `t` pastes the word as **code** (needed for `sizeof(t)`); `#t` turns it into a **string literal** (`"char"`) for the label. Plain `t` in the `%s` spot would pass a type name to a function → compile error.
  - No `;` at the end of a function-like macro: the caller writes it. An extra `;` breaks `if (x) SHOW(int); else ...`.
- On the board there's no screen, so the debugger is the printf: an array of structs `{ char name[10]; uint8_t size; }` shows each name next to its value in Watch.

## Gotchas (symptom → cause → fix)
- `warning: variable 'size_v' set but not used` + `ninja: build stopped` → the warning isn't what stopped the build; the real `error:` line was further up → `cmake --build --preset Debug 2>&1 | Select-String "error"`.
- `error: assignment to 'char' from 'char *' makes integer from pointer without a cast` → put a string (an address) into a single `char` → declare it `char *` (or `const char *`). Since GCC 14 this is an error, not a warning.
- Watch showed 216 / 0 / 0 for the last three sizes → I paused with no breakpoint, so the CPU was inside `HAL_Delay()`, not `main()`, and `sizes` is a local of `main` → breakpoint on `HAL_GPIO_TogglePin` in `main`. **To read a function's locals, stop inside that function.**
- Watch expression `sizeof(size_t)` → "unable to create object": `sizeof` is resolved at compile time, so the `size_t` typedef may not be in the .elf debug info for GDB to find. `sizeof(void *)` and `sizeof(uint64_t)` worked.
- The `for` loop was between `USER CODE END WHILE` and `USER CODE BEGIN 3` → CubeMX would delete it on regeneration → code goes after `BEGIN 3`.
