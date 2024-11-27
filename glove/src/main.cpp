#include <Arduino.h>
#include <Arduino_JSON.h>
#include <SPI.h>
#include "api.h"
#include "hardware.h"
#include "ml.h"

TensorModel tensorModel;
Api api = Api();
Hardware hardware = Hardware();

void setup() {
  Serial.begin(9600);
  api.setupWifi();
  tensorModel = TensorModel();
  Serial.println("Setup done");
}

void training(){
  hardware.readSensors();
  hardware.printJson();
  delay(500);
}

void detect(){
  hardware.setLED(0, 255);  //red
  hardware.handleMotor(true);
  api.sendDetectApi(tensorModel.rollingMode, tensorModel.rollingConfidence);
}

void prediction(){
  hardware.readSensors();
  tensorModel.mlPredict(hardware.flexVals);

  if(tensorModel.bufferIndex == 0){
    tensorModel.displayResults();

    if(tensorModel.rollingMode > 0){
      detect();
    }
  }

  delay(100);
}

void blink(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

void cancel(){
  hardware.handleMotor(false);
  hardware.setLED(0, 0);
  api.sendCancelApi();
}

void loop() {
  //  training();
  prediction();
  //  blink();
}
