#include <FastLED.h>

const uint8_t PIN = 13;
const uint8_t CPM = 12;
const uint8_t DIM = 16;
const uint8_t BRT = 192;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);
}

void breathe(uint16_t m) {
  static uint8_t count = 0;
  static boolean last_pulse = false;
  static uint16_t last_m = 65535;
  static uint8_t level = 0;
  uint8_t cycle;
  boolean pulse;

  // Generating the level once every millisecond reduces quadwave8 to 1.13ms / cycle
  if (m != last_m) {  // Check the cycle every millisecond only
    last_m = m;

    cycle = beat8(CPM); // Sawtooth freq = CPM

    // Now convert the sawtooth into a smoother periodic waveform
    //  level = ease8InOutCubic(cycle);  // 2.54ms / cycle (9.9µs / count)
    //  level = ease8InOutApprox(cycle); // 2.34ms / cycle (9.1µs / count)
    //  level = cubicwave8(cycle);       // 2.62ms / cycle (10.2µs / count)?
    level = quadwave8(cycle);            // 2.50ms / cycle (9.8µs / count)
    //  level = triwave8(cycle);         // 2.26ms / cycle (8.8µs / count)

    // or we could generate the periodic waveform directly
    //  level = beat8(CPM);              // Takes 2.20ms / cycle (8.5µs / count)

    //  level = beatsin8(CPM, DIM, BRT); // 2.82ms / cycle (11.0µs / count)
  }
  
  // Generate the actual pulse
  pulse = count <= level;

  count += 1; // Increment for the next time through

  if (pulse != last_pulse) {  // Checking pulse saves! sin:2.82ms / cycle (11.0µs), saw:2.16ms (8.4µs)
    digitalWrite(PIN, pulse);

    last_pulse = pulse;
  }
}

void loop() {
  uint16_t m = millis();
  breathe(m);
}
