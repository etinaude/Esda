#include <Arduino.h>
#include <Arduino_JSON.h>
#include "FastLED.h"
#include "api.h"
#include "hardware.h"
#include <SPI.h>

int issueDetected = false;

void setup() {
  Serial.begin(9600);
  setupPins();
  setupWifi();
}

void loop() {
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
