#include <Arduino.h>
#include <SPI.h>
#include "core/api.h"
#include "core/hardware.h"
#include "core/ml.h"

Api api = Api();
Hardware hardware = Hardware();

void training(){
  hardware.readSensors();
  JSONVar json = hardware.convertJson();

  api.sendTrainApi(json);
  delay(500);
}
