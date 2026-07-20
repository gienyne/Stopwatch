# dot — timer-driven LED blink/dim, stopwatch, and staircase-light timer

Three timer-based use cases for the display's "dot" segment (PE11) and the on-board LED, all offloaded to hardware timers so none of them consume CPU cycles in a busy-wait.

## 1. Blink (Output Compare / PWM)

```c
void blink_dot_timer_init(void);
void blink_dot_oc_init(void);
void set_blink_Dot_freq(int poti_num, uint32_t mv);
int  get_freq(void);
```

Blinks the dot segment at a 50% duty cycle using a timer Output-Compare channel, so the on/off toggling happens in hardware. `set_blink_Dot_freq` maps a potentiometer reading (via [`potis`](../potis)) to a blink frequency between 1 Hz and 10 Hz — continuous in appearance, though true continuity isn't possible with a fixed timer resolution (mHz would be the more honest unit).

## 2. Dimming (PWM duty cycle)

```c
void dimming_dot_timer_init(void);
void dimming_dot_oc_init(void);
void dimming_intensitat_Dot(int poti_num);
```

Same Output-Compare mechanism, but instead of varying the on/off frequency, the duty cycle is varied at a frequency high enough (>25 Hz) that the eye perceives an averaged brightness rather than a flicker — the classic PWM-dimming trick.

## 3. Staircase-light timer (One-Pulse Mode)

```c
void treppenhaus_init(void);
```

Turns the LED on when the joystick button (or reset) is pressed and keeps it on for 59 s before switching off automatically, via a timer in **One-Pulse Mode**: the timer starts once, counts to the configured period, and stops itself, no manual timer stop is needed once triggered.

## 4. Stopwatch (1/10000 s resolution)

```c
void stopwatch_timer_init(void);
void stopwatch_gpio_init(void);
void zeit_anzeigen_at(uint32_t total_centiemes, uint16_t x, uint16_t y);
void dessine_boutons(void);
void gesamtdauer_anzeigen(void);
void taste_verarbeitung(void);
```

A lap-timer, simulated via the on-board user button standing in for a laser gate: first press starts the overall clock, each subsequent press records a lap under the running total. Timer resolution is set for 1/10 000 s granularity to keep close laps distinguishable, with the split/lap logic and LCD layout modeled after a typical stopwatch UI.

## Shared state

`dot.h` exposes the stopwatch's live state as `extern volatile` globals (`zeitstand`, `chrono_laeuft`, `runden_zeiten[]`, …) so the timer ISR and the application's LCD-refresh loop can share them without a getter per field — appropriate here given the tight, single-producer/single-consumer relationship between the ISR and the display loop.
