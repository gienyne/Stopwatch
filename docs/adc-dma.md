# ADC & DMA

Several projects in this repository require continuously reading analog signals,
such as potentiometer positions for LED brightness control and fan speed
regulation.

The STM32 provides three different approaches for acquiring ADC data, ranging
from simple polling to fully autonomous DMA transfers.

---

# Analog-to-Digital Conversion

The Analog-to-Digital Converter (ADC) converts an input voltage into a digital
value.

For a 12-bit ADC running from a 3.3 V reference:

| Input Voltage | ADC Value |
|--------------:|----------:|
| 0.0 V | 0 |
| 1.65 V | ≈ 2048 |
| 3.3 V | 4095 |

The conversion result can then be processed by software exactly like any other
integer.

---

# ADC Conversion Time

The total conversion time depends on

- the ADC clock
- the configured sampling time
- the ADC resolution

For this project:

| Parameter | Value |
|-----------|-------|
| ADC Clock | 14 MHz |
| Resolution | 12 bit |
| Sampling Time | 3 cycles |

Therefore

```
Conversion cycles

Sampling + Resolution

3 + 12 = 15 cycles
```

Resulting in

```
15 / 14 MHz

≈ 1.07 µs
```

per conversion.

This corresponds to a theoretical maximum throughput of approximately

```
≈ 933 kSamples/s
```

per ADC channel.

---

# Why Analog Pins Need No Pull-Up or Pull-Down

Unlike digital GPIOs, an analog input is not interpreted as HIGH or LOW.

Instead, the ADC measures the actual voltage present on the pin.

Adding a pull-up or pull-down resistor would shift that voltage toward one of the
supply rails and therefore distort the measurement.

For this reason, STM32 analog inputs are configured with

```
GPIO_MODE_ANALOG
GPIO_NOPULL
```

---

# Three Ways to Read the ADC

STM32 HAL supports three different acquisition methods.

## 1. Polling

```c
HAL_ADC_Start();
HAL_ADC_PollForConversion();
HAL_ADC_GetValue();
```

Advantages

- simplest implementation
- easy to understand

Disadvantages

- blocks the CPU while waiting
- unsuitable for continuous sampling

This method is used by the **potis** module.

---

## 2. Interrupt

```c
HAL_ADC_Start_IT();
```

The ADC generates an interrupt whenever a conversion finishes.

Advantages

- CPU can perform other work meanwhile

Disadvantages

- every sample still generates one interrupt

Useful for medium sampling rates.

---

## 3. DMA

```c
HAL_ADC_Start_DMA();
```

The ADC writes conversion results directly into memory without CPU involvement.

```text
ADC
 │
 ▼
DMA Controller
 │
 ▼
RAM Buffer
```

Advantages

- no interrupt per sample
- very low CPU load
- ideal for continuous measurements

This method is used throughout the later exercises, especially in
**potis_dma** and the **Fan Controller**.

---

# Correct Initialization Order

The DMA controller must already be prepared before the ADC starts producing
samples.

The correct sequence is therefore

```text
Initialize DMA
        │
        ▼
Initialize ADC
        │
        ▼
Start ADC + DMA
```

Starting the ADC before enabling DMA may cause the first conversion results to
be lost.

---

# Circular DMA

For continuously changing analog signals, DMA is configured in **Circular Mode**.

```text
Buffer

+----+----+----+----+----+
| A0 | A1 | A2 | A3 | A4 |
+----+----+----+----+----+
  ▲                       │
  └───────────────────────┘
```

Once the last element has been written, DMA automatically continues at the first
element again.

No software intervention is required.

---

# DMA Request Mapping

On STM32F429 every peripheral is connected to specific DMA controllers,
streams and channels.

Examples used throughout this repository:

| Peripheral | DMA | Stream | Channel |
|------------|------|--------|---------|
| ADC3 | DMA2 | Stream 0 / 1 | Channel 2 |
| TIM1 CH3 | DMA2 | Stream 6 | Channel 6 |
| DAC2 | DMA1 | Stream 6 | Channel 7 |

Selecting an incorrect mapping prevents DMA transfers from occurring.

---

# Noise Reduction by Averaging

Analog measurements naturally contain small fluctuations.

Instead of using every raw sample directly, the project averages multiple
measurements.

```text
ADC Samples

1020
1024
1023
1021
1025

↓

Average

1022.6
```

Averaging significantly reduces random measurement noise while introducing only
a small delay.

---

# Why DMA Matters

Without DMA:

```text
ADC
 │
 ▼
CPU
 │
 ▼
RAM
```

The CPU must actively copy every sample.

With DMA:

```text
ADC
 │
 ▼
DMA
 │
 ▼
RAM
```

The transfer happens entirely in hardware.

The CPU is only involved when the application actually needs the data.

---

# Design Takeaways

Several important embedded-system principles are illustrated by these exercises.

- Polling is simple but inefficient.
- Interrupts reduce idle waiting but still consume CPU time.
- DMA enables continuous high-speed sampling with almost no processor overhead.
- Averaging improves measurement stability without additional hardware.

For continuously sampled sensors, combining the ADC with Circular DMA is
generally the most efficient solution.
