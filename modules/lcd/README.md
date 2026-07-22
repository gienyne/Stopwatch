# lcd

Wrapper around the ILI9341 graphics library provided as part of the laboratory
framework.

The module exposes a simplified and consistent interface for initializing the
display and drawing common graphical primitives, allowing applications to use
the LCD without interacting directly with the underlying ILI9341 library.

## Responsibilities

* Initialize the TFT display.
* Draw text at predefined lines or arbitrary coordinates.
* Draw basic graphical primitives.
* Provide a simplified interface to the underlying graphics library.

## Public API

```c
void lcd_init(void);

void lcd_fill_screen(uint16_t color);

void lcd_draw_text_at_line(...);
void lcd_draw_text_at_coord(...);

void lcd_draw_pixel(...);
void lcd_draw_rect(...);
void lcd_draw_circle(...);
void lcd_draw_horizontal_line(...);
void lcd_draw_vertical_line(...);
```

## Notes

This module wraps the ILI9341 graphics library supplied with the laboratory
framework and is reused throughout the repository as the common display
interface.

Higher-level visualization components developed for this repository are
implemented in the [`my_lcd`](../my_lcd) module.
