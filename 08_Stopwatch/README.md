# Stopwatch with Timers and Interrupts

This exercise combines hardware timers with external interrupts to implement a
lap stopwatch running entirely on the STM32.

The user button is configured as an external interrupt source, while a hardware
timer provides the time base used to measure the elapsed time and individual lap
times.

## Objectives

- Configure timer interrupts.
- Configure GPIO external interrupts (EXTI).
- Understand the interaction between timers and the NVIC.
- Build an interrupt-driven embedded application.

## Implementation

The application demonstrates:

- High-resolution time measurement using a hardware timer.
- External interrupt handling through the USER button.
- Stopwatch start/stop logic.
- Lap-time recording.
- Real-time LCD visualization of the elapsed and lap times.

## Repository progression

This exercise combines most concepts introduced throughout the repository,
including GPIO, timers, interrupts and LCD visualization, into a complete
embedded application.
