# potis

Driver for reading the two potentiometers on the Waveshare Analog Test Board
using ADC1 in polling mode.

The module configures the required GPIO pins and ADC peripheral, performs
single software-triggered conversions and provides the measured voltage either
as a raw ADC value or directly in millivolts.

## Hardware

| Signal | GPIO | ADC Channel |
|--------|------|-------------|
| Potentiometer 1 | PA7 | ADC Channel 7 |
| Potentiometer 2 | PA6 | ADC Channel 6 |

## Public API

```c
void potis__ADC_init(void);

uint32_t potis_ADC_get_val(uint8_t poti_num);
uint32_t potis_ADC_get_val_mv(uint8_t poti_num);
