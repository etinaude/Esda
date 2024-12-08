#include <Arduino.h>
#include <SPI.h>
#include "core/api.h"
#include "core/hardware.h"
#include "core/ml.h"
#include "core/websockets.h"


Api api;
Hardware hardware;
TensorModel tensorModel;
Websocket websocket;

TaskHandle_t MLTask;
TaskHandle_t ApiTask;

int triggerCount = 0;

void cancel(){
  hardware.handleMotor(false);
  hardware.offLED();
  triggerCount = 0;
  delay(200);
}

void detect(){
  hardware.handleMotor(true);
  hardware.setLED(BLUE);

  triggerCount++;
  if(triggerCount > 5){
    api.sendDetectApi(tensorModel.rollingMode, tensorModel.rollingConfidence);
    hardware.blink();
    cancel();
  }
}

void MLTaskFunction( void * pvParameters ){
  for(;;){
    hardware.readSensors();
    tensorModel.mlPredict(hardware.flexVals);
    delay(50);
  }
}

void ApiTaskFunction( void * pvParameters ){
  for(;;){
    if(tensorModel.bufferIndex == 0){
      websocket.poll();
      JSONVar data = hardware.convertJson();
      tensorModel.displayResults();

      data["confidence"] = tensorModel.rollingConfidence;
      data["pose"] = tensorModel.rollingMode;

      String jsonString = JSON.stringify(data);

      websocket.sendMessage(jsonString);

      if(tensorModel.rollingMode > 0){
        // detect();
      } else{
        cancel();
      }
    }
    delay(100);
  }
}

void predictSetup() {
  hardware.setup();
  hardware.setLED(RED);

  tensorModel.setup();
  hardware.setLED(YELLOW);

  api.setup();
  websocket.connect();
  hardware.setLED(GREEN);

  delay(500);
  hardware.offLED();

  xTaskCreatePinnedToCore(MLTaskFunction, "MLTask", 10000, NULL, 1, &MLTask, 0);
  xTaskCreatePinnedToCore(ApiTaskFunction, "ApiTask", 10000, NULL, 1, &ApiTask, 1);
}