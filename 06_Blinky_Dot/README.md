# 06_Blinky_Dot — timer Output Compare / PWM

First use of hardware timers for a task previously done (in `01_ESD`'s countdown) with a software delay loop: blinking the display's dot segment (PE-11) at 50% duty cycle without touching the CPU, via a timer Output-Compare channel in [`dot`](../modules/dot).

## What this covers

- Timer TimeBase configuration: setting the prescaler for a target counting frequency and confirming the overflow period experimentally by printing `__HAL_TIM_GET_COUNTER(&htim)` to the LCD in a `while(1)` loop.
- Comparing Timer 1 (APB2) against Timer 2 (APB1) under an identical nominal configuration, and reasoning about why their actual overflow timing differs — the "timer clock = ×2 APB clock when the APB prescaler ≠ 1" rule (see [`docs/clock-tree-and-timers.md`](../docs/clock-tree-and-timers.md)).
- Output-Compare-based PWM generation: the dot segment blinks entirely in hardware once configured, freeing the CPU for other work.
- Mapping a potentiometer reading (via [`potis`](../modules/potis)) to a blink frequency continuously adjustable between 1 Hz and 10 Hz — and why "continuous" is only true down to the resolution the timer period allows (mHz being the more honest unit than Hz).

See [`dot`](../modules/dot) for the API.
