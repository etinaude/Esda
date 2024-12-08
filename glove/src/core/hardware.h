#include <Arduino.h>
#include "FastLED.h"
#include <Arduino_JSON.h>

#define NumPixels 1

#define LEDPin 21
#define MotorPin 7
#define ButtonPin 8

#define Flex1Pin 2
#define Flex2Pin 3
#define Flex3Pin 9
#define Flex4Pin 10
#define Flex5Pin 11

#define GREEN 0
#define YELLOW 80
#define PURPLE 120
#define BLUE 200
#define RED 100

CRGB leds[NumPixels];

class Hardware {
  public:
    int flexVals[5];

    Hardware() {}

    void setup(){
      pinMode(MotorPin, OUTPUT);
      pinMode(ButtonPin, INPUT);

      pinMode(Flex1Pin, INPUT);
      pinMode(Flex2Pin, INPUT);
      pinMode(Flex3Pin, INPUT);
      pinMode(Flex4Pin, INPUT);
      pinMode(Flex5Pin, INPUT);


      FastLED.addLeds<WS2812, LEDPin, GRB>(leds, NumPixels);
      FastLED.setBrightness(255);
      setLED(YELLOW);

      delay(200);
    }

    void handleMotor(bool onOff) {
      digitalWrite(MotorPin, onOff);
    }

    void setLED(int hue) {
      leds[0] = CHSV(hue, 255, 255);
      FastLED.show();
    }

    void offLED() {
      leds[0] = CRGB::Black;
      FastLED.show();
    }

    void readSensors(){
      flexVals[0] = analogRead(Flex1Pin);
      flexVals[1] = analogRead(Flex2Pin);
      flexVals[2] = analogRead(Flex3Pin);
      flexVals[3] = analogRead(Flex4Pin);
      flexVals[4] = analogRead(Flex5Pin);
    }

    void print(){
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

    JSONVar convertJson(){
      JSONVar data;
      String jsonString = "[" + String(flexVals[0]) + "," + String(flexVals[1]) + "," + String(flexVals[2]) + "," + String(flexVals[3]) + "," + String(flexVals[4]) + "]";
      data["flexVals"] = jsonString;
      Serial.println(jsonString);

      return data;
    }

    bool isButtonPressed(){
      return digitalRead(ButtonPin) == LOW;
    }

    void blink(){
      setLED(PURPLE);
      delay(500);
      offLED();
      delay(500);
    }
};
