# Clock Tree & Timers

## System clock configuration

This project runs the STM32F429 at 168 MHz SYSCLK, derived from an 8 MHz HSE crystal through the main PLL:

```
HSE (8 MHz)
  │  ÷ PLLM (8)         → VCO input  = 1 MHz     (must land in 1–2 MHz)
  ▼
  ×  PLLN (336)         → VCO output = 336 MHz    (must land in 100–432 MHz)
  ▼
  ÷  PLLP (2)            → SYSCLK     = 168 MHz
  ▼
AHB  (÷1)  → 168 MHz
  ├─ APB1 (÷4) → 42 MHz  → APB1 timers run at ×2 = 84 MHz
  └─ APB2 (÷2) → 84 MHz  → APB2 timers run at ×2 = 168 MHz
```

The "timer clock = ×2 APB clock" rule applies whenever the APB prescaler is not 1 — it's a quirk of the STM32F4 clock tree that gives timers on a divided APB bus back their full input frequency. It explains an easy trap: two timers configured identically but sitting on different APB buses (e.g. TIM1 on APB2 vs. TIM2 on APB1) will *not* overflow at the same wall-clock rate, since TIM1 effectively ticks at 168 MHz and TIM2 at 84 MHz even though the "APB clock" driving them differs by only a factor of 2, not the 2× that ends up mattering after the timer doubling rule.

## Prescaler / period arithmetic

Timer output frequency:

```
timer_freq = timer_input_clock / (Prescaler + 1)
```

**Example — 1 kHz tick from a 50 MHz clock:**

```
1000 = 50 000 000 / (Prescaler + 1)
Prescaler + 1 = 50 000
Prescaler = 49 999
```

**Example — overflow every 2 minutes at a 1 kHz tick:**

```
120 s × 1000 ticks/s = 120 000 ticks
Period = 120 000 − 1 = 119 999
```

`119 999` exceeds the 16-bit auto-reload register's range (max `65 535` = 2¹⁶ − 1), so this specific period is only achievable on a 32-bit timer. On a 16-bit-only timer, the same 2-minute interval can still be reached by chaining: configure a 1-second overflow and count overflows in the update-interrupt handler until 120 have elapsed.

## One-Pulse Mode

For a "stay on for N seconds, then switch off automatically" behavior (used in [`dot`](../drivers/dot)'s staircase-light timer), One-Pulse Mode is a better fit than a free-running timer with software bookkeeping: the timer starts once, counts to its configured period, and stops itself — no explicit stop call or elapsed-time tracking needed in the application.

## Output Compare / PWM for LED blink & dimming

Both the blink and dimming behaviors on the "dot" segment ([`dot`](../drivers/dot)) are implemented with a timer Output-Compare channel rather than a software toggle loop, so the on/off (or duty-cycle) switching happens entirely in hardware and the CPU is free to do other work between updates. Blink frequency and dim duty cycle are each driven from a potentiometer reading via [`potis`](../drivers/potis).
