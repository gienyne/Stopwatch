# Timer Output Compare (PWM)

This exercise introduces the STM32 hardware timers and demonstrates how a
repetitive task can be delegated entirely to dedicated timer peripherals instead
of being executed by the CPU.

Using Timer 1 in Output Compare / PWM mode, the application controls the DOT
segment of the seven-segment display with a configurable blinking frequency.

## Objectives

- Configure a timer time base.
- Generate a PWM signal using Output Compare.
- Control the DOT segment entirely in hardware.
- Adjust the blinking frequency using a potentiometer.

## Implementation

The application demonstrates:

- Timer initialization with a configurable prescaler and period.
- Hardware-generated PWM with a 50% duty cycle.
- Adjustable blinking frequency between approximately 1 Hz and 10 Hz.
- Mapping an ADC measurement to timer parameters.

## Repository progression

This exercise introduces hardware timers.

The same timer infrastructure is extended in the following exercises for PWM
brightness control, stopwatch timing and additional timer-based applications.
