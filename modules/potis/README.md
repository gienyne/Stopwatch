# potis — polling-mode dual potentiometer ADC

Reads the two potentiometers on the Waveshare Analog Test Board via `ADC1`, in single-conversion (non-continuous) polling mode.

## Hardware mapping

| Signal | Pin | ADC channel |
|---|---|---|
| Poti 1 | PA7 | ADC1_IN7 |
| Poti 2 | PA6 | ADC1_IN6 |

## API

```c
void potis__ADC_init(void);
uint32_t potis_ADC_get_val(uint8_t poti_num);
uint32_t potis_ADC_get_val_mv(uint8_t poti_num);
```

`potis__ADC_init` configures both pins as analog inputs (no pull-up/down needed — the ADC input stage presents a high impedance and a pull resistor would bias the analog reading) and initializes ADC1 at 12-bit resolution.

Each call to `potis_ADC_get_val` starts the ADC, waits for end-of-conversion, reads the result, and stops the ADC again — see [`potis_dma`](../potis_dma) for the continuous/averaged variant used once noise on the raw reading became a problem.

`potis_ADC_get_val_mv` scales the 12-bit result (0–4095) against the 3.3 V reference to millivolts.

### Sampling time, for reference

With `ADC_CLOCK_SYNC_PCLK_DIV6` and `ADC_SAMPLETIME_3CYCLES` at 12-bit resolution, ADC1 runs at 84 MHz / 6 = 14 MHz. A conversion takes `3 + 12 = 15` ADC clock cycles ≈ 71.4 ns, i.e. roughly 14 million samples/second per channel — the practical throughput is lower here since conversions are triggered one at a time from software rather than back-to-back.
