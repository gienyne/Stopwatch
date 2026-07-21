# Introduction

The first exercise introduces the STM32CubeIDE development environment and the
STM32 HAL programming model.

Using only the two on-board LEDs of the STM32F429 Discovery board, the project
demonstrates the fundamental workflow of configuring GPIO peripherals and
controlling digital outputs before introducing reusable drivers in the following
exercises.

## Objectives

- Initialize the STM32 HAL.
- Configure GPIO pins as digital outputs.
- Control the on-board LEDs using the HAL GPIO API.
- Become familiar with the build, flash and debug workflow.

## Implementation

The application performs the following tasks:

- Initializes the STM32 HAL.
- Enables the GPIO peripheral clock.
- Configures the green and red LEDs as push-pull outputs.
- Demonstrates single and multiple GPIO writes using `HAL_GPIO_WritePin()`.
- Toggles the LEDs using `HAL_GPIO_TogglePin()`.
- Uses `HAL_Delay()` to create a visible blinking pattern.

No custom drivers are used at this stage. Every GPIO operation is performed
directly through the STM32 HAL to expose the basic programming model used
throughout the repository.

## Repository progression

This project establishes the GPIO programming model reused by every subsequent
exercise.

The same HAL functions introduced here later become the foundation of the
[`esd`](../modules/esd) driver, which multiplexes a four-digit seven-segment
display across thirteen GPIO pins.
