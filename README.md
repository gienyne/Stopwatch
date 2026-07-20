# STM32F429 Embedded Systems Lab

Bare-metal / HAL-based firmware modules and two complete embedded applications built on the **STM32F429** (Cortex-M4F, Waveshare Open429Z-D board). Developed as part of a university embedded systems practicum and reworked here into a standalone, documented reference of the drivers and control logic — the coursework framing has been stripped out.

## What's here

**Drivers** (`/drivers`) — small, single-responsibility C modules on top of the STM32Cube HAL, each with its own README:

| Module | Peripheral(s) | What it does |
|---|---|---|
| [`esd`](drivers/esd) | GPIO | 4-digit 7-segment display driver (13 GPIO lines, multiplexed) |
| [`joystick`](drivers/joystick) | GPIO | 5-way joystick input (up/down/left/right/press) |
| [`lcd`](drivers/lcd) | SPI | ILI9341 TFT driver + graphics primitives (third-party base, adapted) |
| [`my_lcd`](drivers/my_lcd) | — | Bar graph and diagnostic drawing helpers on top of `lcd` |
| [`potis`](drivers/potis) | ADC | Polling-mode dual-potentiometer ADC reader |
| [`potis_dma`](drivers/potis_dma) | ADC + DMA | Continuous circular-DMA ADC sampling with ring-buffer averaging |
| [`dot`](drivers/dot) | Timer (PWM/OC) | Blink/dim an LED via hardware PWM, plus a stopwatch and a One-Pulse-Mode staircase-light timer, all timer-driven (no CPU polling) |
| [`utils`](drivers/utils) | GPIO, Timer | Timer-based blocking delay, atomic port-wide GPIO read/write |
| [`median`](drivers/median) | — | Fixed-window median filter for noisy sensor signals |

**Projects** (`/projects`) — applications built from the drivers above:

| Project | Summary |
|---|---|
| [`fan-control`](projects/fan-control) | Closed-loop PI speed controller for a PWM/tacho fan, setpoint from a potentiometer, live bar-graph readout on the LCD |
| [`weather-station`](projects/weather-station) | BME280-based sensor node publishing temperature/pressure/humidity on a CAN bus, with a multi-node LCD dashboard |

**Notes** (`/docs`) — write-ups on the underlying peripheral concepts (clock tree & timers, ADC/DMA, GPIO interrupts & NVIC, I²C, CAN bus) with the calculations and design decisions behind the drivers above.

## Hardware

- **MCU**: STM32F429 (Cortex-M4F, FPU, up to 180 MHz)
- **Board**: Waveshare Open429Z-D (STM32F429 Discovery-class board)
- **Peripherals used**: Waveshare Analog Test Board (potentiometers), ILI9341 TFT LCD, 4-digit 7-segment display, 5-way joystick, BME280 environmental sensor, CAN transceiver, PWM fan with tacho output

## License

MIT — see [LICENSE](LICENSE).
