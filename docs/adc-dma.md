# ADC & DMA

## Sample timing

With `ADC_CLOCK_SYNC_PCLK_DIV6` (ADC clock = APB2/6 = 84 MHz/6 = 14 MHz), 12-bit resolution, and a 3-cycle sampling time:

```
T_conv = sampling_cycles + resolution_cycles = 3 + 12 = 15 ADC clock cycles
T_conv (time) = 15 / 14 MHz ≈ 71.4 ns per sample
Sample rate = 1 / 71.4 ns ≈ 14.0 Msps (channel throughput ceiling)
```

The 12 "resolution cycles" come from the successive-approximation conversion needing one cycle per output bit at 12-bit resolution — this is what the datasheet's total conversion time table accounts for beyond the configurable sampling time.

## Why analog inputs need no pull-up/down

A GPIO in analog mode isn't being read as a logic level at all — the ADC needs the pin's voltage to float freely between 0 V and V<sub>REF</sub> so it can measure it. A pull resistor would bias that voltage toward one rail, corrupting the measurement; pull-ups/downs only make sense for a *digital* input, where the goal is a well-defined logic level rather than an arbitrary analog value.

## Getting the conversion result: three options

The HAL supports three ways to retrieve an ADC conversion result:

1. **Polling** — `HAL_ADC_Start` / `HAL_ADC_PollForConversion` / `HAL_ADC_GetValue`; simplest, blocks the caller ([`potis`](../drivers/potis)).
2. **Interrupt** — `HAL_ADC_Start_IT`, result collected in `HAL_ADC_ConvCpltCallback`.
3. **DMA** — `HAL_ADC_Start_DMA`; the ADC streams results directly into memory with zero CPU involvement per sample ([`potis_dma`](../drivers/potis_dma)).

## ADC + DMA configuration order

The correct initialization order is DMA first, then ADC, then start:

1. Configure the DMA stream/channel (destination buffer, circular mode if continuous sampling is wanted).
2. Configure the ADC (scan mode for multiple channels, continuous conversion) and link it to the DMA request.
3. Call `HAL_ADC_Start_DMA` to kick the pipeline off.

Reference Manual §13.8 ("Data management" / "Using the DMA") documents this dependency; getting the order wrong (e.g. starting the ADC before the DMA stream is armed) drops the first conversion(s).

### DMA request mapping (STM32F429, Reference Manual Tables 43/44)

| Peripheral | DMA controller | Stream | Channel |
|---|---|---|---|
| TIM1 Channel 3 | DMA2 | Stream 6 | Channel 6 |
| ADC3 | DMA2 | Stream 0 or 1 | Channel 2 |
| DAC2 | DMA1 | Stream 6 | Channel 7 |

## Averaging trade-off: divide by N vs. divide by N/4

[`potis_dma`](../drivers/potis_dma) averages a 100-sample ring buffer per channel. Dividing the running sum by the true sample count (100) gives an honest millivolt reading. Dividing by only a quarter of that (25) instead of 100 artificially amplifies the result:

- **Upside**: more visible resolution — useful for weak signals, or to make small bar-graph movements visible without extra hardware gain.
- **Downside**: the output is no longer a real voltage (it can't be interpreted as mV or % directly), and a fixed-width accumulator (e.g. `uint16_t`) is more likely to overflow since the scaled sum runs closer to — or past — its numeric range.
