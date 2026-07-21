# 03_LCD — SPI TFT display

First use of the ILI9341 TFT display, via the [`lcd`](../modules/lcd) and [`my_lcd`](../modules/my_lcd) modules — the display used by every project from here on to replace the 7-segment/serial-only feedback used so far.

## What this covers

- SPI-based display initialization and the text-drawing API (`lcd_draw_text_at_line` vs. `lcd_draw_text_at_coord` — line-indexed vs. pixel-coordinate placement).
- Formatting numeric values for display with `sprintf`/`snprintf` into a fixed-size stack buffer, sized deliberately small given a microcontroller's limited RAM/stack.
- A 10→1 second countdown rendered as text on the LCD.
- Implementing `my_lcd_draw_baargraph` on top of `ILI9341_Draw_Filled_Rectangle_Coord` — a horizontal bar graph parameterized by position, size, a per-mille fill value, and two colors (fill/background), reused by every later project ([`P1_Fan_Control`](../P1_Fan_Control), [`P2_Weatherstation`](../P2_Weatherstation)) to visualize setpoints and sensor readings.
- Combining the countdown with a live bar-graph readout of the same value.
- Direct pixel drawing (`lcd_draw_pixel`) to render a 100×100 px cross — the lowest-level primitive the higher-level shape functions build on.

See [`lcd`](../modules/lcd) and [`my_lcd`](../modules/my_lcd) for the full API.
