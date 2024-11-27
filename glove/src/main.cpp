#include <Arduino.h>
#include <Arduino_JSON.h>
#include <SPI.h>
#include "api.h"
#include "hardware.h"
#include "ml.h"

TensorModel tensorModel;
Api api = Api();
Hardware hardware = Hardware();

int triggerCount = 0;

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

void blink(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
}

void cancel(){
  hardware.handleMotor(false);
  hardware.setLED(0, 0);
  // api.sendCancelApi();
  triggerCount = 0;
}

void detect(){
  hardware.handleMotor(true);

  triggerCount++;
  if(triggerCount > 5){
    api.sendDetectApi(tensorModel.rollingMode, tensorModel.rollingConfidence);
    blink();
    cancel();
  }

}

void prediction(){
  hardware.readSensors();
  tensorModel.mlPredict(hardware.flexVals);

  if(tensorModel.bufferIndex == 0){
    tensorModel.displayResults();

    if(tensorModel.rollingMode > 0){
      detect();
    } else{
      cancel();
    }
  }

  delay(150);
}



void loop() {
  //  training();
  prediction();

  //  blink();
}
