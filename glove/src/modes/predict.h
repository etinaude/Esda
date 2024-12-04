#include <Arduino.h>
#include <SPI.h>
#include "core/api.h"
#include "core/hardware.h"
#include "core/ml.h"

Api api = Api();
Hardware hardware = Hardware();

TaskHandle_t MLTask;
TaskHandle_t ApiTask;

int triggerCount = 0;

void cancel(){
  hardware.handleMotor(false);
  hardware.setLED(0, 0);
  // api.sendCancelApi();
  triggerCount = 0;
  delay(200);
}

void detect(){
  hardware.handleMotor(true);

  triggerCount++;
  if(triggerCount > 5){
    api.sendDetectApi(tensorModel.rollingMode, tensorModel.rollingConfidence);
    hardware.blink();
    cancel();
  }
}

void MLTaskFunction( void * pvParameters ){
  tensorModel = TensorModel();

  for(;;){
    hardware.readSensors();
    tensorModel.mlPredict(hardware.flexVals);
    delay(50);
  }
}

void ApiTaskFunction( void * pvParameters ){
  api.setupWifi();
  delay(1000);

  for(;;){
    if(tensorModel.bufferIndex == 0){
      tensorModel.displayResults();
      if(tensorModel.rollingMode > 0){
        // detect();
      } else{
        // cancel();
      }
    }
    delay(100);
  }
}

void predictSetup() {
  xTaskCreatePinnedToCore(MLTaskFunction, "MLTask", 10000, NULL, 1, &MLTask, 0);
  xTaskCreatePinnedToCore(ApiTaskFunction, "ApiTask", 10000, NULL, 1, &ApiTask, 1);
}