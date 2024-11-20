#include <Arduino.h>
#include <Arduino_JSON.h>
#include "FastLED.h"
#include "api.h"
#include "hardware.h"
#include <SPI.h>
// #include "training.h"

int issueDetected = false;

void setup() {
  Serial.begin(9600);
  setupPins();
  setupWifi();

  Serial.println("Setup done");
}

void sendDetectApi() {
  issueDetected = true;
  setLED(0, 255);  //red
  handleMotor(true);

  JSONVar data;

  data["accuracy"] = 90;
  data["pose"] = 2;

  // send api
  sendAPI("/detect", data);
}

void sendStatusApi() {
  // TODO
}

void sendCancelApi() {
  handleMotor(false);
  setLED(0, 0);  //off

  JSONVar data;
  sendAPI("/cancel", data);
}


void sendTrainingData(){

  JSONVar data;

  String jsonString = "["+ String(flexVals[0]) + "," + String(flexVals[1]) + "," + String(flexVals[2]) + "," + String(flexVals[3]) + "," + String(flexVals[4]) + "]";

  data["flexVals"] = jsonString;

  Serial.println(data["flexVals"]);


  sendAPI("/train", data);
}

void detectIssue(){
  if(flexVals[0] < 20000){
    sendDetectApi();
  }
}

void loop() {
  getTrainingData();
  sendTrainingData();

  delay(3000);
}
