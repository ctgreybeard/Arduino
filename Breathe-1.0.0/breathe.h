/**
 * breath - Slowly pulse an LED, usually on PIN 13, to show we are alive ...
 */

#include "FastLED.h"

struct Breathe {

    public:
//    Breathe();
//    Breathe(uint8_t CPM);
    Breathe(uint8_t CPM=12, uint8_t PIN=13);
    void breathe(uint16_t millis);

    private:
    uint8_t cpm;
    uint8_t pin;
    void setup();
};
