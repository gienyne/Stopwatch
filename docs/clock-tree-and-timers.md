# Clock Tree & Timers

This document explains the clock configuration and timer concepts used
throughout this repository. Most exercises and projects rely on STM32 timers,
whether for delays, PWM generation, periodic sampling, stopwatch operation, or
closed-loop motor control.

---

# STM32F429 Clock Tree

The STM32F429 is configured to run at **168 MHz** using the external **8 MHz
HSE crystal** and the internal PLL.

```text
          8 MHz HSE
               │
          PLLM = 8
               ▼
         1 MHz VCO input
               │
         PLLN = 336
               ▼
        336 MHz VCO output
               │
          PLLP = 2
               ▼
        SYSCLK = 168 MHz
               │
     ┌─────────┴─────────┐
     ▼                   ▼
AHB = 168 MHz      APB1 = 42 MHz
                        │
                 Timer clock = 84 MHz

                   APB2 = 84 MHz
                        │
                 Timer clock = 168 MHz
```

One STM32 peculiarity is that timers connected to an APB bus automatically
receive **twice the APB clock** whenever the APB prescaler is greater than one.

Therefore:

| Bus | Bus Clock | Timer Clock |
|------|----------:|------------:|
| APB1 | 42 MHz | 84 MHz |
| APB2 | 84 MHz | 168 MHz |

Ignoring this rule is one of the most common causes of incorrect timer
calculations.

---

# Timer Frequency Calculation

A timer first divides its input clock using the **prescaler**, then counts up
until it reaches the **auto-reload register (ARR)**.

The timer tick frequency is

```text
Timer Tick =
Timer Clock / (Prescaler + 1)
```

The update event frequency becomes

```text
Update Frequency =
Timer Tick / (ARR + 1)
```

---

# Example: 1 kHz Timer

Assume a timer clock of **84 MHz**.

To obtain a timer tick of **1 kHz**:

```text
Prescaler + 1 = 84 000
Prescaler = 83 999
```

The timer now increments once every millisecond.

---

# Example: 25 kHz PWM

The fan controller generates its PWM signal at **25 kHz**.

Instead of directly dividing 168 MHz to 25 kHz, the implementation first creates
a timer clock of **2.5 MHz**:

```text
168 MHz
   │
Prescaler
   ▼
2.5 MHz timer tick
```

Using

```text
ARR = 99
```

produces

```text
2.5 MHz / 100 = 25 kHz
```

Keeping the ARR at exactly **100 counts** also means the compare register maps
directly to duty cycle percentages.

| Compare Value | Duty Cycle |
|--------------:|-----------:|
| 0 | 0 % |
| 25 | 25 % |
| 50 | 50 % |
| 75 | 75 % |
| 100 | 100 % |

This greatly simplifies the PI controller implementation.

---

# PWM (Pulse Width Modulation)

PWM rapidly switches a digital output between HIGH and LOW while varying the
percentage of time the signal remains HIGH.

```text
100 %

████████████████████

75 %

██████████████______

50 %

████████________

25 %

████______
```

The average voltage seen by the load depends on the duty cycle.

PWM therefore allows a digital pin to control

- LED brightness
- motor speed
- fan speed
- power delivered to a load

without generating an analog voltage.

---

# Output Compare

Output Compare uses the timer to trigger an action whenever the counter reaches
a predefined compare value.

Unlike PWM, Output Compare does not necessarily generate a periodic waveform.

In this repository it is used for

- LED blinking
- staircase-light timing
- stopwatch timing

without software polling loops.

---

# One-Pulse Mode

One-Pulse Mode automatically stops a timer after a single timing interval.

```text
Start
  │
  ▼
Timer counts
  │
  ▼
ARR reached
  │
  ▼
Timer stops automatically
```

This is ideal for applications such as a staircase-light timer, where the LED
should remain ON for a fixed duration before switching OFF automatically.

---

# 16-bit vs 32-bit Timers

Most STM32 timers are **16-bit**, meaning they can count up to

```text
65 535
```

before overflowing.

Long timing intervals therefore require either

- a larger prescaler,
- overflow counting in software,
- or a 32-bit timer such as TIM2 or TIM5.

The custom delay implementation in the `utils` module uses **TIM2**, allowing
long delays without overflow handling.

---

# Where Timers Are Used

Timers appear throughout this repository.

| Project | Purpose |
|---------|---------|
| Blinky Dot | Hardware LED blinking |
| Dimming Dot | PWM brightness control |
| Stopwatch | High-resolution time measurement |
| Fan Controller | PWM generation, tachometer timing, PI control period |
| Weather Station | Periodic 1 Hz sensor sampling |

---

# Key Takeaways

- Understand the STM32 clock tree before configuring timers.
- Remember that timer clocks are often **twice the APB clock**.
- Prescaler controls timer resolution.
- ARR controls the timer period.
- PWM controls average output power through duty cycle.
- Output Compare generates hardware events without CPU polling.
- One-Pulse Mode is useful for one-shot timing applications.
- Timer peripherals are the foundation of most real-time embedded systems.
