# P2_Weatherstation

A distributed weather station built around the **Bosch BME280** environmental
sensor and the **STM32F429** CAN peripheral.

Each board periodically measures **temperature**, **pressure**, and
**humidity**, publishes the measurements on a shared CAN bus, and can display
either its own values or the latest values received from any other node in the
network.

Unlike the previous projects, this application combines multiple embedded
communication layers in a single system:

- I²C for sensor communication
- CAN for inter-node communication
- Timer-driven periodic sampling
- LCD visualization
- Joystick-controlled node selection

---

## System Architecture

```text
                    +----------------------+
                    |     BME280 Sensor    |
                    +----------------------+
                               │
                              I²C
                               │
                               ▼
                    env_sensor module
                               │
             Temperature / Pressure / Humidity
                               │
               every 1 second (hardware timer)
                               │
                               ▼
                      CAN Transmission
                               │
                ───────── CAN Bus ─────────
                               │
          ┌────────────────────┴────────────────────┐
          │                                         │
          ▼                                         ▼
     Local Weather Node                      Remote Weather Nodes
          │                                         │
          └────────────── CAN Reception ────────────┘
                               │
                               ▼
                     Latest received values
                               │
                     Node selected by joystick
                               │
                               ▼
                       LCD User Interface
```

---

## Features

- Reads temperature, pressure and humidity from a Bosch BME280.
- Uses I²C to communicate with the sensor.
- Publishes measurements once per second over CAN.
- Receives measurements from other weather nodes.
- Displays either local or remote sensor values.
- Selects the displayed node using the joystick.
- Visualizes all values using reusable LCD bar graphs.

---

## Hardware

| Component | Purpose |
|-----------|----------|
| STM32F429 Discovery | Main controller |
| Waveshare Open429Z-D | Expansion board |
| Bosch BME280 | Temperature, humidity and pressure sensor |
| CAN Transceiver | CAN communication |
| TFT LCD | User interface |
| Joystick | Node selection |

---

## Software Architecture

The project is intentionally divided into layers.

### env_sensor

Responsible for

- BME280 initialization
- I²C communication
- Bosch driver integration
- CAN initialization
- CAN frame encoding
- CAN transmission
- CAN reception

The application therefore never communicates directly with the Bosch driver or
the CAN peripheral.

---

### main

The application layer only coordinates the modules.

1. Initialize LCD, sensor and CAN.
2. Wait for the periodic timer.
3. Read the local sensor.
4. Publish the values on CAN.
5. Receive new CAN messages.
6. Display the currently selected weather node.

---

## CAN Message Format

Every transmitted CAN frame contains one sensor value.

| Sensor | Payload |
|---------|---------|
| Temperature | int16_t (0.01 °C) |
| Pressure | uint32_t (Pa) |
| Humidity | uint16_t (0.01 %) |

The Standard Identifier contains

- Node ID
- Sensor ID

allowing every node to distinguish both the sender and the sensor type.

---

## LCD Visualization

The LCD displays

- current node ID
- temperature
- pressure
- humidity

Each value is shown as

- numerical value
- engineering unit
- horizontal bar graph

using the reusable `my_lcd` module developed earlier in the repository.

---

## Development Highlights

This project introduced several embedded-system concepts:

- I²C master communication
- Integration of a third-party sensor driver
- Layered hardware abstraction
- CAN communication
- CAN filtering
- Multi-node embedded networks
- Periodic real-time data acquisition
- Embedded graphical user interfaces

---

## Validation

Development was performed incrementally.

1. Validate the BME280 using I²C.
2. Verify CAN communication in Loopback Mode.
3. Decode transmitted CAN frames with a Logic Analyzer.
4. Connect multiple nodes to the physical CAN bus.
5. Verify data exchange between independent weather stations.

---

## Related Modules

- `modules/env_sensor`
- `modules/my_lcd`
