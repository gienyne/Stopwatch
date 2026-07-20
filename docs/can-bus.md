# CAN Bus

Notes behind the multi-node network in [`weather-station`](../projects/weather-station).

## Frame types

| Frame | Purpose |
|---|---|
| Data Frame | Carries payload (e.g. a sensor reading) |
| Remote Frame | Requests another node to send its data |
| Error Frame | Signals a detected bus error |
| Overload Frame | Inserts a delay when a node is overloaded |

## Data frame structure

```
SOF (1 dominant bit) → Arbitration field (11 or 29 bit ID + priority)
→ Control field (6 bit, incl. frame type + DLC) → Data field (0–8 bytes)
→ CRC field (16 bit) → ACK field (2 bit) → EOF (7 recessive bits)
```

## Arbitration (CSMA/CR)

Bit `0` is **dominant**, bit `1` is **recessive**. When multiple nodes transmit simultaneously, each compares the bit it's driving against what's actually on the bus; a node that reads back a dominant bit while it sent a recessive one has lost arbitration and immediately backs off. Practically: a lower numeric identifier always wins bus access, since a `0` in a higher bit position dominates any node trying to send a `1` there — this is what makes the identifier double as a priority.

## Bit stuffing

CAN uses NRZ encoding, where 5+ identical consecutive bits give no signal transition to resynchronize on. Bit stuffing automatically inserts an opposite-polarity bit after every 5 consecutive identical bits, guaranteeing regular transitions for clock recovery.

## bxCAN test modes

| Mode | Behavior |
|---|---|
| Silent | Receives normally, never transmits — for passively observing bus traffic |
| Loopback | Transmits to itself internally, never touches the physical bus — for self-test |
| Combined (Silent + Loopback) | Internal send/receive loop, isolated from the bus entirely |

This project's CAN logic was validated in **Loopback mode** first — with a logic analyzer's CAN decoder watching the TX pin to confirm outgoing frames — before connecting to the shared physical bus.

## Filter bank example (Mask mode, 16-bit scale)

Given `CAN_FiR1 = 0xFE00AA00`, `CAN_FiR2 = 0xFE00D600`, mask mode, 16-bit scale:

- Mask `0xFE00` = `1111111000000000` → only the upper 7 bits (15–9) of an incoming ID are checked; the rest are don't-care.
- Filter 0: `0xAA00` → `1010101000000000`
- Filter 1: `0xD600` → `1101011000000000`

| ID (binary) | Passes? | Matching filter |
|---|---|---|
| `0b11000110101` | No | — |
| `0b10101011001` | Yes | Filter 0 |
| `0b11010111111` | Yes | Filter 1 |

Corresponding `CAN_FilterTypeDef`:

```c
CAN_FilterTypeDef filter = {
    .FilterBank           = 0,
    .FilterMode           = CAN_FILTERMODE_IDMASK,
    .FilterScale          = CAN_FILTERSCALE_16BIT,
    .FilterFIFOAssignment = CAN_FILTER_FIFO0,
    .FilterActivation     = ENABLE,
    .FilterIdHigh         = 0xAA00,   // Filter 0 ID
    .FilterMaskIdHigh     = 0xFE00,   // Filter 0 mask
    .FilterIdLow          = 0xD600,   // Filter 1 ID
    .FilterMaskIdLow      = 0xFE00,   // Filter 1 mask
};
HAL_CAN_ConfigFilter(&hcan, &filter);
```

(Field-to-register mapping cross-checked against `HAL_CAN_ConfigFilter()` in `stm32f4xx_hal_can.c`.)

## `CAN_TxHeaderTypeDef`

| Field | Meaning |
|---|---|
| `StdId` | 11-bit standard identifier |
| `ExtId` | 29-bit extended identifier |
| `IDE` | Standard vs. extended ID selector |
| `RTR` | Data frame vs. Remote frame |
| `DLC` | Payload length, 0–8 bytes |

(`TransmitGlobalTime` only applies to Time-Triggered Communication mode, unused here.)

## Receiving via interrupt

Two pieces are required to react to an inbound FIFO0 message by interrupt rather than polling:

```c
HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rxHeader;
    uint8_t data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, data);
    // handle the frame
}
```

## Network frame format used in this project

See [`weather-station`](../projects/weather-station#can-network-frame-format) for the concrete 7-bit-node-ID / 4-bit-sensor-ID identifier layout and per-sensor filter bank setup built on top of these primitives.
