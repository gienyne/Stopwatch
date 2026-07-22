# median

Median filter used to suppress outliers in noisy sensor measurements.

Unlike a moving average, a median filter rejects isolated spikes while
preserving fast signal changes, making it particularly suitable for tachometer
measurements where occasional invalid pulses would otherwise produce incorrect
RPM estimates.

## API

```c
uint32_t median_get_median(uint32_t newElement);
```

## Implementation

The module maintains a fixed-size circular buffer.

Whenever a new sample arrives

1. the sample is inserted into the buffer,
2. the buffer is copied,
3. the copy is sorted,
4. the middle element (median) is returned.

A small moving-average smoothing stage is then applied to further reduce
measurement jitter.

## Used in

- P1_Fan_Control

to filter tachometer-derived RPM measurements before they are processed by the
PI controller.
