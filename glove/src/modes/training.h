#include <Arduino.h>
#include <SPI.h>
#include "core/api.h"
#include "core/hardware.h"

Api api;
Hardware hardware;

void trainingSetup() {
  hardware.setup();
  api.setup();
}

void training(){
  hardware.readSensors();
  JSONVar json = hardware.convertJson();

  api.sendTrainApi(json);
  delay(500);
}
