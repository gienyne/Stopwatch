# my_lcd — bar graphs and diagnostic shapes

Small drawing helpers on top of [`lcd`](../lcd), factored out so application code (fan control, weather station) doesn't reimplement bar-graph math per project.

## API

```c
void my_lcd_draw_baargraph(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                            uint16_t value, uint16_t Colour, uint16_t bg_colour);
void draw_diag_cross(uint16_t x, uint16_t y, uint16_t color);
```

`my_lcd_draw_baargraph` fills a `width × height` rectangle anchored at its bottom-left corner (`x`, `y`), proportionally to `value` expressed in **per-mille** (0–1000) — not percent — so callers get one extra digit of resolution without switching to floating point. The filled portion uses `Colour`, the remainder `bg_colour`, drawn via `ILI9341_Draw_Filled_Rectangle_Coord` underneath.

Used throughout both projects (fan-control and weather-station) to visualize setpoint-vs-actual (fan RPM) and live sensor readings (temperature/pressure/humidity) side by side with the numeric readout.
