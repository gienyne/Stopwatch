# env_sensor - BME280 sensor + CAN communication

This module combines two responsibilities required by the weather-station project:

- acquisition of environmental data from a Bosch BME280 sensor over I²C;
- distribution of these measurements over a CAN network.

It also provides the timer, interrupt and CAN filtering logic required by the application.

---

## Hardware

| Resource | Assignment |
|----------|------------|
| I²C | I2C1 |
| GPIO | PB6 (SCL), PB7 (SDA) |
| Sensor | Bosch BME280 |
| CAN | CAN1 |
| GPIO | PB8 (CAN_RX), PB9 (CAN_TX) |
| Timer | TIM2 (1 s transmission period) |

---

## API

```c
void sensor_gpio_init(void);
void sensor_i2c_init(void);
int  sensor_bme280_init(void);

int  sensor_read(float *temperature, float *pressure, float *humidity);

int sensor_read_struct(Data *data);

void init_can(void);
void SendSensorData(uint8_t sensor_id, uint32_t value);

void timer_sensor_init(void);

bool knoten_wechsel_mitJoystick(void);
```

---

## Responsibilities

### BME280 driver integration

The Bosch BME280 driver is wrapped behind a simple interface.

The module supplies the driver's platform-specific callbacks:

- I²C register read
- I²C register write
- microsecond delay

so the application never interacts with the Bosch driver directly.

The sensor operates in **Forced Mode**:

1. trigger one measurement;
2. wait until conversion finishes;
3. read temperature, pressure and humidity;
4. return to sleep.

This minimizes unnecessary sensor activity.

---

### CAN communication

Every environmental quantity is transmitted as an independent CAN frame.

```
Temperature
↓

CAN ID:
GroupID | TEMP

Pressure
↓

CAN ID:
GroupID | PRESS

Humidity
↓

CAN ID:
GroupID | HUM
```

Each message contains exactly one measurement.

Separating the values simplifies filtering and allows receivers to update only the quantity that actually changed.

---

### CAN filtering

Three hardware filter banks are configured:

- Filter 0 -> Temperature
- Filter 1 -> Pressure
- Filter 2 -> Humidity

Incoming messages are decoded inside

```
HAL_CAN_RxFifo0MsgPendingCallback()
```

and stored as the most recent measurements of the selected node.

---

### Periodic transmission

TIM2 generates an interrupt every second.

The interrupt does not read the sensor directly.

Instead it simply raises

```
flag_send_data
```

which lets the main application perform the sensor read and CAN transmission outside the interrupt context.

This keeps the ISR short while maintaining a deterministic sampling interval.

---

### Node selection

The joystick selects the active CAN node (Group ID).

When the selected node changes, subsequent received CAN frames belonging to that group replace the values currently displayed on the LCD.

This allows one weather station to inspect measurements coming from any other node on the CAN network.

---

## Design

The module intentionally combines I²C, BME280 and CAN because they together form a single abstraction:

> environmental sensor node.

The application therefore only needs a few high-level functions (`sensor_read()`, `SendSensorData()`, etc.) instead of managing several independent peripheral drivers.
