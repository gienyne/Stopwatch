# potis_dma — DMA-driven potentiometer sampling with averaging

Continuous, CPU-free acquisition of both potentiometers via `ADC1` in circular DMA mode, with a running average over the last 100 samples per channel to smooth out ADC noise.

## Hardware / peripherals

| Resource | Assignment |
|---|---|
| GPIO | PA6 (ADC1_IN6), PA7 (ADC1_IN7) |
| ADC | ADC1, 12-bit, 2 channels (scan mode) |
| DMA | DMA2, Stream 4, Channel 0 |

## API

```c
void potis_DMA_init(void);
void potis_ADC_init(void);
uint32_t potis_DMA_get_average_val_mv(uint8_t poti_num);
```

`potis_DMA_init` configures DMA2/Stream4 in circular mode, targeting a `RING_PUFFER_SIZE`-deep buffer (100 samples × 2 channels); `potis_ADC_init` configures the ADC for scan conversion and hands control to the DMA. Once started, the ADC free-runs and the DMA continuously refills the ring buffer without CPU intervention — `potis_DMA_get_average_val_mv` just walks the buffer and returns the mean, converted to millivolts.

## Why DMA over polling

Moving the ADC from the [`potis`](../potis) polling loop to DMA removes the CPU from the sampling path entirely — conversions happen in the background at the ADC's own rate, and the application only touches the data when it needs a reading. It also enables the averaging: 100 independent samples per read meaningfully cut ADC/reference noise compared to a single one-shot conversion.

**Trade-off on the divisor**: dividing the running sum by the full sample count (100) gives a true millivolt average. Dividing by a quarter of that instead over-scales the result — useful if you want more visible resolution on a bar graph for a weak signal — but the output is then an artificially amplified value, not a real voltage, and it becomes easier to overflow a fixed-width accumulator (e.g. `uint16_t`) if the raw sum pushes past 65535.
