#include <Arduino.h>
#include <SPI.h>
// #include "modes/training.h"
// #include "core/hardware.h"

#include "modes/predict.h"

void setup() {
  Serial.begin(9600);
  predictSetup();
}

void loop() {
  // training();
  // delay(500);
}
