# my_lcd

Utility module providing reusable visualization components built on top of the
common `lcd` interface.

Rather than repeatedly implementing application-specific drawing logic, this
module groups higher-level graphical helpers that can be shared across multiple
projects.

## Responsibilities

The module provides reusable drawing utilities for:

- Horizontal bar graphs.
- Simple diagnostic graphics.

## Public API

```c
void my_lcd_draw_baargraph(
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    uint16_t value,
    uint16_t color,
    uint16_t bg_color);

void draw_diag_cross(
    uint16_t x,
    uint16_t y,
    uint16_t color);
```

### `my_lcd_draw_baargraph()`

Draws a horizontal bar graph whose fill level is specified in per mille
(`0...1000`). Values outside this range are automatically clamped.

Using per-mille values instead of percentages provides finer resolution while
avoiding floating-point arithmetic, making the function suitable for
resource-constrained embedded systems.

### `draw_diag_cross()`

Draws a simple diagnostic cross using the pixel drawing primitive provided by
the `lcd` module.

## Design Notes

This module represents the first custom visualization layer of the repository.

Instead of exposing only generic drawing primitives, it introduces reusable UI
components that can be integrated directly into applications. The same bar graph
function is later reused by the Fan Control and Weatherstation projects to
visualize control variables and sensor measurements.
