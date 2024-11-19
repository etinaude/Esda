// Glove code

#include <Arduino_JSON.h>
#include "FastLED.h"
#include <SPI.h>

#define NumPixels 1
#define LEDPin 11
#define valvePinA 9
#define valvePinB 9

CHSV leds[NumPixels];

const char* ssid = "etienne";
const char* password = "etienne123";
String deviceID = "cf414906-b98b-4c42-bcac-c64987501bd9";
String serverName = "http://192.168.1.106:1880";

void setup() {
  Serial.begin(9600);
  setupPins();
  FastLED.addLeds(leds, NumPixels);
  setupWifi();
}

void loop() {
}


void handleMotor(bool onOff) {
}

void setupPins() {

}

void setupWifi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}