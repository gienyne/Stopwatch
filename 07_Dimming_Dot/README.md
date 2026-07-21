# PWM Brightness Control

This exercise extends the timer-based PWM implementation introduced in the
previous exercise by controlling the duty cycle instead of the output frequency.

Rather than producing a visible blinking effect, the PWM signal operates at a
high enough frequency for the human eye to perceive only the average brightness
of the DOT segment.

## Objectives

- Understand Pulse Width Modulation (PWM).
- Control LED brightness by varying the duty cycle.
- Map an analog input to a PWM output.

## Implementation

The application demonstrates:

- PWM generation using Timer 1.
- Fixed PWM frequency with a variable duty cycle.
- Brightness control using one of the potentiometers.
- Continuous duty-cycle updates based on ADC measurements.

## Repository progression

This exercise builds directly on the timer configuration introduced in the
previous project and demonstrates another common application of PWM beyond
simple blinking.
