# I²C Protocol

Notes behind the BME280 integration in [`weather-station`](../projects/weather-station).

## Bus initiation

A transfer starts with a **Start Condition**: SDA transitions High → Low while SCL stays High. Both lines idle High (open-drain + pull-up) when the bus is free.

## Repeated Start

A **Repeated Start** is a second Start Condition sent without an intervening Stop — it lets the master change transfer direction (e.g. from writing a register address to reading its contents) without releasing the bus, so no other master can interject between the write and the read half of the transaction.

## NACK

A receiver signals **Not Acknowledge** by leaving SDA High during the ACK clock pulse — either because it has no more data to send/accept, or because something was invalid (e.g. an unrecognized address).

## Write sequence

1. Start Condition
2. Slave address + Write bit (R/W = 0)
3. Wait for ACK
4. Register address (the slave's internal register to target)
5. Wait for ACK
6. Data byte(s)

The two bytes preceding the actual payload are always **address+W** and **register address**.

## Read vs. write

| | Write | Read |
|---|---|---|
| Data direction | Master → Slave | Slave → Master |
| R/W bit | 0 | 1 |
| Typical flow | Single phase | Two phases: write the target register, Repeated Start, then read — switching direction without releasing the bus |

The HAL exposes this distinction as separate `HAL_I2C_Mem_Write()` / `HAL_I2C_Mem_Read()` calls, which handle the Repeated Start internally, versus the lower-level `HAL_I2C_Master_Transmit()` / `HAL_I2C_Master_Receive()` for simple, addressless transfers.

## BME280 wiring (I²C mode)

| BME280 pin | Connects to | Function |
|---|---|---|
| VCC | 3.3 V | Supply |
| GND | GND | Ground |
| SCL | PB6 (I2C1_SCL) | Clock |
| SDA | PB7 (I2C1_SDA) | Data |
| CSB | 3.3 V | Selects I²C mode (Low would select SPI) |
| SDO | GND or 3.3 V | I²C address select: GND → `0x76`, 3.3 V → `0x77` |

## Bosch driver integration

The Bosch reference driver is hardware-agnostic: it never calls the HAL directly, and instead expects the application to hand it function pointers for I²C read/write and a microsecond delay (the reference library's README labels this field `delay_ms`, which is a documentation error — the field is actually consumed in **microseconds**):

```c
struct bme280_dev dev;
dev.intf     = BME280_I2C_INTF;
dev.read     = user_i2c_read;
dev.write    = user_i2c_write;
dev.delay_us = user_delay_us;
dev.intf_ptr = &hi2c1;
bme280_init(&dev);
```

Because `bme280_init()` itself performs read/write transactions on the sensor during initialization, the I²C and GPIO peripherals must be fully configured and enabled *before* this call — there's no deferred setup inside the library.

The library reports temperature, pressure, and humidity as floating-point values in °C, Pa, and %RH respectively.
