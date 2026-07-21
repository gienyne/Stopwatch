# P1_Fan_Control — closed-loop PI speed regulation

A PWM-driven fan is regulated to a potentiometer-selected target RPM using a discrete PI controller, with live setpoint-vs-actual feedback on the LCD.

## Signal path

```
Potentiometer (modules/potis_dma) ──► setpoint (RPM)
                                            │
Tacho pulse (EXTI) ──► RPM measurement ──► median filter (modules/median) ──► actual (RPM)
                                            │                                      │
                                            └──────────► PI controller ◄───────────┘
                                                              │
                                                        PWM duty cycle
                                                              │
                                                          Fan (TIM9_CH1)
```

## Hardware

| Signal | Pin | Peripheral |
|---|---|---|
| Fan PWM | PE5 | TIM9_CH1 |
| Tacho input | PF6 | TIM10_CH1 / EXTI |

## Control loop

1. A timer generates the fan's PWM signal at the frequency the fan's datasheet requires.
2. Every rising edge of the tacho signal fires an EXTI interrupt; a second timer measures the time between two consecutive edges, from which the current RPM is derived.
3. The raw RPM is smoothed with the [`median`](../modules/median) filter to remove tacho-line glitches.
4. A fixed-period timer (`Ta_pid_timer_init`, `T_A = 0.1 s`) triggers `pi_regler_update()`, which computes the PWM duty cycle from the RPM error using:

   ```c
   #define K_P 0.12f
   #define K_I 0.02f
   #define T_A 0.1f
   ```

   `T_A` here is the controller's own fixed loop period, not the time between tacho pulses — keeping it constant (rather than deriving it from the variable tacho interval) is what makes the PI math a standard fixed-`Ta` discrete controller rather than a variable-timestep one.

## Tuning

Gains were first tuned against a MATLAB/Octave `pid_regulator.m` simulation before being carried over to hardware:

- **Kp only** (Ki = 0): too small → the fan settles well short of setpoint (steady-state error dominates); too large → saturates PWM at ~99% and, when *decreasing* setpoint, causes overshoot/oscillation from over-aggressive braking.
- **Adding Ki**: removes the steady-state error left by a P-only controller; too large causes sustained oscillation.
- **Final values**: `Kp = 2.0`, `Ki = 3.8` — settles within roughly 0.3–0.5 s with no sustained oscillation.

## LCD readout

`main.c` displays the numeric setpoint and actual RPM plus two bar graphs (setpoint in red, actual in blue, see [`my_lcd`](../modules/my_lcd)) refreshed once per control-loop tick.
