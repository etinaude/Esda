// Glove code

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "FastLED.h"
#include <SPI.h>

#define NumPixels 1
#define LEDPin 11
#define MotorPin 9
#define ButtonPin 10

#define Flex1Pin 3
#define Flex2Pin 4
#define Flex3Pin 5
#define Flex4Pin 6
#define Flex5Pin 7

int flexVals[5];
int issueDetected = false;

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

void sendDetectApi() {

  issueDetected = true;
  setLED(0, 255);  //red
  handleMotor(true);

  JSONVar data;

  data["deviceId"] = deviceID;
  data["accuracy"] = 90;
  data["pose"] = 2;

  String jsonString = JSON.stringify(data);

  // send api
  sendAPI("/detect", jsonString);
}

void sendStatusApi() {
  // TODO
}

void sendCancelApi() {
  handleMotor(false);
  setLED(0, 0);  //off

  JSONVar data;

  data["deviceId"] = deviceID;
  String jsonString = JSON.stringify(data);

  sendAPI("/cancel", jsonString);
}

void sendAPI(String endPoint, String data) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverName + endPoint);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(data);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    http.end();
  }
}

void setLED(int hue, int val) {
  leds[0] = CHSV(hue, 255, val);
  FastLED.show();
}

void handleMotor(bool onOff) {
  digitalWrite(MotorPin, onOff);
}

void setupPins() {
  pinMode(MotorPin, OUTPUT);
  pinMode(LEDPin, OUTPUT);

  pinMode(ButtonPin, INPUT);

  pinMode(Flex1Pin, INPUT);
  pinMode(Flex2Pin, INPUT);
  pinMode(Flex3Pin, INPUT);
  pinMode(Flex4Pin, INPUT);
  pinMode(Flex5Pin, INPUT);
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