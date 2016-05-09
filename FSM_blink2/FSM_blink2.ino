#include <Fsm.h>

//#define DEBUG

// State machine variables
#define FLIP_LIGHT_SWITCH true

#define PIN1 12
#define PIN2 13

uint64_t next_flip1;
uint64_t next_flip2;

State state_light_on(&on_light_on_enter, &on_light_on_exit);
State state_light_off(&on_light_off_enter, &on_light_off_exit);
Fsm fsm(&state_light_off);

// Transition callback functions
void on_light_on_enter()
{
#ifdef DEBUG
  Serial.println("Entering LIGHT_ON");
#endif
  digitalWrite(PIN1, HIGH);
}

void on_light_on_exit()
{
#ifdef DEBUG
  Serial.println("Exiting LIGHT_ON");
#endif
}

void on_light_off_enter()
{
#ifdef DEBUG
  Serial.println("Entering LIGHT_OFF");
#endif
  digitalWrite(PIN1, LOW);
}

void on_light_off_exit()
{
#ifdef DEBUG
  Serial.println("Exiting LIGHT_OFF");
#endif
}

void on_trans_light_on_light_off()
{
#ifdef DEBUG
  Serial.print("Transitioning from LIGHT_ON to LIGHT_OFF at ");
  Serial.println(millis());
#endif
  next_flip1 += 2000;  // Turn on in 2 sec
}

void on_trans_light_off_light_on()
{
#ifdef DEBUG
  Serial.print("Transitioning from LIGHT_OFF to LIGHT_ON at ");
  Serial.println(millis());
#endif
  next_flip1 += 100; /// Turn OFF in .1 sec
}

// standard arduino functions
void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.print("An unsigned long is ");Serial.print(sizeof(unsigned long));Serial.println(" bytes");
  Serial.print("An uint32_t is ");Serial.print(sizeof(uint32_t));Serial.println(" bytes");
  Serial.print("An uint64_t is ");Serial.print(sizeof(uint64_t));Serial.println(" bytes");
#endif

  pinMode(PIN1, OUTPUT);
  digitalWrite(PIN1, LOW);
  pinMode(PIN2, OUTPUT);
  digitalWrite(PIN2, LOW);

  next_flip1 = 0;
  next_flip2 = 0;

  fsm.add_transition(&state_light_on, &state_light_off,
                     FLIP_LIGHT_SWITCH,
                     &on_trans_light_on_light_off);
  fsm.add_transition(&state_light_off, &state_light_on,
                     FLIP_LIGHT_SWITCH,
                     &on_trans_light_off_light_on);
}

void loop()
{
  fsm.trigger(millis() >= next_flip1);
}
