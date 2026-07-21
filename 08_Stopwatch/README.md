# 08_Stopwatch — timers + EXTI/NVIC

Combines hardware timers with interrupt-driven input: a 1/10 000 s resolution lap timer, using the on-board user button (via EXTI) to simulate a lap-triggering photogate, built on [`dot`](../modules/dot)'s stopwatch functions.

## What this covers

- **NVIC / EXTI fundamentals**: configuring a GPIO pin for interrupt-on-edge, the difference between an EXTI *interrupt* (drives an ISR) and an EXTI *event* (hardware-only pulse, no ISR), and why the EXTI peripheral exposes 23 lines despite a GPIO port only having 16 pins (the extra 7 are internal system/peripheral sources — RTC, PVD, USB/Ethernet wakeup, etc.)
- `USE_HAL_TIM_REGISTER_CALLBACKS`: what changes when it's set to 1 (per-timer callback registration instead of one shared callback dispatched by instance).
- High-resolution timekeeping: configuring a timer for 1/10 000 s ticks so that closely-spaced laps remain distinguishable.
- **Application**: first button press starts the overall clock; each subsequent press records a lap time under the running total, styled after a typical online stopwatch UI.
- **Prescaler/period arithmetic exercises** (worked in [`docs/clock-tree-and-timers.md`](../docs/clock-tree-and-timers.md)): deriving the prescaler for a 1 kHz tick from a 50 MHz clock, and the period needed for a 2-minute overflow — including why that specific period exceeds a 16-bit auto-reload register and how a 32-bit timer (or overflow-counting on a 16-bit one) resolves it.
- Timer-based `utils_delay_ms` (see [`utils`](../modules/utils)): replacing the software delay loop used since `01_ESD` with one driven by an actual hardware timer.

See [`dot`](../modules/dot)'s "Stopwatch" section for the API.
