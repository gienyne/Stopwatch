# 01_ESD — GPIO output at scale: 7-segment display

Scales GPIO output from two LEDs ([`00_Introduction`](../00_Introduction)) to 13 coordinated pins driving a multiplexed 4-digit, 7-segment display, via the [`esd`](../modules/esd) module.

## What this covers

- Mapping and documenting a fixed GPIO-pin-to-hardware-signal assignment (segments A–F, segment G/dot, and 4 digit-select lines) directly from the board's schematic, so later code never has to re-derive it.
- Structuring GPIO logic into a proper module (`esd.h`/`esd.c`) instead of inline `main.c` code — the pattern every later exercise's driver follows.
- Implementing `esd_init` (13-pin push-pull output configuration) and `esd_show_digit` (writing a precomputed segment bitmask + asserting one digit-select line).
- A 4-digit countdown application: cycling all four digit positions fast enough for persistence of vision to read as "four digits at once," then a second pass counting down on all digits simultaneously.
- Software timing: implementing a millisecond delay from first principles (see [`utils`](../modules/utils)) before a proper timer-based version exists ([`06_Blinky_Dot`](../06_Blinky_Dot) onward).

See [`esd`](../modules/esd)'s README for the concrete pin mapping and API.
