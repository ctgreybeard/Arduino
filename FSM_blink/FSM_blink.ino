#include <Fsm.h>

// State machine variables
#define FLIP_LIGHT_SWITCH true
#define DONT_FLIP 2
#define PIN 12

unsigned long next_flip;

State state_light_on(&on_light_on_enter, &on_light_on_exit);
State state_light_off(&on_light_off_enter, &on_light_off_exit);
Fsm fsm(&state_light_off);

// Transition callback functions
void on_light_on_enter()
{
  Serial.println("Entering LIGHT_ON");
  digitalWrite(PIN, HIGH);
}

void on_light_on_exit()
{
  Serial.println("Exiting LIGHT_ON");
}

void on_light_off_enter()
{
  Serial.println("Entering LIGHT_OFF");
  digitalWrite(PIN, LOW);
}

void on_light_off_exit()
{
  Serial.println("Exiting LIGHT_OFF");
}

void on_trans_light_on_light_off()
{
  Serial.print("Transitioning from LIGHT_ON to LIGHT_OFF at ");
  Serial.println(millis());
  next_flip += 2000;  // Turn on in 2 sec
}

void on_trans_light_off_light_on()
{
  Serial.print("Transitioning from LIGHT_OFF to LIGHT_ON at ");
  Serial.println(millis());
  next_flip += 100; /// Turn OFF in .5 sec
}

// standard arduino functions
void setup()
{
  Serial.begin(115200);

  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);

  next_flip = 0;

  fsm.add_transition(&state_light_on, &state_light_off,
                     FLIP_LIGHT_SWITCH,
                     &on_trans_light_on_light_off);
  fsm.add_transition(&state_light_off, &state_light_on,
                     FLIP_LIGHT_SWITCH,
                     &on_trans_light_off_light_on);
}

void loop()
{
  fsm.trigger(millis() >= next_flip);
}
