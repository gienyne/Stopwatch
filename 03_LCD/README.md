# SPI Display

This exercise replaces the seven-segment display used in the previous exercises
with a graphical TFT display based on the ILI9341 controller.

Using the provided LCD driver together with the custom `my_lcd` module, the
application introduces text rendering, simple graphics and reusable visualization
components.

## Objectives

- Display text on a graphical LCD.
- Format numerical values for visualization.
- Draw basic graphical elements.
- Build reusable visualization functions on top of the LCD driver.

## Implementation

The application demonstrates several display features:

- Displays a countdown from 10 to 1.
- Formats numeric values before rendering them on the screen.
- Draws a horizontal bar graph representing the current countdown value.
- Draws a simple pixel-based cross using the lowest-level drawing primitive.

The graphical bar graph is implemented inside the reusable `my_lcd` module,
allowing later projects to visualize values without duplicating drawing code.

## Repository progression

From this exercise onward, the TFT display becomes the primary user interface for
the repository.

Later projects reuse the same display infrastructure to present sensor data,
control variables and graphical dashboards.
