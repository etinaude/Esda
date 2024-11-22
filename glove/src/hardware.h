#include <Arduino.h>
#include "FastLED.h"

#define NumPixels 1

#define LEDPin 37
#define MotorPin 35
#define ButtonPin 33

#define Flex1Pin 11
#define Flex2Pin 9
#define Flex3Pin 7
#define Flex4Pin 5
#define Flex5Pin 3

int flexVals[5];
int flexPercentVals[5];


CRGB leds[NumPixels];

void setupPins() {
  pinMode(MotorPin, OUTPUT);
  pinMode(LEDPin, OUTPUT);

  pinMode(ButtonPin, INPUT);

  pinMode(Flex1Pin, INPUT);
  pinMode(Flex2Pin, INPUT);
  pinMode(Flex3Pin, INPUT);
  pinMode(Flex4Pin, INPUT);
  pinMode(Flex5Pin, INPUT);

  // FastLED.addLeds<WS2812, LEDPin, GRB>(leds, NumPixels);
  // FastLED.setBrightness(255);
}

void handleMotor(bool onOff) {
  digitalWrite(MotorPin, onOff);
}

void setLED(int hue, int val) {  leds[0] = CHSV(hue, 255, val);
  FastLED.show();
}

void readSensors(){
  flexVals[0] = analogRead(Flex1Pin);
  flexVals[1] = analogRead(Flex2Pin);
  flexVals[2] = analogRead(Flex3Pin);
  flexVals[3] = analogRead(Flex4Pin);
  flexVals[4] = analogRead(Flex5Pin);

  flexVals[1] = 0;
  flexVals[2] = 0;
  flexVals[3] = 0;
  flexVals[4] = 0;
}

void printSensors(){
  Serial.print("Low:");
  Serial.print(2000);
  Serial.print(" High:");
  Serial.print(9000);

  Serial.print(" Flex1:");
  Serial.print(flexVals[0]);
  Serial.print(" Flex2:");
  Serial.print(flexVals[1]);
  Serial.print(" Flex3:");
  Serial.print(flexVals[2]);
  Serial.print(" Flex4:");
  Serial.print(flexVals[3]);
  Serial.print(" Flex5:");
  Serial.print(flexVals[4]);
  Serial.println();
}