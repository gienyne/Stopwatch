# utils - shared helper functions

Small utility functions reused by multiple modules.

## API

```c
void utils_delay_ms(uint32_t t);
void utils_gpio_port_write(GPIO_TypeDef *GPIO, uint16_t GPIO_PIN);
uint16_t utils_gpio_port_read(GPIO_TypeDef *GPIO);
```

## Delay

`utils_delay_ms()` provides a simple blocking millisecond delay.

The project originally implemented this as a software delay, then later migrated to a hardware timer (TIM2) during the timer chapter. The timer counts at 1 kHz, so one timer tick corresponds to exactly 1 ms. The function simply waits until the desired number of timer ticks has elapsed.

Although blocking delays should generally be avoided in production firmware, they are perfectly adequate here for short waits during initialization or simple user-interface tasks.

## GPIO helpers

`utils_gpio_port_write()` writes an entire 16-bit GPIO output register (`GPIOx->ODR`) in one operation.

`utils_gpio_port_read()` returns the complete GPIO input register (`GPIOx->IDR`) as a 16-bit value.

Working on the whole port instead of individual pins is convenient for peripherals such as the joystick or the staircase-light example, where several input bits are evaluated together.

## Why this module exists

These helper functions are intentionally kept separate from the peripheral drivers because they are shared across multiple modules (joystick, dot, env_sensor, etc.) and do not naturally belong to any single hardware component.
