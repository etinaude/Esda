#include <Arduino.h>
// #include "modes/training.h"
#include "modes/predict.h"

void setup() {
  Serial.begin(9600);
  Serial.println(".");
  predictSetup();

  // hardware.setup(); // DEBUG

  // trainingSetup();
}

void loop() {
  // hardware.blink(); // DEBUG

  // training();
}
