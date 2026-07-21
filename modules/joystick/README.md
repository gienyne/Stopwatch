# joystick

GPIO driver for the five-way digital joystick available on the STM32F429
Discovery board.

The module configures all joystick pins as digital inputs while exposing the pin
definitions required by the application to detect user actions.

## Responsibilities

The driver is responsible for:

- Configuring the joystick GPIO pins.
- Enabling the required GPIO peripheral clock.
- Providing symbolic pin definitions for each direction.

Reading the joystick state and implementing the application logic remain the
responsibility of the caller.

## Hardware

| Direction | Pin |
|----------|-----|
| Up | PG11 |
| Down | PG6 |
| Left | PG9 |
| Right | PG10 |
| Press | PG12 |

The joystick is wired as an active-low device using the internal pull-up
resistors of the STM32.

## Public API

```c
void joystick_init(void);
```

### `joystick_init()`

Initializes all joystick pins as digital inputs with internal pull-up
resistors enabled.

## Design Notes

The driver intentionally performs only hardware initialization.
