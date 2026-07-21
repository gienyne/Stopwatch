# GPIO Input – Joystick

This exercise introduces digital inputs by integrating a five-way joystick with
the reusable drivers developed in the previous exercise.

The joystick is used to interactively control the four-digit seven-segment
display, transforming the application from a fixed countdown into a simple
user-controlled interface.

## Objectives

- Configure GPIO pins as digital inputs.
- Read the state of multiple input pins.
- Build an interactive application using reusable drivers.
- Introduce basic user input handling.

## Implementation

The application combines the `joystick` and `esd` modules to create a simple
digit editor.

The joystick controls the display as follows:

- **Up** increments the current digit.
- **Down** decrements the current digit.
- **Left** selects the previous display position.
- **Right** selects the next display position.
- **Press** starts a one-second countdown from the current digit to zero.

To reduce unnecessary GPIO accesses, the complete joystick port is read once and
individual directions are extracted using bit masks.

A short software delay is also used to reduce the effect of mechanical switch
bounce.

## Repository progression

This exercise introduces the first user input device of the repository.

The reusable `joystick` driver developed here is later reused by the
Weatherstation project to navigate the graphical user interface on the LCD.
