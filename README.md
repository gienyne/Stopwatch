# STM32F429 Embedded Systems Lab

<p align="center">

![Status](https://img.shields.io/badge/status-active-brightgreen)
![Platform](https://img.shields.io/badge/MCU-STM32F429-blue)
![Language](https://img.shields.io/badge/language-C%20(HAL)-orange)
![Architecture](https://img.shields.io/badge/architecture-Modular-success)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

</p>

> A progressive collection of STM32 embedded systems projects, starting from
> direct GPIO manipulation and evolving into reusable hardware drivers,
> interrupt-driven firmware, closed-loop motor control and distributed CAN
> applications.

Each exercise introduces one new embedded concept while reusing the modules
developed previously. Instead of treating every assignment as an isolated
project, the repository gradually evolves into a reusable embedded software
stack.

---

## Highlights

- 11 standalone STM32CubeIDE projects
- Modular reusable driver library
- GPIO, ADC, DMA, Timers, PWM, EXTI, NVIC
- SPI, I²C and CAN communication
- Bosch BME280 integration
- Closed-loop PI motor controller
- Distributed multi-node weather station
- Layered embedded software architecture

---

# Design Philosophy

Unlike traditional laboratory assignments where every exercise starts from
scratch, this repository follows an incremental engineering approach.

Each project introduces one new embedded concept while reusing the modules
developed previously. Drivers are implemented once, documented, improved over
time and shared across every subsequent project.

The result is a progressively growing embedded software stack rather than a
collection of unrelated exercises.

---

## Learning Journey

The repository follows the same progression as an embedded engineering course,
where every project introduces one new peripheral or concept while building on
the previous ones.

```text
GPIO
 │
 ▼
Joystick
 │
 ▼
LCD Graphics
 │
 ▼
ADC
 │
 ▼
DMA
 │
 ▼
Timers
 │
 ▼
PWM
 │
 ▼
Interrupts (EXTI / NVIC)
 │
 ▼
Closed-Loop PI Control
 │
 ▼
Distributed CAN Network
```

---

## Repository Structure

```text
STM32F429_Embedded_Systems/
│
├── 00_Introduction
├── 01_ESD
├── 02_Joystick
├── 03_LCD
├── 04_Potis
├── 05_Potis_DMA
├── 06_Blinky_Dot
├── 07_Dimming_Dot
├── 08_Stopwatch
│
├── P1_Fan_Control
├── P2_Weatherstation
│
├── modules/
├── docs/
└── README.md
```

Drivers are implemented once inside `modules/` and reused by every subsequent
project, resulting in a progressively growing embedded software stack instead
of duplicated code.

---

# Learning Path

| Project | Embedded Concept | Main Topics |
|----------|------------------|-------------|
| [`00_Introduction`](00_Introduction) | Development Environment | CMSIS, HAL, debugger, register inspection |
| [`01_ESD`](01_ESD) | GPIO Output | Multiplexed 7-segment display |
| [`02_Joystick`](02_Joystick) | GPIO Input | Digital inputs, pull-ups, debouncing |
| [`03_LCD`](03_LCD) | SPI | TFT display driver and graphics |
| [`04_Potis`](04_Potis) | ADC | Polling-mode analog conversion |
| [`05_Potis_DMA`](05_Potis_DMA) | DMA | Continuous ADC acquisition |
| [`06_Blinky_Dot`](06_Blinky_Dot) | Timers | Hardware PWM generation |
| [`07_Dimming_Dot`](07_Dimming_Dot) | PWM | Duty-cycle brightness control |
| [`08_Stopwatch`](08_Stopwatch) | Interrupts | EXTI, NVIC, high-resolution timing |
| [`P1_Fan_Control`](P1_Fan_Control) | Control Engineering | PI controller, PWM, tachometer feedback |
| [`P2_Weatherstation`](P2_Weatherstation) | Embedded Networking | BME280, I²C, CAN bus |

---

# Reusable Modules

Every hardware abstraction is implemented as an independent module with its own
documentation.

| Module | Purpose |
|----------|---------|
| [`esd`](modules/esd) | Multiplexed 7-segment display |
| [`joystick`](modules/joystick) | Five-direction joystick |
| [`lcd`](modules/lcd) | ILI9341 graphics driver |
| [`my_lcd`](modules/my_lcd) | High-level LCD widgets |
| [`potis`](modules/potis) | ADC polling |
| [`potis_dma`](modules/potis_dma) | DMA-based ADC acquisition |
| [`dot`](modules/dot) | PWM blinking, dimming, stopwatch |
| [`utils`](modules/utils) | Timer and GPIO helpers |
| [`median`](modules/median) | Median filter |
| [`P1_Fan`](modules/P1_Fan) | Fan controller |
| [`env_sensor`](modules/env_sensor) | BME280 + CAN communication |

Each module contains its own `README.md` describing

- hardware connections
- public API
- implementation details
- engineering decisions

---

# Final Projects

## P1 – Fan Control

A closed-loop PI speed controller for a PWM-controlled DC fan.

Features

- PWM motor control
- EXTI-based tachometer measurement
- Median-filtered RPM estimation
- Fixed-period discrete PI controller
- LCD visualization
- MATLAB / Octave assisted controller tuning

---

## P2 – Weather Station

A distributed CAN-based environmental monitoring node.

Features

- Bosch BME280 sensor
- I²C communication
- CAN publish/subscribe architecture
- Multi-node communication
- Timer-driven measurements
- LCD dashboard
- Joystick node selection

---

# Hardware

| Component | Purpose |
|-----------|---------|
| STM32F429 | Main microcontroller |
| Waveshare Open429Z-D | Development board |
| ILI9341 TFT LCD | User interface |
| 7-segment display | Early GPIO exercises |
| Dual potentiometers | ADC input |
| 5-way joystick | User interaction |
| Bosch BME280 | Environmental sensing |
| CAN transceiver | Multi-node communication |
| PWM fan with tachometer | Closed-loop control |

---

![Uploading image.png…]()



# Embedded Topics Covered

This repository demonstrates practical implementations of

- GPIO
- ADC
- DMA
- Timers
- PWM
- Output Compare
- One-Pulse Mode
- EXTI
- NVIC
- SPI
- I²C
- CAN
- Interrupt-driven firmware
- Real-time scheduling
- Digital filtering
- PI control
- Embedded graphics
- Hardware abstraction
- Modular firmware architecture

---

# Documentation

The `docs/` directory complements the source code with engineering notes on the
underlying peripherals.

- Clock tree and timer calculations
- ADC and DMA
- GPIO interrupts and NVIC
- I²C protocol
- CAN bus architecture

The goal is not only to explain *how* the drivers work, but also *why* they
were implemented that way.

---

# Building

Each folder is an independent STM32CubeIDE project.

1. Import the desired project into STM32CubeIDE.
2. Import the shared `modules` directory.
3. Build and flash to an STM32F429 board.

---

# License

MIT - see [LICENSE](LICENSE).
