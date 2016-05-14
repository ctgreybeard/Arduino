# Breathe

## A simple "I'm still alive" indicator

Add the FastLED (https://github.com/FastLED/FastLED.git) and this Breathe library
to the Arduino app then include it in your code.

A sample app is shown below ...


    #include "breathe.h"

    Breathe b = Breathe();

    void setup() {
      Serial.begin(57600);
      Serial.println("Hello, world!");
    }

    void loop() {
      b.breathe(millis());
    }

## Discussion

I wanted to make something that would let me see that the sketch was actually running
and hadn't crashed.
I also wanted something that could be easily added and wouldn't take up
too much time in the main loop.
This is the result.

I use the FastLED library because:

1. I will probably be using it anyway
1. It has a fast pulse width routine that produces a sinosoidal waveform. Breathing is, after all, sinosoidal.

Having said that I actually went with the "quad" waveform as it's slightly more obvious that it's breathing.
YMMV and you are free to experiment.
I have left the different codes in for most of the available waveforms.

This is a very simple routine; at every loop cycle it checks the time, we only do the computation once every millisecond.
Then the high/low pulse is calculated and sent to the LED if it has changed from the last.

`beatsin8(CPM, DIM, BRT)` returns a uint8_t that varies sinosidally between the DIM and BRT
parameters with a cycle time of (in this case) `CPM` per minute.
I chose 12 for `CPM` because that's a normal human breathing rate.
And `DIM` and `BRT` for a pleasing effect.
These last two values don't have much to do with how it looks, however.

The result is that we go through the 256 counts pretty quickly but the actual changes to 
the output of beatsin8 happen much more slowly.
This means that the pulse duty cycle changes slowly but the pulses themselves are generated very quickly.
It is the pulse duty cycle that changes the apparent brightness of the LED.
The actual pulse width is dependent on the time it takes to count through all 256 trips through `breathe()`.

Each pulse consists of some variable *on* time followed by *off* time through the end of the pulse.
The width of the pulse for *on* to the *on* for the next pulse is how long it takes to count 256
times through `breathe()`.
This time should be pretty fast so that the LED doesn't "flicker."

Our results were very acceptable.

### Timing

The main purpose for this particular little program is to test how much time generating this _Breathe_
indicator takes away from whatever the main purpose of the Sketch is.
We wouldn't want it to adversely effect whatever it is we are actually doing.
By running it by itself we can get a pretty good idea of what load it is putting on the system.

I am running this on an Adafruit Metro Mini which is based on an Arduino UNO.
It is built with an ATmega328 running at 16MHz.

I used a digital scope to measure the pulse width at the output pin.
Each full pulse is built from 256 trips through the `breath()` function.
By dividing the pulse width by 256 we can determine the service time for each trip through `breathe()`.

In the first iteration I wrote to the pin every time through `breathe()`.
This meant that I was frequently writing the same state to the pin each loop through.

The result was that the `beatsin8()` loop was generating pulses of 3.85ms, meaning the single cycle time was 15.0µs.
I considered this to be very acceptable.

But I wondered whether it would be a benefit to only write to the pin when the value changes.
This would add overhead in checking and saving the output state but save time by not calling
`digitalWrite()` unless we needed to.
The results were encouraging.

After I added `last_pulse` and the logic to check and save it the pulse width went down to 2.82ms (11.0µs cycle time.)
This means that the `digitalWrite` added a full millisecond for the 256 times through for a full pulse.

The extra overhead to avoid the unneeded `digitalWrite()` calls is well worth it!

#### Saw vs Sine

The other choice that I made was to use the sine wave function `beatsin8()` so that the LED "breathed" ...
There are other generators in FastLED that would also work.
One is `beats8()` which generates a sawtooth wave form.

In trying this function it is definitely less pleasing aesthetically.
But it is functional.

`beat8()` produced a pulse width of 2.16ms (8.4µs) which is a gain of nearly 0.7ms or 2.6µs per cyclee.
If your `loop()` cycle is very tight those 2+ microseconds might be worth the trouble.

## Conclusion

Adding _Breathing_ as a health indicator to your project isn't difficult, doesn't take a lot of time away from
"productive" efforts, and does give positive feedback that the entire mess hasn't crashed and burned.

Adding the code takes about 272 bytes so it is not very heavy and adds about 11µs to the main loop.
By changing to a less intensive sawtooth generator the loop cycle time can be brought do to 8.4µs.
