# P2_Weatherstation — BME280 sensor node on a CAN network

A networked weather node: reads temperature, pressure and humidity from a BME280 over I²C, publishes them on a shared CAN bus, and displays either its own or any other node's latest readings on the LCD, selectable via the joystick.

All BME280 and CAN handling lives in [`modules/env_sensor`](../modules/env_sensor) — see that module's README for the wiring, the Bosch driver integration, and the CAN frame format used by the network. This file covers the application layer built on top of it.

## Application flow (`main.c`)

1. Initialize LCD, sensor (I²C + BME280), CAN, and joystick.
2. On a joystick node-switch event: clear the LCD's sensor-value area and reset the locally cached readings.
3. Once per second (`flag_send_data`, set by `timer_sensor_init`): read the local BME280, publish all three values on CAN (`SendSensorData`), then decide which values to display — either the freshly-read local data, or the most recently received CAN values for whichever node is currently selected.
4. Render each value as a bar graph + numeric readout (via [`my_lcd`](../modules/my_lcd)).

## LCD layout

Three rows (temperature in red, pressure in blue, humidity in green), each with a bar graph on the left and the numeric value + unit on the right, per-mille-scaled against a fixed reference range (50 °C, 1100 hPa, 100 %).

## Testing note

Bring-up and CAN validation were done in the bxCAN peripheral's **Loopback mode** first, cross-checked with a logic analyzer's CAN decoder on the TX pin, before connecting to the physical multi-node bus.
