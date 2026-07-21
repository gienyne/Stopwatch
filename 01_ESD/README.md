# GPIO Output – Seven-Segment Display

This exercise extends the GPIO fundamentals introduced in
[`00_Introduction`](../00_Introduction) by driving a four-digit seven-segment
display instead of two individual LEDs.

To avoid placing all GPIO logic inside `main.c`, the display control is moved
into the reusable [`esd`](../modules/esd) driver. The application is therefore
reduced to initializing the driver and updating the displayed digits.

## Objectives

- Control a multiplexed four-digit seven-segment display.
- Separate hardware-specific code from the application.
- Introduce the first reusable driver of the repository.
- Build more complex GPIO applications on top of the STM32 HAL.

## Implementation

The application performs the following tasks:

- Initializes the `esd` driver.
- Displays the digits `9 -> 0` on each display position.
- Demonstrates both individual digit selection and simultaneous activation of
  all four positions.
- Uses a software delay between updates to make the countdown visible.

Unlike the previous exercise, the application no longer manipulates GPIO pins
directly. All hardware access is delegated to the `esd` module.

## Repository progression

This exercise introduces the modular architecture used throughout the
repository.

From this point onward, reusable peripheral drivers are placed inside the
[`modules`](../modules) directory and shared by multiple projects instead of
duplicating hardware-specific code inside each application.
