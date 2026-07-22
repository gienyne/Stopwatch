# P2_Weatherstation

A distributed embedded weather station built around the **Bosch BME280** and the
**STM32F429 CAN peripheral**.

Each node periodically measures **temperature**, **pressure**, and
**humidity**, publishes the measurements over a shared **CAN bus**, and can
display either its own sensor values or the latest measurements received from
any other node. The active node is selected at runtime using the joystick.

Unlike the previous projects, this application combines several communication
layers into one complete embedded system:

- I²C sensor communication
- Bosch BME280 driver integration
- CAN-based distributed communication
- Timer-driven periodic sampling
- LCD graphical visualization
- User interaction through the joystick

---

## System Architecture

```text
                  ┌────────────────────┐
                  │     BME280 Sensor  │
                  └────────────────────┘
                             │
                            I²C
                             │
                             ▼
                    env_sensor module
                             │
          Temperature • Pressure • Humidity
                             │
                   periodic timer (1 Hz)
                             │
                             ▼
                    CAN Message Encoder
                             │
                    ───── CAN Bus ─────
                             │
         ┌───────────────────┴───────────────────┐
         │                                       │
         ▼                                       ▼
    Local Weather Node                    Remote Weather Nodes
         │                                       │
         └──────────── CAN Reception ────────────┘
                             │
                             ▼
                  Latest values per Node ID
                             │
                   Joystick Node Selection
                             │
                             ▼
                    LCD User Interface
```

---

## Features

- Reads temperature, pressure and humidity from a Bosch BME280.
- Uses I²C for sensor communication.
- Publishes measurements once per second over CAN.
- Receives measurements from multiple remote weather nodes.
- Switches between local and remote nodes using the joystick.
- Displays every value numerically and as a graphical bar.
- Separates hardware access from the application logic through dedicated modules.

---

## Hardware

| Component | Purpose |
|-----------|---------|
| STM32F429 Discovery | Main controller |
| Waveshare Open429Z-D | Expansion board |
| Bosch BME280 | Environmental sensor |
| CAN transceiver | CAN communication |
| TFT LCD | User interface |
| Joystick | Node selection |

---

## Software Architecture

The application follows a layered design.

### `env_sensor`

The `env_sensor` module encapsulates all hardware-specific functionality.

Responsibilities include

- BME280 initialization
- I²C communication
- Bosch driver integration
- CAN peripheral configuration
- CAN frame encoding
- CAN transmission
- CAN reception

The application never accesses either the Bosch driver or the CAN peripheral
directly.

---

### `main`

`main.c` contains only the application workflow.

1. Initialize LCD, joystick, sensor and CAN.
2. Wait for the periodic timer.
3. Read the local BME280.
4. Publish the measurements over CAN.
5. Process newly received CAN frames.
6. Display either the local node or the selected remote node.

---

## CAN Message Format

Each CAN frame transports one environmental measurement.

| Sensor | Payload |
|---------|---------|
| Temperature | int16_t (0.01 °C) |
| Pressure | uint32_t (Pa) |
| Humidity | uint16_t (0.01 %) |

The Standard Identifier encodes

- Node ID
- Sensor ID

allowing every receiver to identify both the sender and the transmitted
measurement without increasing the payload size.

---

## LCD Visualization

The display shows

- selected node
- temperature
- pressure
- humidity

Each measurement is rendered as

- numerical value
- engineering unit
- horizontal bar graph

using the reusable `my_lcd` graphics module.

---

## Development Highlights

This project demonstrates several important embedded-system concepts:

- I²C master communication
- Integration of a third-party sensor driver
- Layered driver architecture
- CAN communication and filtering
- Distributed embedded systems
- Periodic real-time data acquisition
- Embedded graphical user interfaces

---

## Validation

Development was carried out incrementally.

1. Validate the BME280 over I²C.
2. Verify CAN communication in Loopback Mode.
3. Decode transmitted CAN frames using a Logic Analyzer.
4. Connect multiple physical nodes to the CAN bus.
5. Verify real-time synchronization between weather stations.

---

## Related Modules

- `modules/env_sensor`
- `modules/my_lcd`
