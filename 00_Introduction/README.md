# 00_Introduction — toolchain & debugger

First project in the series: no custom drivers yet, just the STM32CubeIDE/CMSIS/HAL toolchain and the on-chip debugger, using the Discovery board's two built-in LEDs (green on PG13, red on the adjacent pin).

## What this covers

- **CMSIS / HAL project layout** — how a CubeIDE project is structured, what CMSIS provides versus the vendor HAL, and why CMSIS exists at all (a standardized register/peripheral abstraction so the same application-level code ports across ARM Cortex-M vendors with minimal change).
- **Cortex-M4 basics** — core architecture, the Thumb-2 instruction set, and the FPU (used later for the PI controller's floating-point math in [`P1_Fan_Control`](../P1_Fan_Control)).
- **Debugger workflow**:
  - Breakpoints vs. watchpoints (halt on reaching a line vs. halt on a memory/register access)
  - Step Into vs. Step Over
  - Inspecting and editing live variable values
  - Inspecting peripheral/core registers directly
- **GPIO output, the direct way** — toggling both on-board LEDs via `HAL_GPIO_TogglePin`/`WritePin`/`SetPin`/`ResetPin`, single- and combined-pin writes, as the baseline that [`esd`](../modules/esd) later scales up to 13 simultaneous GPIO lines.

## Why it matters later

Every subsequent project in this repo builds on the GPIO/HAL fundamentals introduced here — in particular the distinction between configuring a pin (`HAL_GPIO_Init`) and driving it (`HAL_GPIO_WritePin`/`TogglePin`), which becomes non-trivial once 13+ pins need coordinated multiplexing in [`esd`](../modules/esd).
