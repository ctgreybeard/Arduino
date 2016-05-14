//#include "FastLED.h"
#include "breathe.h"

Breathe b = Breathe();

void setup() {
  Serial.begin(57600);
  Serial.println("Hello, world!");
}

void loop() {
  b.breathe(millis());
}
