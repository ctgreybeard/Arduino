#import "Modulo.h"

DisplayModulo display;

void setup() {
  // put your setup code here, to run once:
  Modulo.setup();

  display.clear();
  for (int i = 0; i < display.height(); i += 2) {
    display.setCursor(i, i);
    display.print("-.-");
  }

  display.refresh();
}

void loop() {
  // put your main code here, to run repeatedly:
  Modulo.loop();
}
