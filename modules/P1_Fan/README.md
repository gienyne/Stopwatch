# P1_Fan

Hardware abstraction and control module for the PWM-driven DC fan used in
Project 1.

The module brings together all peripherals required by the closed-loop speed
controller, including PWM generation, tachometer acquisition, RPM measurement
and the discrete PI controller.

## Responsibilities

- Generate the PWM signal driving the fan.
- Measure the fan speed from the tachometer signal.
- Filter RPM measurements using the median filter module.
- Execute the discrete PI controller at a fixed sampling period.
- Convert potentiometer input into the desired speed setpoint.
- Provide the current setpoint and measured speed to the application.

## Hardware Resources

| Peripheral | Purpose |
|------------|---------|
| TIM9 | PWM generation |
| TIM1 | Tachometer period measurement |
| TIM10 | Periodic PI controller execution |
| EXTI | Tachometer pulse detection |
| GPIO PE5 | Fan PWM output |
| GPIO PF6 | Fan tachometer input |

## Public API

```c
void gpio_fan_init(void);

void timer_init(void);
void fan_oc_init(void);

void gpio_tacho_init(void);
void timer_rpm_init(void);

void Ta_pid_timer_init(void);

void set_fan_speed_percent(uint8_t percent);

void pi_regler_update(void);

uint32_t fan_get_soll_wert(void);
uint32_t fan_get_speed(void);
```

## Control Strategy

The fan speed is controlled using a discrete PI controller executed every
100 ms by TIM10.

The desired speed is obtained from the DMA-based potentiometer module, while
the actual speed is calculated from the time between two tachometer pulses.
A median filter removes measurement outliers before the controller computes
the PWM duty cycle.

## Notes

This module depends on:

- `potis_DMA` for the speed setpoint
- `median` for RPM filtering

Visualization of the controller state is intentionally left to the application
(`P1_Fan_Control`), allowing the module to be reused independently of the user
interface.