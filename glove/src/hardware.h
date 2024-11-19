#include "FastLED.h"
#include <Arduino.h>

#define NumPixels 1

#define LEDPin 10
#define MotorPin 8
#define ButtonPin 9

#define Flex1Pin 0
#define Flex2Pin 1
#define Flex3Pin 2
#define Flex4Pin 3
#define Flex5Pin 4

int flexVals[5];

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

  FastLED.addLeds<WS2812, LEDPin, GRB>(leds, NumPixels);
  FastLED.setBrightness(255);
}

void handleMotor(bool onOff) {
  digitalWrite(MotorPin, onOff);
}

void setLED(int hue, int val) {
  leds[0] = CHSV(hue, 255, val);
  FastLED.show();
}