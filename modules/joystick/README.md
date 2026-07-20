# joystick — 5-way digital joystick input

Reads a 5-way joystick (up/down/left/right/press) wired to GPIO port G, active-low with internal pull-ups.

## Hardware mapping

| Direction | Pin |
|---|---|
| Up | PG11 |
| Down | PG6 |
| Left | PG9 |
| Right | PG10 |
| Press | PG12 |

## API

```c
void joystick_init(void);
```

Configures the five pins as digital inputs with pull-ups enabled, so each line reads `1` when idle and `0` when pressed. Reading is left to the caller via [`utils_gpio_port_read`](../utils) — this module only owns pin configuration and the bitmask defines (`JOY_PIN_UP`, `JOY_PIN_DOWN`, …, including diagonal combinations like `JOY_PIN_UP_LEFT`), so a caller can mask `JOY_GPIO_PORT` against `JOY_ALL_DIRECTION` in one read instead of five separate `HAL_GPIO_ReadPin` calls.

## Debounce

This 5-way switch exhibits the usual mechanical contact bounce; debouncing (software delay/edge filtering) is handled at the call site, not inside this module.
