#include <FastLED.h>

const uint8_t PIN = 13;
const uint8_t CPM = 12;
const uint8_t DIM = 16;
const uint8_t BRT = 192;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);
//  Serial.begin(57600);
//  Serial.print("LOW is ");Serial.println(LOW);
//  Serial.print("HIGH is ");Serial.println(HIGH);
//  Serial.print("true is ");Serial.println(true);
//  Serial.print("false is ");Serial.println(false);
}

void breathe() {
  static uint8_t count = 0;
  static uint8_t last_pulse = 127;
  uint8_t cycle;
  uint8_t level;
  uint8_t pulse;
  
  cycle = beat8(CPM); // Sawtooth freq = CPM
//  level = cycle;
//  level = ease8InOutCubic(cycle); // 2.54ms / cycle (9.9µs / count)
//  level = ease8InOutApprox(cycle); // 2.34ms / cycle (9.1µs / count)
//  level = cubicwave8(cycle); // 2.62ms / cycle (10.2µs / count)?
  level = quadwave8(cycle); // 2.50ms / cycle (9.8µs / count)
//  level = triwave8(cycle); // 2.26ms / cycle (8.8µs / count)

//  level = beat8(CPM); // Takes 2.20ms / cycle (8.5µs / count)

//  level = beatsin8(CPM, DIM, BRT);  // 2.82ms / cycle (11.0µs / count)

// Generate the actual pulse
  pulse = squarewave8(count, level);
  
  if (pulse != last_pulse) {  // Checking pulse saves! sin:2.82ms / cycle (11.0µs), saw:2.16ms (8.4µs)
    digitalWrite(PIN, pulse == 255 ? HIGH : LOW);
    
    last_pulse = pulse;
  }
  
  count += 1;
}

void loop() {
  breathe();
}
