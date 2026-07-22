# Clock Tree & Timers

Timers are among the most versatile peripherals available on STM32
microcontrollers.

Almost every real-time feature implemented in this repository relies on one or
more hardware timers:

- PWM generation for the fan controller
- LED blinking and dimming
- Stopwatch time base
- One-Pulse Mode
- RPM measurement
- Periodic control loops

Understanding how timers obtain their clock is therefore essential.

---

# STM32F429 Clock Tree

The STM32F429 used in this repository runs at a maximum system frequency of
**168 MHz**.

The system clock is generated from the external 8 MHz crystal using the main
PLL.

```text
                 8 MHz HSE
                     │
                ÷ PLLM = 8
                     │
                1 MHz VCO input
                     │
               × PLLN = 336
                     │
               336 MHz VCO
                     │
                ÷ PLLP = 2
                     │
              168 MHz SYSCLK
                     │
      ┌──────────────┴──────────────┐
      │                             │
      ▼                             ▼
 AHB Prescaler                  APB Prescalers
      │                             │
 168 MHz HCLK           APB1 = 42 MHz
                        APB2 = 84 MHz
```

The clock tree distributes different clock frequencies to the various buses and
peripherals inside the microcontroller.

---

# Timer Clock Doubling

One STM32 feature often surprises beginners.

Whenever an APB prescaler is greater than 1, the timers connected to that bus
automatically receive **twice** the APB clock frequency.

For this project:

| Bus | Bus Clock | Timer Clock |
|------|----------:|------------:|
| APB1 | 42 MHz | 84 MHz |
| APB2 | 84 MHz | 168 MHz |

This means two timers configured with identical prescalers may run at different
speeds simply because they are connected to different buses.

Understanding this behavior avoids many timer configuration mistakes.

---

# Timer Frequency Calculation

Every timer first divides its input clock using the prescaler.

```
Timer Tick Frequency

timer_clock
──────────────
Prescaler + 1
```

Example:

```
Timer Clock = 84 MHz
Desired Tick = 1 kHz

Prescaler =
84 000 000 / 1 000 − 1

Prescaler = 83 999
```

The timer now increments exactly once every millisecond.

---

# Auto-Reload Register (ARR)

The timer counts upward until it reaches the Auto-Reload Register (ARR).

```text
0 -> 1 -> 2 -> ... -> ARR
                  │
                  ▼
             Update Event
```

The resulting overflow frequency becomes

```
Overflow Frequency

Timer Tick
────────────
ARR + 1
```

Both Prescaler and ARR therefore determine the final timing behavior.

---

# Example: Two-Minute Timer

Suppose a timer should overflow every two minutes.

```
120 seconds

↓

120 000 timer ticks
(at 1 kHz)
```

Since

```
120 000 > 65 535
```

a standard 16-bit timer cannot represent this value directly.

Possible solutions are

- use a 32-bit timer
- reduce the timer tick frequency
- count multiple shorter overflows in software

Several projects inside this repository use the third approach.

---

# PWM Generation

PWM (Pulse Width Modulation) is generated entirely by hardware.

Instead of toggling a GPIO in software, the timer automatically produces a
periodic waveform.

```text
100 %

████████████████████

50 %

████████      ██████

20 %

████      ████
```

Changing the Compare Register (CCR) modifies the duty cycle without affecting
the signal frequency.

This repository uses PWM for

- LED brightness control
- Fan speed control

---

# Output Compare

Output Compare compares the current timer counter with a predefined compare
value.

Whenever both values become equal, the timer automatically performs an action.

Examples include

- toggle an output pin
- generate an interrupt
- trigger another peripheral

Unlike PWM, Output Compare is often used for precisely timed events rather than
continuous waveforms.

The LED blinking exercises make extensive use of Output Compare mode.

---

# One-Pulse Mode

One-Pulse Mode allows a timer to execute exactly one timing cycle before
stopping automatically.

```text
Start

│

Timer counts

│

Overflow

│

Timer stops
```

This behavior is ideal for

- staircase lighting
- automatic timeouts
- delayed switching

The staircase-light exercise uses this feature so the application never has to
monitor elapsed time in software.

---

# Timers Used Throughout This Repository

| Project | Timer Usage |
|----------|-------------|
| Blinky Dot | Output Compare LED blinking |
| Dimming Dot | PWM brightness control |
| Stopwatch | High-resolution time base |
| Fan Controller | PWM generation, RPM measurement and PI update timer |
| Weather Station | Periodic sensor acquisition |

Each exercise builds on the previous one, progressively introducing additional
timer capabilities.

---

# Design Takeaways

Several important embedded-system design principles emerge from these exercises.

- Let hardware perform repetitive work whenever possible.
- Configure timers once instead of polling inside software loops.
- Separate timing generation from application logic.
- Use interrupts only when software intervention is required.

Offloading periodic work to dedicated peripherals significantly reduces CPU load
and leads to more deterministic real-time behavior.
