# GPIO Interrupts & NVIC

## EXTI trigger modes

Each GPIO-backed EXTI line can be configured to fire on:

| Mode | Trigger |
|---|---|
| `GPIO_MODE_IT_RISING` | Low → High transition |
| `GPIO_MODE_IT_FALLING` | High → Low transition |
| `GPIO_MODE_IT_RISING_FALLING` | Either transition |

Set via the same `GPIO_InitTypeDef` used for regular pin configuration:

```c
GPIO_InitTypeDef gpio_init = {
    .Pin  = GPIO_PIN_7,
    .Mode = GPIO_MODE_IT_RISING,
    .Pull = GPIO_NOPULL,
};
HAL_GPIO_Init(GPIOB, &gpio_init);
```

This is exactly the mechanism [`fan-control`](../projects/fan-control) uses for the tacho signal (rising-edge triggered).

## Configuring EXTI independently of GPIO

The lower-level `stm32f4xx_hal_exti` module exposes `EXTI_ConfigTypeDef` directly, for cases where EXTI needs configuring separately from the GPIO init call:

```c
EXTI_HandleTypeDef hexti;   // obtained via HAL_EXTI_GetHandle()

EXTI_ConfigTypeDef exti_config = {
    .Line    = EXTI_LINE_7,
    .Mode    = EXTI_MODE_INTERRUPT,
    .Trigger = EXTI_TRIGGER_RISING,
};
HAL_EXTI_SetConfigLine(&hexti, &exti_config);
```

## Interrupt vs. Event

| | Interrupt | Event |
|---|---|---|
| Triggers an ISR | Yes | No |
| Affects program flow | Yes | No — internal pulse only |
| Consumer | The CPU | Other peripherals (e.g. triggering a timer start) |

Events are for hardware-to-hardware synchronization without CPU involvement; interrupts are for software reacting to a condition via an ISR.

## Why EXTI has 23 lines when a GPIO port only has 16 pins

Lines 0–15 map one-to-one to a GPIO port's 16 pins (only one port at a time per line, selected via `SYSCFG`). The remaining 7 lines (16–22) are wired to internal system/peripheral events rather than GPIO pins — RTC alarm/wakeup, PVD output, USB OTG wakeup, Ethernet wakeup, and similar sources, documented in the Reference Manual's external interrupt/event mapping figure.

## `USE_HAL_TIM_REGISTER_CALLBACKS`

With this define left at its default (`0`), every timer interrupt funnels through the same weak `HAL_TIM_PeriodElapsedCallback()`, and if multiple timers are in play, that callback needs a `switch`/`if` on `htim->Instance` to dispatch correctly.

Setting it to `1` enables per-timer, per-event callback *registration* (`HAL_TIM_RegisterCallback`), so each timer can have its own dedicated callback function instead of funneling through one shared handler with manual instance dispatch.
