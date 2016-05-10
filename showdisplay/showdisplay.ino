#include "FastLED.h"

/**
 * An expansion on the FastLED Cylon script
 * 
 * This extension on Cylon demonstrates aysnchronous updates. Rather than using delay()
 * for the timing, which limits the flexibility, it uses timing based on millis() to
 * trigger the updates. This allows us to do two things at once, each with different
 * update schedules.
 * 
 * I also added a button (optional) which controls which of the two tasks are running.
 * The button can be either polled or use interrupts.
 * 
 * In this case the original Cylon is running on an Adafruit Noepixel strip (8 LEDs)
 * on a 120 ms cycle. The blinking LED (Pin 7) is running on a 73 ms schedule.
 * 
 * Based on the original Cylon.ino (https://github.com/FastLED/FastLED/blob/master/examples/Cylon/Cylon.ino)
 * 
 * Copyright (c) 2016 - William C Waggoner
 * Licensed under the MIT License (https://opensource.org/licenses/MIT)
 */

// How many leds in your strip?
const unsigned int NUM_LEDS = 30;

// Delays and stuff
const unsigned int CYCLE_DELAY = 120;
const unsigned int BLINK_DELAY = 73;

// Brightness 128 is 1/2 bright, plenty bright enough (the V in HSV)
const byte BRIGHT = 128;

// Saturation 240 is not completely pure (the S in HSV)
const byte SAT = 240;

// Fade scale 128 (1/2 fade) works well for 8 LEDs
const byte SCALE = 128;

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
const byte DATA_PIN = 6;
const byte CLOCK_PIN = 13;
const byte BLINK_PIN = 7;
const byte SWITCH_PIN = 2;

// Debounce time (ms) for pushbutton
const unsigned long DEBOUNCE = 250;

// define USE_INTERRUPTS to vary how the switch is handled, polled or interrupt driven
#define USE_INTERRUPTS

// Define the array of leds
CRGB leds[NUM_LEDS];

// Enable bits
const byte DO_BLINK = 0x01;
const byte DO_CYCLE = 0x02;
#ifdef USE_INTERRUPTS
volatile   // Must be VOLATILE if we use interrupts
#endif
byte mode = 0x00;

void setup() {
  // See the FastLED Blink sketch for other settings
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  pinMode(13, OUTPUT);
  pinMode(BLINK_PIN, OUTPUT);
  digitalWrite(BLINK_PIN, LOW);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
#ifdef USE_INTERRUPTS
  // Link our interrupt service routine to the pin FALLING state
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switch_int, FALLING);
#endif USE_INTERRUPTS
}

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(SCALE);
  }
}

void cycle(unsigned long now) {
  static unsigned int led = 0;
  static int dir = 1;
  static byte hue = 0; // The H in HSV
  static unsigned long next_cycle = 0;

  if (next_cycle <= now) {
    if (mode & DO_CYCLE) {
      // Set the led'th led to hue
      leds[led] = CHSV(hue, SAT, BRIGHT);
      
      // Show the leds
      FastLED.show();
      
      // Fade all the leds to make the "tail"
      // We will regenerate the "head" the next time through
      fadeall();
    
      // Bounds check and reset
      if (led == NUM_LEDS - 1) {
        // We have reached the end of the array, turn it around to go back
        dir = -1;           // And go backwards
      } else if (led == 0) {
        // Back at the beginning
        dir = 1;
        hue++;    // Change color on each loop
      }
      
      // Increment/decrement the LED number
      led += dir; // Next/Prevoius LED
    }
    next_cycle += CYCLE_DELAY;
  }
}

void blink(unsigned long now) {
  static bool onoff = false;
  static unsigned long next_blink = 0;

  if (next_blink <= now) {
    if (mode & DO_BLINK) {
      if (onoff) {
        digitalWrite(BLINK_PIN, HIGH);
      } else {
        digitalWrite(BLINK_PIN, LOW);
      }
      
      onoff = !onoff; // Flip it
    }
    next_blink += BLINK_DELAY;
  }
}

void breathe(unsigned long now) {
  /**
   * Breathe uses Pin 13 to "breathe" the onboard LED.
   * It merely shows that we are running ...
   */
  static char ontime = 0;
  static unsigned int counter = 0;
  static boolean onstate = false;
  static unsigned long mark_time = 0;

  if (mark_time <= now) {
    if (onstate) {
      digitalWrite(13, LOW);
      mark_time = now + 16 - abs(ontime>>3);
    } else {
      digitalWrite(13, HIGH);
      mark_time = now + abs(ontime>>3);
    }
    onstate = !onstate;
    if ((++counter & 0x03) == 0) {  // Only vary ontime every 4 cycles
      ontime += 1;
    }
  }
}

#ifdef USE_INTERRUPTS
/**
 * This section is included only if USE_INTERRUPTS is defined
 * 
 * The interrupt is triggered on the pin FALLING. We increment the
 * mode and set a delay (DEBOUNCE) before we will react to another
 * FALLING interrupt.
 */
void switch_int() {
  static unsigned long next_check = 1000; // Ignore the first second
  unsigned long m = millis();
  if (m >= next_check) {
    mode += 1;
    next_check = m + DEBOUNCE;
  }
}
#endif USE_INTERRUPTS

void loop() {
  static byte hue = 0;
  static unsigned long next_cycle = 0;
  static unsigned long next_blink = 0;
  static boolean is_low = false;
  unsigned long m;

  m = millis();
  
  cycle(m);

  blink(m);

  breathe(m);
  
#ifndef USE_INTERRUPTS
  /** This section is included only if we are not using interrupts
   *  
   *  When the SWITCH_PIN goes LOW we switch then set a flag (is_low)
   *  We then wait for the pin to go HIGH again and then set a delay
   *  to "debounce" the switch. We don't care what the pin does for
   *  DEBOUNCE milliseconds.
   */
  static unsigned long next_check = 0;
  
  if (m >= next_check) {
    if (is_low) {
      if (digitalRead(SWITCH_PIN) == HIGH) {
        next_check = m + DEBOUNCE;
        is_low = false;
      }
    } else  {
      if (digitalRead(SWITCH_PIN) == LOW) {
        mode += 1;
        is_low = true;
      }
    }
  }
#endif USE_INTERRUPTS
}
