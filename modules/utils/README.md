# utils — shared helpers with no natural home elsewhere

Small cross-cutting functions used by several other modules.

## API

```c
void utils_delay_ms(uint32_t t);
void utils_gpio_port_write(GPIO_TypeDef *GPIO, uint16_t GPIO_PIN);
uint16_t utils_gpio_port_read(GPIO_TypeDef *GPIO);
```

- **`utils_delay_ms`** — blocking millisecond delay. Started on the HAL's `SysTick`-based `HAL_Delay`, then re-implemented on a hardware timer once precise, non-SysTick-dependent timing was needed elsewhere in the project (timers keep counting independent of interrupt latency on `SysTick`, which matters once other interrupts are competing for CPU time).
- **`utils_gpio_port_write`** — writes an entire 16-bit port in one register access (`GPIOx->ODR` write) instead of one `HAL_GPIO_WritePin` call per bit, so a multi-bit value (e.g. a binary pattern across several pins of the same port) is set and cleared atomically.
- **`utils_gpio_port_read`** — the read-side counterpart, returning the whole port's input register as a 16-bit bitmask.

## Why these live here

Both GPIO helpers are used by [`joystick`](../joystick), [`dot`](../dot)'s staircase-light logic, and elsewhere — functions that don't belong to one specific peripheral driver, or that are useful across several, land in `utils` rather than being duplicated.
