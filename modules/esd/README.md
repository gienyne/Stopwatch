# esd

GPIO driver for a multiplexed four-digit seven-segment display.

The module encapsulates all GPIO configuration and display control, providing a
simple API for displaying decimal digits without exposing the underlying pin
mapping to the application.

## Responsibilities

The driver is responsible for:

- Configuring all display GPIO pins.
- Selecting the active digit position.
- Driving the corresponding segment lines.
- Abstracting the hardware through a simple public API.

## Hardware

The display is connected through thirteen GPIO lines.

| Signal | Pins |
|---------|------|
| Segments A–F | PD7, PD4, PD5, PD6, PD12, PD11 |
| Segment G | PE12 |
| Decimal Point | PE11 |
| Digit Select (1–4) | PD14, PD15, PD0, PD1 |

Because Segment G is connected to a different GPIO port than the remaining
segments, it is written separately when required.

## Public API

```c
void esd_init(void);
void esd_show_digit(esd_digit_t digit, esd_position_t position);
```

### `esd_init()`

Initializes all GPIO pins required by the display as push-pull outputs.

### `esd_show_digit()`

Displays a decimal digit at the selected position by updating the segment lines
and enabling the corresponding digit.

The driver exposes only the primitive display operation. Display multiplexing
and animation remain the responsibility of the application.

## Design Notes

The module intentionally hides the complete GPIO layout from the application.

As a result, applications only manipulate digits and display positions, while
all hardware-specific details remain centralized inside the driver. This design
pattern is reused by every subsequent module in the repository.
