# 05_Potis_DMA — ADC + DMA, continuous & averaged

Extends [`04_Potis`](../04_Potis) by moving ADC acquisition off the CPU entirely, via [`potis_dma`](../modules/potis_dma) — continuous circular-DMA sampling with ring-buffer averaging to eliminate the ADC noise visible in the polling-mode version.

## What this covers

- Correct ADC + DMA configuration order (DMA stream armed before the ADC starts, documented in [`docs/adc-dma.md`](../docs/adc-dma.md)) and the DMA request mapping (Reference Manual Tables 43/44) for various peripherals.
- Circular DMA mode: the ADC free-runs and DMA continuously refills a buffer with zero CPU involvement per sample, versus the one-shot start/stop cycle from [`04_Potis`](../04_Potis).
- Diagnosing ADC noise on the raw readings and addressing it with a 100-sample ring buffer, averaged on read.
- The concrete trade-off explored between dividing by the true sample count (real millivolt average) versus dividing by only a quarter of it (higher apparent resolution, but no longer a real voltage, and closer to overflowing a fixed-width accumulator) — see [`potis_dma`](../modules/potis_dma)'s README.

See [`potis_dma`](../modules/potis_dma) for the DMA stream/channel assignment and API.
