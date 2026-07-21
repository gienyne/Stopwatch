# ADC in Polling Mode

This exercise introduces the STM32 ADC peripheral by reading the two
potentiometers available on the Waveshare Analog Test Board.

The application performs single conversions in polling mode, converts the
measured voltage to millivolts and visualizes both channels on the LCD using
text and bar graphs.

## Objectives

- Configure GPIO pins as analog inputs.
- Perform single ADC conversions in polling mode.
- Convert raw ADC values into millivolts.
- Visualize analog measurements on the LCD.

## Implementation

The application periodically reads both potentiometers connected to ADC1.

For each measurement it:

- selects the desired ADC channel,
- starts a software-triggered conversion,
- waits until the conversion completes,
- reads the 12-bit ADC result,
- converts it into millivolts,
- displays both the numeric value and a graphical bar graph.

The visualization is implemented using the reusable `my_lcd` module introduced
in the previous exercise.

## Repository progression

This project introduces analog data acquisition.

The following exercise replaces the polling approach with DMA-based continuous
sampling, allowing the CPU to process measurements without waiting for each
conversion to complete.
