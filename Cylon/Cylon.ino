#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 8

// Delay and stuff
#define DELAY 120
#define BRIGHT 128
#define SAT 240
#define SCALE 128 // 128 (1/2 fade) works well for 8 LEDs

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  LEDS.addLeds<NEOPIXEL, DATA_PIN>(leds,NUM_LEDS);
  LEDS.setBrightness(BRIGHT);
}

void fadeall() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(SCALE);
  }
}

void cycle(int led, uint8_t hue) {
    // Set the led'th led to red
    leds[led] = CHSV(hue, SAT, BRIGHT);
    // Show the leds
    FastLED.show();
    // Fade all the leds to make the "tail"
    // We will regenerate the "head" the next time through
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(DELAY);
}

void loop() {
  static uint8_t hue = 0;
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    cycle(i, hue);
  }
  
  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    cycle(i, hue);
  }
  
  // Change the color each time through
  hue += 1;
}
