# potis_dma

DMA-based driver for continuously sampling the two potentiometers on the
Waveshare Analog Test Board.

The module combines ADC scan mode with circular DMA transfers to continuously
fill a ring buffer. Application code reads averaged values instead of individual
ADC conversions, resulting in more stable measurements.

## Hardware

| Resource | Assignment |
|----------|------------|
| GPIO | PA6 (ADC1_IN6), PA7 (ADC1_IN7) |
| ADC | ADC1, 12-bit, scan mode |
| DMA | DMA2, Stream 4, Channel 0 |

## Public API

```c
void potis_DMA_init(void);
void potis_ADC_init(void);

uint32_t potis_DMA_get_average_val_mv(uint8_t poti_num);
```

- `potis_DMA_init()` configures DMA2 for circular memory transfers.
- `potis_ADC_init()` configures ADC1 for continuous scan conversion and starts
  the DMA transfer.
- `potis_DMA_get_average_val_mv()` computes the average of the samples stored
  in the ring buffer and returns the corresponding voltage in millivolts.

## Notes

The ring buffer stores the most recent 100 samples for each potentiometer.
Averaging these samples significantly reduces the noise visible in individual
ADC conversions while requiring no additional CPU work during data acquisition.

This module demonstrates how DMA can continuously acquire data in the
background while the application only processes the results when needed.
