# dot

Timer-based utilities built around the DOT segment of the seven-segment display.

Originally created to blink the DOT segment using hardware PWM, the module was
progressively extended during later timer exercises to support brightness
control, a staircase-light timer and a stopwatch implementation.

## Responsibilities

- Configure hardware timers.
- Generate PWM signals.
- Control the DOT segment.
- Provide timer-based application utilities.
- Implement the stopwatch timing logic.

## Features

### Blink

Generates a 50% duty-cycle PWM signal whose frequency can be adjusted from
approximately 1 Hz to 10 Hz using a potentiometer.

### Dimming

Uses PWM duty-cycle modulation to control the perceived brightness of the DOT
segment.

### Staircase timer

Uses Timer 2 in One-Pulse mode to keep the DOT segment active for a fixed time
after a button press.

### Stopwatch

Implements a stopwatch based on timer interrupts and external interrupts,
including lap-time recording and LCD visualization.

## Public API

```c
void blink_dot_timer_init(void);
void blink_dot_oc_init(void);
void set_blink_Dot_freq(...);

void dimming_dot_timer_init(void);
void dimming_dot_oc_init(void);
void dimming_intensitat_Dot(...);

void treppenhaus_init(void);

void stopwatch_timer_init(void);
void stopwatch_gpio_init(void);

void taste_verarbeitung(void);
void gesamtdauer_anzeigen(void);
void zeit_anzeigen_at(...);
void dessine_boutons(void);
