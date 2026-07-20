# median —  filter

```c
uint32_t median_get_median(uint32_t newElement);
```

Returns the median of the window each time a new value is pushed in. Used in [`fan-control`](../../projects/fan-control) to filter the tachometer-derived RPM reading, which is noisy due to signal glitches on the tacho line — a median filter rejects isolated outlier pulses without the lag a moving-average filter would introduce.
