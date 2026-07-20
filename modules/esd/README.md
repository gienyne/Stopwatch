# esd — 4-digit 7-segment display driver

Drives a multiplexed 4-digit, 7-segment (+ dot) display over 13 GPIO lines: 6 segment lines on one port, the dot/segment-G on another, and 4 digit-select ("control") lines.

## Hardware mapping

| Signal | Pin |
|---|---|
| SEG A–F | PD7, PD4, PD5, PD6, PD12, PD11 |
| SEG G / DOT | PE12 / PE11 |
| Digit select CNTL1–4 | PD14, PD15, PD0, PD1 |

Segment-G lives on a different GPIO port than the other six segments, so it's OR'd into the output separately inside `esd_show_digit`.

## API

```c
void esd_init(void);
void esd_show_digit(esd_digit_t digit, esd_position_t pos);
```

`esd_init` configures the 13 pins as push-pull outputs. `esd_show_digit` writes the segment pattern for `digit` (0–9) to the display and asserts the given `pos` (`ESD_POSITION_1..4` or `ESD_POSITION_ALL`). Segment patterns are precomputed bitmasks (`DIGIT_0` … `DIGIT_9`) rather than a lookup table, to keep the write a single register store.

## Notes

Since only one digit position is enabled at a time, showing more than one digit "at once" (e.g. a 4-digit countdown) requires the caller to cycle through positions fast enough for persistence of vision — this module exposes the primitive, the multiplexing loop lives in the application.
