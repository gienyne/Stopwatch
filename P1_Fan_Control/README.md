# Fan Speed Control

A closed-loop fan controller implemented on the STM32F429 using hardware timers,
PWM, ADC with DMA, external interrupts and a discrete PI controller.

The system continuously measures the actual fan speed, compares it with a
user-selected target speed and automatically adjusts the PWM duty cycle to
maintain the requested RPM.

---

## Overview

Unlike the previous exercises, this project combines several STM32 peripherals
into a complete embedded control application.

The project demonstrates how measurement, filtering and control can be
integrated into a real-time feedback loop running entirely on a
microcontroller.

---

## Features

- Closed-loop PI speed regulation
- PWM-based motor control
- Tachometer-based RPM measurement
- ADC + DMA acquisition of the speed setpoint
- Median filtering of RPM measurements
- Hardware timer driven control loop
- Real-time LCD visualization

---

## System Architecture

```text
                 Potentiometer
                       │
                       ▼
              Desired Speed (RPM)
                       │
                       ▼
               PI Controller (100 ms)
                       │
                       ▼
                PWM Duty Cycle
                       │
                       ▼
                  DC Fan Motor
                       │
             Tachometer Pulses
                       │
                       ▼
                EXTI Interrupt
                       │
                       ▼
              RPM Measurement
                       │
                       ▼
                 Median Filter
                       │
                       └───────────────┐
                                       │
                                       ▼
                              Feedback Loop
```

---

## Hardware Resources

| Peripheral | Purpose |
|------------|---------|
| ADC1 + DMA | Read potentiometer position |
| TIM9 | Generate 25 kHz PWM signal |
| TIM1 | Measure time between tachometer pulses |
| TIM10 | Execute the PI controller every 100 ms |
| EXTI | Detect tachometer rising edges |
| LCD | Display measured values |

---

## Control Strategy

The potentiometer defines the desired fan speed.

The fan's tachometer generates pulses proportional to the current rotational
speed. Every pulse triggers an interrupt, while a hardware timer measures the
elapsed time between consecutive pulses to calculate the current RPM.

To reduce measurement noise, the raw RPM values are passed through a median
filter before entering the controller.

Every 100 ms a dedicated timer executes the PI controller, which computes the
required PWM duty cycle from the difference between the desired and measured
speed.

The resulting duty cycle is applied directly to the PWM output driving the fan.

---

## PI Controller

The controller is implemented as a discrete PI controller

```text
u = Kp · e + Ki · Ta · Σe
```

where

- **e** is the speed error,
- **Ta** is the controller sampling period,
- **Kp** is the proportional gain,
- **Ki** is the integral gain.

The controller output is limited to the valid PWM range before updating the
hardware timer.

---

## Display

The LCD provides live feedback including

- desired speed
- measured speed
- setpoint bar graph
- actual speed bar graph

allowing the regulation behaviour to be observed in real time.

---

## Learning Outcomes

This project combines concepts introduced throughout the repository into a
single embedded application:

- ADC
- DMA
- PWM
- Hardware timers
- External interrupts
- Signal filtering
- Closed-loop feedback control
- Real-time embedded software design
