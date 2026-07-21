# 07_Dimming_Dot — PWM duty-cycle dimming

Reuses [`06_Blinky_Dot`](../06_Blinky_Dot)'s Output-Compare setup, but sweeps duty cycle instead of frequency: dims the dot segment's *perceived* brightness rather than blinking it visibly.

## What this covers

- The perceptual basis for PWM dimming: the human eye can't temporally resolve more than ~25 frames/second, so at a sufficiently high blink frequency it perceives the time-averaged brightness rather than discrete on/off flicker.
- Driving the same Output-Compare channel from [`06_Blinky_Dot`](../06_Blinky_Dot), but varying the compare value (duty cycle) instead of the timer period (frequency).
- Mapping a potentiometer reading to a duty-cycle percentage, giving continuous (in appearance) brightness control via [`dot`](../modules/dot)'s dimming functions.

See [`dot`](../modules/dot) for the API.
