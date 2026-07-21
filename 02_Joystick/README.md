# 02_Joystick — GPIO digital input

Introduces digital *input* (as opposed to output) via a 5-way joystick, and combines it with [`esd`](../modules/esd) from the previous exercise into an interactive digit editor.

## What this covers

- Digital input configuration: `GPIO_MODE_INPUT` with pull-ups, and reading a joystick's schematic to determine wiring per direction.
- Pull-up vs. pull-down choice, derived from the joystick's actual switch-to-ground wiring rather than assumed.
- Reading multiple GPIO pins on the same port in a single register access (see [`utils_gpio_port_read`](../modules/utils)) instead of one `HAL_GPIO_ReadPin` call per direction.
- **Application**: a 4-digit editor built on [`esd`](../modules/esd) —
  - Up/Down increments/decrements the current digit (wrapping 9→0 and 0→9)
  - Left/Right moves which digit position is being edited (wrapping across all 4 positions)
  - Press starts a 1-second-tick countdown on the currently selected digit, from its current value down to 0, then restores it
- **Switch bounce**: identifying the mechanical contact-bounce artifact on the joystick's press action, and the debouncing strategies used to filter it (software delay after detecting a transition, or requiring the signal to be stable across several consecutive reads).

See [`joystick`](../modules/joystick)'s README for the pin mapping and API.
