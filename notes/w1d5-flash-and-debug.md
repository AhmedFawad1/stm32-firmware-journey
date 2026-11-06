# W1 D5: Flashing and debugging from the terminal and VS Code

**Date:** 2026-11-06
**Board:** Nucleo-F401RE (STM32F401xD/E, Cortex-M4, 512 KB flash, 96 KB RAM)
**Result:** Flashed blinky from the command line (LED now 5 s on / 5 s off) and debugged it in VS Code, stopping at `main()`.

---

## 1. The flashing chain

```
PC software  →  USB  →  ST-Link (debug probe)  →  SWD  →  STM32 flash + CPU
```

- **ST-Link**: the debug probe. On a Nucleo it's the small chip near the USB connector.
- **SWD**: a 2-wire link (SWDIO + SWCLK). The probe uses it to halt the CPU, read and write memory, and program flash.
- **Host software**: STM32CubeProgrammer CLI (flashing) or OpenOCD (flashing + GDB server).
- An **.elf** holds machine code plus the address of each section and the symbols. The flash tool uses the addresses, and the debugger uses the symbols.

## 2. STM32CubeProgrammer CLI

Install path (added to Path):
```
C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin
```

| Command | Purpose |
| --- | --- |
| `STM32_Programmer_CLI --version` | Check the install |
| `STM32_Programmer_CLI -l st` | List connected ST-Link probes (serial number, firmware) |
| `STM32_Programmer_CLI -c port=SWD` | Connect and read chip info only (writes nothing) |
| `STM32_Programmer_CLI -c port=SWD -w blinky.elf -v -rst` | **Flash, verify, reset** (the one to remember) |

Options: `-c` connect, `-w` write file, `-v` verify, `-rst` reset and run.

**Key lesson:** each run of `STM32_Programmer_CLI` is a separate session. It connects, does its work, and disconnects when it exits. Running `-w`, `-v` or `-rst` on its own fails ("connection to target must be established first") or does nothing. All the options must go in **one** command.

### Reading the connect output

| Field | My value | Meaning |
| --- | --- | --- |
| Voltage | 3.24 V | Target is powered |
| SWD freq | 4000 kHz | Speed of the SWD link |
| Device ID | 0x433 | Family ID: STM32F401xD/E |
| NVM size | 512 KB | Flash size |
| Device CPU | Cortex-M4 | Core (matches arm-none-eabi toolchain) |

## 3. Problems I hit and how I fixed them

| Symptom | Cause | Fix |
| --- | --- | --- |
| `ST-LINK error (DEV_USB_COMM_ERR)` | Old ST-Link firmware (`V2J28M18`) | Upgraded probe firmware via the CubeProgrammer GUI → Firmware upgrade. Now `V2J48M35` |
| `-w` printed nothing / LED unchanged | Typo in the filename (`blink.elf` vs `blinky.elf`) and no `-c` | Correct filename, all options in one command |
| `-v` / `-rst` alone → "connection must be established first" | Each CLI call is a new session | One command: `-c ... -w ... -v -rst` |
| VS Code: "GDB Server Quit Unexpectedly" | `configFiles` missing folder prefixes | Use `interface/stlink.cfg` and `target/stm32f4x.cfg` |

How to read the firmware string `V2J48M35`: **V2** = ST-Link/V2-1 probe, **J48** = SWD/JTAG firmware version, **M35** = USB mass-storage version.

**Tip:** always change something visible (such as the blink delay) before flashing, so you can prove the new code is really on the chip.

## 4. How debugging works

```
VS Code + Cortex-Debug  →  arm-none-eabi-gdb  →  OpenOCD (GDB server, port 3333)  →  ST-Link  →  STM32
```

- **OpenOCD** opens the probe and waits for GDB on a network port.
- **GDB** connects and uses the .elf symbols to map addresses back to C lines.
- **Cortex-Debug** starts both, using the settings in `launch.json`.

### Testing OpenOCD on its own
```
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg
```
Success = it keeps running and prints `Listening on port 3333 for gdb connections`. Stop it with Ctrl+C before using VS Code, because only one program can use the ST-Link at a time.

OpenOCD output worth knowing:
- `Cortex-M4 r0p1 processor detected`: reached the CPU.
- `6 breakpoints, 4 watchpoints`: hardware limits of this chip.
- `DEPRECATED ... transport` warning: harmless.
- `using 1800 kHz`: the nearest speed the probe supports. Normal.

### `.vscode/launch.json`
```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug blinky",
      "type": "cortex-debug",
      "request": "launch",
      "cwd": "${workspaceFolder}",
      "executable": "${workspaceFolder}/build/Debug/blinky.elf",
      "servertype": "openocd",
      "configFiles": [
        "interface/stlink.cfg",
        "target/stm32f4x.cfg"
      ],
      "runToEntryPoint": "main"
    }
  ]
}
```
Open the **blinky folder** itself in VS Code so `${workspaceFolder}` points to it. F5 starts debugging, Shift+F5 stops.

### What the debugger showed at startup
- `pc: 0x08001afc`: the CPU stopped in **flash** (0x0800....) at the reset handler, the code that runs before `main()` (Week 3 topic).
- `msp: 0x20018000`: the initial stack pointer = top of RAM (0x20000000 + 96 KB). The stack grows downwards.
- `automatically using hardware breakpoints`: flash can't be patched on the fly, so GDB uses the chip's hardware breakpoints.
- `CP1252 ... UTF-32` warning: harmless Windows encoding quirk.
- Stopped at `main.c:76  HAL_Init();`

## 5. Side note: CubeMX user code sections

Code placed outside `/* USER CODE BEGIN x */ ... /* USER CODE END x */` is **deleted** when CubeMX regenerates the project. My toggle + delay lines sat between `END WHILE` and `BEGIN 3`, so move them inside the `USER CODE BEGIN 3` / `END 3` block.

## 6. Check yourself
- [x] Board blinks after a command-line flash
- [x] VS Code debug session stops at `main()`
- [ ] Explain why `STM32_Programmer_CLI -rst` on its own fails
