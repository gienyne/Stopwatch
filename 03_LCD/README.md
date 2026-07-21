# SPI Display

This exercise introduces a graphical TFT display based on the ILI9341
controller, replacing the seven-segment display used in the previous exercises.

Using the provided `lcd` wrapper together with the custom `my_lcd` module, the
application introduces text rendering, simple graphics and reusable
visualization components.

## Objectives

- Display text on a graphical LCD.
- Format numerical values for visualization.
- Draw basic graphical elements.
- Extend the display API with reusable visualization components.

## Implementation

The application demonstrates several display features:

- Displays a countdown from 10 to 1.
- Formats numeric values before rendering them on the screen.
- Draws a horizontal bar graph representing the current countdown value.
- Draws a simple pixel-based cross using the pixel drawing primitive.

The reusable bar graph is implemented in the `my_lcd` module, allowing later
projects to visualize values without duplicating drawing code.

## Repository progression

From this exercise onward, the TFT display becomes the primary user interface
used throughout the repository.

The same display infrastructure is later reused by the Fan Control and
Weatherstation projects to visualize control variables, sensor measurements and
system status.
