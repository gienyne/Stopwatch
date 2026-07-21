# ADC with DMA

This exercise extends the previous polling-based implementation by moving the
ADC acquisition to DMA.

Instead of starting a conversion every time the application needs a value, the
ADC continuously samples both potentiometers while the DMA automatically stores
the results in a circular buffer. The application only reads already available
data and computes an averaged measurement.

## Objectives

- Configure ADC scan mode for multiple channels.
- Transfer conversion results using DMA.
- Use circular DMA for continuous sampling.
- Reduce measurement noise by averaging multiple samples.

## Implementation

The application continuously samples both potentiometers using ADC1 and DMA.

The DMA writes the conversion results into a circular ring buffer containing
the latest 100 samples for each channel.

Whenever the application requests a measurement, it computes the arithmetic
mean of the stored samples and converts the result into millivolts before
displaying it on the LCD.

Compared to the previous polling implementation, no CPU intervention is
required for individual ADC conversions.

## Repository progression

This exercise introduces autonomous peripheral operation.

Later projects reuse the same principle whenever peripherals continuously
exchange data without requiring the CPU to manage every transfer.
