# CAN Bus

These notes summarize the concepts that were applied while implementing the
multi-node weather station.

The project uses the STM32F429 CAN peripheral to periodically exchange
environmental measurements between several independent weather nodes over a
shared CAN bus.

---

# Why CAN?

Unlike UART, CAN is designed for communication between many embedded devices
sharing the same physical bus.

Every node can transmit and receive without a central controller while the
hardware automatically resolves transmission conflicts through bus arbitration.

This makes CAN particularly suitable for

- industrial automation
- automotive systems
- robotics
- distributed sensor networks

which is why it was selected for the Weather Station project.

---

# CAN Frame Types

Four frame types exist.

| Frame | Purpose |
|---------|----------|
| Data Frame | Transmits application data |
| Remote Frame | Requests another node to transmit data |
| Error Frame | Indicates a communication error |
| Overload Frame | Delays the next transmission when necessary |

Only **Data Frames** are used in this project.

---

# Standard CAN Data Frame

A standard (11-bit identifier) CAN frame consists of the following fields.

```text
 SOF
  │
  ▼
┌───────────────┬─────────┬──────────┬────────────┬──────┬─────┬─────┐
│ Arbitration   │ Control │ Payload  │ CRC        │ ACK  │ EOF │     │
│ 11-bit ID     │ DLC     │ 0...8 B  │ Error Check│      │     │
└───────────────┴─────────┴──────────┴────────────┴──────┴─────┴─────┘
```

The payload carries the sensor value while the identifier determines both the
message priority and its meaning.

---

# Arbitration (CSMA/CR)

One of CAN's defining features is **non-destructive arbitration**.

If multiple nodes begin transmitting simultaneously, every node continuously
monitors the bus while sending.

```
Dominant bit = 0
Recessive bit = 1
```

If a node transmits a recessive bit but detects a dominant bit on the bus, it
immediately stops transmitting.

The node with the **lowest numerical identifier** therefore wins access to the
bus without corrupting the transmitted frame.

This means that

- lower CAN ID
- higher priority

are exactly the same thing.

Unlike Ethernet, no frame collisions occur.

---

# Bit Stuffing

CAN uses **NRZ (Non Return to Zero)** encoding.

A long sequence of identical bits would provide no clock transitions, making
synchronization difficult.

To solve this, the controller automatically inserts one opposite-polarity bit
after every five consecutive identical bits.

```text
11111
      |

111110
```

This process is completely transparent to the application.

---

# CAN Test Modes

During development the communication stack was verified incrementally using the
different hardware test modes provided by the STM32 CAN peripheral.

| Mode | Description |
|-------|-------------|
| Silent | Receive only, never acknowledge or transmit |
| Loopback | Internal transmit → receive without using the physical bus |
| Silent + Loopback | Internal self-test while remaining electrically isolated |

The Weather Station project was first validated entirely in **Loopback Mode**
before connecting multiple physical nodes.

A logic analyzer with CAN decoding was used to verify the generated frames.

---

# CAN Filters

Receiving every CAN frame wastes CPU time.

Instead, the STM32 hardware can discard unwanted frames before they ever reach
the software.

Each filter compares selected identifier bits against a predefined pattern.

Example:

```
Mask

1111111000000000

↑ these bits must match
↓ remaining bits ignored
```

This allows one filter to accept an entire range of identifiers.

The Weather Station uses separate filter banks so each node only receives the
sensor messages that belong to the supported sensor types.

---

# Message Format Used in this Project

The application uses the 11-bit Standard Identifier.

```
┌───────────────┬─────────────┐
│ Node ID       │ Sensor ID   │
└───────────────┴─────────────┘
```

The identifier therefore tells the receiver

- which weather station transmitted the frame
- which environmental quantity is contained in the payload

The payload itself contains one measurement.

| Sensor | Payload |
|---------|---------|
| Temperature | int16_t (0.01 °C) |
| Pressure | uint32_t (Pa) |
| Humidity | uint16_t (0.01 %) |

Separating metadata (identifier) from measurement data (payload) keeps the
frame compact while allowing simple hardware filtering.

---

# Interrupt-driven Reception

Instead of continuously polling the receive FIFO, the project reacts to incoming
messages using interrupts.

```c
HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
```

Whenever a frame arrives, the HAL automatically invokes

```c
HAL_CAN_RxFifo0MsgPendingCallback()
```

where the application retrieves the message using

```c
HAL_CAN_GetRxMessage()
```

This avoids unnecessary CPU usage and allows the microcontroller to remain idle
until new data becomes available.

---

# Development Workflow

The CAN implementation was validated in several stages.

1. Verify frame transmission in Loopback Mode.
2. Decode transmitted frames using a logic analyzer.
3. Test CAN filters with different identifiers.
4. Connect multiple STM32 boards to the physical bus.
5. Verify periodic sensor exchange between independent weather stations.

Developing the network incrementally greatly simplified debugging and ensured
that every communication layer worked before introducing additional complexity.
