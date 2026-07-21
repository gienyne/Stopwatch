# 04_Potis — ADC in polling mode

Introduces the ADC peripheral: reading the two potentiometers on the Waveshare Analog Test Board via [`potis`](../modules/potis), one-shot polling mode, displayed live on the LCD.

## What this covers

- Identifying which GPIO pins and ADC input channels the Analog Test Board's potentiometers are wired to, from the board schematics and the controller datasheet.
- Configuring a GPIO pin for analog input, and why analog inputs need **no** pull-up/pull-down (a pull resistor would bias the very voltage the ADC is trying to measure — pull resistors only make sense for digital logic levels).
- One-shot conversion in polling mode: start ADC → poll for end-of-conversion → read result → stop ADC, repeated on every call rather than left running continuously (that comes next, in [`05_Potis_DMA`](../05_Potis_DMA)).
- Sample-time / conversion-time arithmetic: with `ADC_CLOCK_SYNC_PCLK_DIV6`, 12-bit resolution, and `ADC_SAMPLETIME_3CYCLES`, deriving the per-sample conversion time and resulting sample rate from the ADC clock (see [`docs/adc-dma.md`](../docs/adc-dma.md)).
- The three ways the HAL exposes a conversion result — polling, interrupt, and DMA — and why this exercise deliberately starts with the simplest (polling) before [`05_Potis_DMA`](../05_Potis_DMA) moves to the third.
- Rendering both potentiometers as bar graphs plus their millivolt value as text on the LCD.

See [`potis`](../modules/potis) for the pin/channel mapping and API.
